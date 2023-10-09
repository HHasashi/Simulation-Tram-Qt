#include "Arret.h"

Arret::Arret (const QString& nom, const double& x, const double& y, const int& temps):
    d_nom{nom}, position{x, y}, d_distance{0.0}, tempsArret{temps}, sens{true}, d_suiv{nullptr}
{}

Arret::Arret():
    d_nom{""}, position{0, 0}, d_distance{0.0}, d_suiv{nullptr}
{}

double Arret::x() const
{
    return position.x;
}

double Arret::y() const
{
    return position.y;
}

double Arret::xSuivant() const
{
    return d_suiv->x();
}

double Arret::ySuivant() const
{
    return d_suiv->y();
}

double Arret::distanceSuivant() const
{
    double x = d_suiv->x() - position.x;
    double y = d_suiv->y() - position.y;
    return sqrt(x*x + y*y);
}
