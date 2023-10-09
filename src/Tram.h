#ifndef TRAM_H
#define TRAM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <cmath>
#include <QPoint>
#include <QTime>
#include <QSize>

#include "Arret.h"
#include "Ligne.h"

class Tram : public QGraphicsItem
{
    friend class ReseauTram;
public:

    Tram(Ligne* l ,Arret* a, double v);

    QRectF boundingRect() const override; // sert à faire le carré représentatif du tram

    // paint sert à mettre le tram en place dans le plan
    void paint(QPainter *peintre, const QStyleOptionGraphicsItem *option = nullptr, QWidget *widget = nullptr) override;

    double x() const; // renvoie la position x (position.x)
    double xSuiv() const; // renvoie la position x du tram suivant (d_suiv->position.x)
    double y() const; // renvoie la position y (position.y)
    double ySuiv() const; // renvoie la position y du tram suivant (d_suiv->position.y)

    double Time() const;
    double distanceTramSuivant() const; // formule basique de la distance

    bool tramSuivantProche() const; // vérifie si le tram suivant est proche à l'aide de sa position
                                    // et de la position de son arrêt de destination

    bool arretSuivantOccupee() const; // vérifie si le tram suivant n'est pas stationné à un arrêt
    bool bloquer() const; // bloque le tram en milieur de rame si nécessaire

protected:
    void advance(int phase) override; // dicte l'avancement du tram

private :
    double d_vitesse;
    double d_compteur;

    double ID;

    int d_x;
    int d_y;

    QPointF position;
    QSize taille;
    Arret* comminus; // inverse de terminus
    QTime Temps; // sert de compteur pour le temps à l'arrêt / en circulation

    Ligne* principale;
    Tram* d_suiv;
};

#endif
