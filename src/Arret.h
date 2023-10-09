#ifndef ARRET_H_INCLUDED
#define ARRET_H_INCLUDED

#include <QString>
#include <QGraphicsScene>
#include <cmath>

struct pos
{
    double x;
    double y;
};

class Arret
{
    friend class Ligne;
    friend class ReseauTram;
    friend class Tram;
    friend class Dessin;
    friend class Dialog;
public:
    Arret();
    Arret(const QString& nom, const double& x, const double& y, const int& temps);
    double x() const;
    double y() const;

    double xSuivant() const;
    double ySuivant() const;

    double distanceSuivant() const;

private:
    QString d_nom;
    pos position;
    double d_distance;
    int tempsArret;
    bool sens;
    Arret* d_suiv;
};

#endif
