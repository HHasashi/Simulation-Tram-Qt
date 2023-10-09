#include "ReseauTram.h"
#include <fstream>
#include <QMessageBox>
using std::string;
using std::ifstream;

ReseauTram::ReseauTram(Ligne* lgn, const string& TextTram, QGraphicsScene* scene) :
    t_Tete{nullptr}, d_nbTram{lgn->d_nbArrets}


{
    ifstream fTram("./Files/" + TextTram); // fichier qui permet d'avoir les infos sur le réseau de trams (Trams i.txt)
    string ID = std::to_string(lgn->getNumLigne());
    std::ofstream lelog("./Logs/LOGSRESEAU " + ID + ".TXT");

    if (!fTram or lgn->d_numLigne == -1)
    {   // informer l'utilisateur de la démarche à faire
        lelog << "Impossible d'ouvrir les fichiers (erreur sur les fichiers Trams).";
        QMessageBox msgBox;
        msgBox.setText("Les documents de trams sont introuvables, essayez de les placer dans le dossier \"Build/Files\".");
        msgBox.exec();
        return;
    }

    string chara; // sert à lire les blocks non-utiles au programme
    double IAmSpeed; // référence à CARS, récupère la vitesse du tram numéro i

    fTram >> chara >> chara >> chara >> chara >> d_nbTram;

    fTram >> chara >> chara >> chara >> chara >> IAmSpeed >> chara;

    Arret* arretCrt = lgn->d_t;
    t_Tete = new Tram{lgn, arretCrt, IAmSpeed};
    Tram* tramCourant = t_Tete;
    t_Tete->ID = 0;
    t_Tete->setZValue(1); // fait en sorte que le carré du tram soit au 1er plan (devant le tracé de la ligne de tram)
    scene->addItem(t_Tete);
    lelog << "Tram " << 0 << " de vitesse : " << IAmSpeed << " KM/h" << endl;

    unsigned int nbTrams = 1;

    for(int unsigned i=1; i < lgn->d_nbArrets; i++)
    {
        arretCrt = arretCrt->d_suiv;

        if (nbTrams == d_nbTram)
            break;

        if (i%2==0)
        {   // Calqué sur le réseau SOLÉA de l'agglomération de Mulhouse.
            //  Sachant qu'il y a un tram toute les 5 minutes par arrêt,
            // et qu'il faut 15 minutes pour parcourir 12 arrêts,
            // il y a donc en moyenne 1 minute 15 de trajet inter-arrêts.
            // 300 secondes / 75 secondes = 4, un tram tout les 4 arrêts.
            // Sur 2 voies ça fait 2x plus d'arrêts
            // donc on pose un tram tout les 4 arrêts aller/retour
            // soit tout les2 arrêts en soit.

            fTram >> chara >> chara >> chara >> chara >> IAmSpeed >> chara;
            tramCourant->d_suiv = new Tram{lgn, arretCrt, IAmSpeed};
            tramCourant = tramCourant->d_suiv;
            tramCourant->setZValue(1);
            tramCourant->ID = nbTrams++;
            scene->addItem(tramCourant);

            lelog << "Tram " << nbTrams << " de vitesse : " << IAmSpeed << " KM/h" << endl;
        }
    }

    tramCourant->d_suiv = t_Tete;
    d_nbTram = nbTrams;
}

ReseauTram::~ReseauTram()
{
    Tram* t = t_Tete;

    for(int unsigned i =0; i<d_nbTram; i++)
    {
        t_Tete = t_Tete->d_suiv;
        delete t;
        t = t_Tete;
    }
    delete t;
}

void ReseauTram::supprimerTram(const Tram &t)
{
    int ID = t.ID;

    if (!t_Tete)
    {
        return;
    }

    else if (t_Tete->ID == ID)
    {
        auto c = t_Tete;
        t_Tete = c->d_suiv;
        delete c;
    }

    else
    {
        auto c = t_Tete;
        Tram* prec;

        while (c and c->ID != ID)
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

void ReseauTram::insererTram(const Tram &t)
{
    Tram *nt = new Tram (t.principale, t.comminus, t.d_vitesse);

    if(t_Tete==nullptr)
    {
        t_Tete=nt;
        nt->d_suiv=nullptr;
        return;
    }

    else
    {
        Tram *tc = t_Tete , *prec=nullptr;
        while(tc!=nullptr)
        {
            prec=tc;
            tc=tc->d_suiv;
        }
        prec->d_suiv=nt;
        nt->d_suiv=nullptr;
    }
}

void ReseauTram::afficherListeTram(ostream &os)
{
    Tram *c = t_Tete;

    for (unsigned int i=0; i<d_nbTram; i++)
    {
        os << "Tramway numero : ";

        if (c->ID < 10)
            os << "00" << c->ID << endl;

        else if (c->ID < 100)
            os << "0"  << c->ID << endl;

        else os << c->ID << endl;

        os << "Numero de sa ligne : " << c->principale->d_numLigne << endl;
        os << "Position (x;y) : (" << c->d_x << ";" << c->d_y << ")" << endl;
        os << "Vitesse maximum : " << c->d_vitesse << " KM/h" <<endl;

        if (c->comminus->sens == true)
            os << "Sens de deplacement : Aller" << endl << endl;

        else os << "Sens de deplacement : Retour" << endl << endl;

        c=c->d_suiv;
    }

    os << endl;
}
