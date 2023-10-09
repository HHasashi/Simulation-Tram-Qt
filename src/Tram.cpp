#include "Tram.h"

Tram::Tram(Ligne* l ,Arret* a, double v) :
    d_vitesse{v}, d_compteur{0}, comminus{a}, Temps{QTime::currentTime()}, principale{l},
    d_suiv{nullptr}
{
    d_x = comminus->x();
    d_y = comminus->y();

    // Positionner le tram sur le plan

    taille = QSize(10, 10);
    position = QPointF(-7, -7);

    setPos(mapFromParent(0, 0));
}

QRectF Tram::boundingRect() const
{
    return QRectF(position , QSize(14, 14));
}

void Tram::paint(QPainter *peintre, const QStyleOptionGraphicsItem *Option, QWidget *widget)
{
    QRectF tram = boundingRect();
    QPen LePen {principale->couleur};
    QBrush brush {Qt::red};

    // déterminer la couleur en fonction
    // du sens de déplacement.
    // Gris foncé -> retour
    // Blanc ------> aller
    if(comminus->sens)
    {
          brush.setColor(Qt::white);
    }

    else
    {
        brush.setColor(Qt::darkGray);
    }
    peintre->setPen(LePen);
    peintre->fillRect(tram, brush);
    peintre->drawRect(tram);

}

void Tram::advance(int phase)
{

    if (!phase) return;

    if (Time()>d_compteur && bloquer() && distanceTramSuivant()<20)
    {
        d_compteur = Time();
        Temps = QTime::currentTime().addSecs(comminus->tempsArret);
    }

    else if (Time()>d_compteur)
    {
        d_compteur = 0;

        double DistanceToArriver = d_vitesse*Time();
        double RATIO = DistanceToArriver / comminus->distanceSuivant();

        if (DistanceToArriver>=0)
        {
            if(RATIO<1) // si il n'est pas à destination
            {   // faire avancer le tram linéairement sur la ligne
                d_x = RATIO*comminus->xSuivant() + (1-RATIO) * comminus->x();
                d_y = RATIO*comminus->ySuivant() + (1-RATIO) * comminus->y();
            }
            else
            {   // Le positionner sur l'arrêt de destination
                d_x = comminus->xSuivant();
                d_y = comminus->ySuivant();

                comminus = comminus->d_suiv;

                Temps = QTime::currentTime().addSecs(comminus->tempsArret);
            }
        }// mise à jour du tram sur le plan
        setPos(d_x, d_y);
    }
}

double Tram::x() const
{
    return d_x;
}

double Tram::xSuiv() const
{
    return d_suiv->x();
}

double Tram::y() const
{
    return d_y;
}

double Tram::ySuiv() const
{
    return d_suiv->y();
}

bool Tram::tramSuivantProche() const
{
    return d_suiv->comminus->xSuivant()==d_suiv->xSuiv() && d_suiv->comminus->ySuivant()==d_suiv->ySuiv();
}

bool Tram::arretSuivantOccupee() const
{
    return xSuiv()==comminus->xSuivant() && ySuiv()==comminus->ySuivant();
}

bool Tram::bloquer() const
{
    return tramSuivantProche() || arretSuivantOccupee();
}

double Tram::Time() const
{
    double tmp = Temps.msecsTo(QTime::currentTime());
    tmp = tmp/1000;
    return tmp;
}

double Tram::distanceTramSuivant() const
{
    double a=(x()-xSuiv()) , b=(y()-ySuiv());
    double dist = sqrt((a*a)+(b*b));
    return dist;
}
