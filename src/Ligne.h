#ifndef LIGNE_H_INCLUDED
#define LIGNE_H_INCLUDED

#include <QColor>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLine>
#include <QGraphicsScene>
#include <QDebug>
#include <iostream>
using std::ostream;
using std::endl;
#include "Arret.h"

class Ligne{
    friend class ReseauTram;
    friend class Tram;
    friend class Dialog;
public:
    Ligne();
    Ligne(const std::string& TextLigne);
    ~Ligne();

    void draw(QGraphicsScene* scene) const; // dessine la ligne de tram

    int getNumLigne();
    void insererArret(const Arret &a);
    void supprimerArret(const Arret &a);
    bool chercherArret(const Arret &a);
    void afficherListeArret(ostream &os) const;

private:
    Arret* d_t;
    Arret* d_q;
    int d_numLigne;
    unsigned int d_nbArrets;
    QColor couleur;
};

#endif
