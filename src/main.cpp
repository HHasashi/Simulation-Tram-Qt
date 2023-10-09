#include "dialog.h"
#include <fstream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w{"Info.txt"};
    w.show();

    for (int i=0; i < w.getNbLignes(); i++)
    {
        std::ofstream lt ("./Program output/Affichage Ligne " + std::to_string(w.reseauDeLignes[i]->getNumLigne()) + ".txt");
        std::ofstream at ("./Program output/Affichage Trams " + std::to_string(w.reseauDeLignes[i]->getNumLigne()) + ".txt");
        w.reseauDeLignes[i]->afficherListeArret(lt);
        w.reseauDeTrams[i]->afficherListeTram(at);
    }

    return a.exec();
}
