#ifndef RESEAUTRAM_H
#define RESEAUTRAM_H

#include <QFile>
#include <QColor>
#include <QGraphicsScene>
#include <QTextStream>

#include "Tram.h"
#include "Ligne.h"
#include "Arret.h"

class ReseauTram
{
public:
    ReseauTram(Ligne* lgn,const std::string& TextTram, QGraphicsScene* scene);
    ~ReseauTram();

    void afficherListeTram(ostream &os);
    void insererTram(const Tram &t);
    void supprimerTram(const Tram &t);
    void chercherTram(const Tram &t);

private:
    Tram* t_Tete;
    int unsigned d_nbTram;
};

#endif
