#include "Ligne.h"
#include <fstream>
#include <QMessageBox>
using std::string;
using std::ifstream;

Ligne::Ligne() : d_t{nullptr},d_q{nullptr}, d_nbArrets{0}
{}

Ligne :: Ligne(const string& TextLigne) :
    d_t{nullptr}, d_q{nullptr}, d_numLigne{-1}, d_nbArrets{0}
{
    int duree;
    pos position;
    string name; // sert à lire les blocks inutiles au programme
    int r, v, b;

    ifstream fArret ("./Files/" + TextLigne); // fichier pour les arrêts d'une ligne
    std::ofstream lelogWORK("./Logs/LOGSLIGNE.TXT");

    if (!fArret)
    {   // informe l'utilisateur du problème
        lelogWORK << "Impossible d'ouvrir les fichiers (erreur sur les fichiers Ligne).";
        QMessageBox msgBox;
        msgBox.setText("Les documents de lignes sont introuvables, essayez de les placer dans le dossier \"Build/Files\".");
        msgBox.exec();
        return;
    }

    lelogWORK << "Les fichiers sont ouverts (pas d'erreurs sur les fichiers Ligne).";

    fArret >> name >> duree;
    d_numLigne = duree; // durée sert ici à stocker le numéro de la ligne entrant du fichier

    fArret >> name >> name >> name >> name >> name >> r >> v >> b;
    couleur = QColor(r, v, b); // définie la couleur de la ligne

    fArret >> name >> name >> name >> duree;
    d_nbArrets = duree; // durée sert ici à stocker le nombre d'arrêts de la ligne entrant du fichier

    Arret* tabArrets = new Arret [d_nbArrets];

    string ID = std::to_string(getNumLigne());
    std::ofstream lelog("./Logs/LOGSLIGNE " + ID + ".TXT");

    for (unsigned int i = 0; i<d_nbArrets; i++)
    {
        string fullname = "";
        name = "name";

        // Sert à lire les noms de façon naturelle.
        // Les arrêts peuvent avoir des espaces sans que cela casse la lecture du fichier en c++

        while (name.find(";") == string::npos) // séparateur entre les infos de la ligne
        {
            fArret >> name;

            if (name != ";")
                fullname += name + " ";
        }

        fArret >> duree >> name >> position.x >> name >> name >> position.y >> name >> name;

        lelog << fullname << " " <<duree << " " <<  position.x << "x " << position.y << "y " << endl;

        tabArrets[i].d_nom = QString::fromStdString(fullname);
        tabArrets[i].position = position;
        tabArrets[i].tempsArret = duree;
    }

    d_t = new Arret(tabArrets[0].d_nom, tabArrets[0].position.x, tabArrets[0].position.y, tabArrets[0].tempsArret);
    Arret* prec = d_t;
    Arret* na = nullptr;

    for (unsigned int i = 1; i<d_nbArrets; i++)
    {
        na = new Arret(tabArrets[i].d_nom, tabArrets[i].position.x, tabArrets[i].position.y, tabArrets[i].tempsArret);
        prec->d_suiv = na;
        prec->d_distance = prec->distanceSuivant();
        prec = na;
    }
    na->sens = false;

    // Pour dissocier l'aller du retour, on stocke les arrêts dans le sens inverse.
    // Ils seront parcourus à l'envers, mais leurs position ne change pas
    // selon le sens. On aura l'impression d'avoir 2 voies parallèles.

    for (unsigned int i = d_nbArrets - 2; i>0; i--)
    {
        na = new Arret(tabArrets[i].d_nom, tabArrets[i].position.x, tabArrets[i].position.y, tabArrets[i].tempsArret);
        na->sens = false;
        prec->d_suiv = na;
        prec->d_distance = prec->distanceSuivant();
        prec = na;
    }
    na->d_suiv = d_t;
    na->d_distance = na->distanceSuivant();
    d_q = na;

    delete [] tabArrets;
}

Ligne::~Ligne()
{
    Arret* a = d_t;
    while(a != d_q)
    {
        d_t=d_t->d_suiv;
        delete a;
        a = d_t;
    }
    delete a;
}

void Ligne::draw(QGraphicsScene* scene) const
{
    Arret* a = d_t;
    QPen pen{couleur, 8, Qt::SolidLine};
    QRectF stat{d_t->position.x-3, d_t->position.y-3, 6, 6};
    scene->addRect(stat,pen);

    for (unsigned int i = 0 ; i < d_nbArrets-1; i++)
    {
        QLineF line{a->position.x, a->position.y, a->d_suiv->position.x, a->d_suiv->position.y};
        QRectF ASuivre{a->d_suiv->position.x-3, a->d_suiv->position.y-3, 6, 6};

        scene->addLine(line,pen);
        scene->addRect(ASuivre,pen);
        a=a->d_suiv;
    }
}

void Ligne::afficherListeArret(ostream &os) const
{
    Arret *c=d_t;

    for (unsigned int i=0; i<d_nbArrets; i++)
    {
        pos position = c->position;
        os << "Nom de l'arret : " << c->d_nom.toStdString() << endl;
        os << "Duree de l'arret : " << c->tempsArret << " secondes" << endl;
        os << "Position (x;y) : (" << position.x << ";" << position.y << ")" << endl << endl;
        c=c->d_suiv;
    }

    os << endl;
}

void Ligne::insererArret(const Arret &a)
{

    Arret *nc = new Arret(a);

    if (d_t == nullptr)
    {
        d_t = d_q = nc;
        nc->d_suiv=nullptr;
        return;
    }

    else
    {
        Arret *c=d_t , *prec=nullptr;

        while(c!=nullptr)
        {
            prec=c;
            c=c->d_suiv;
        }

        prec->d_suiv=nc;
        nc->d_suiv=nullptr;
        d_q=nc;
    }
}

bool Ligne::chercherArret(const Arret &a) // renvoie si un arrêt est présent dans la ligne
{
    bool found = false;
    QString name = a.d_nom;
    auto c = d_t;

    while (c and !found)
    {
        if (c->d_nom == name)
            found = true;

        c = c->d_suiv;
    }

    return found;
}

void Ligne::supprimerArret(const Arret &a)
{
    QString name = a.d_nom;

    if (!d_t or chercherArret(a)==false)
    {
        return;
    }

    else if (d_t->d_nom == name)
    {
        auto c = d_t;
        d_t = c->d_suiv;
        delete c;
    }

    else
    {
        auto c = d_t;
        Arret* prec;

        while (c and c->d_nom != name)
        {
            prec = c;
            c = c->d_suiv;
        }

        if (c)
        {
            prec->d_suiv = c->d_suiv;
            delete c;
        }
    }
}

int Ligne::getNumLigne()
{
    return d_numLigne;
}
