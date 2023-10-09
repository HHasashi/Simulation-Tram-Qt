#include "dialog.h"
#include "ui_dialog.h"
#include "Tram.h"
#include "Ligne.h"
#include "Arret.h"
#include "ReseauTram.h"
#include <QMessageBox>
#include <fstream>

using std::ifstream;
using std::string;


Dialog::Dialog(const QString& Info, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ifstream Fin("./Files/" + Info.toStdString());

    string chara;

    std::ofstream lelog("./Logs/DIA-LOGS.TXT");

    if (!Fin)
    {   // informe l'utilisateur du problème
        lelog << "Impossible d'ouvrir les fichiers (erreur sur le fichier Info).";
        QMessageBox msgBox;
        msgBox.setText("Les documents d'infos sont introuvables, essayez de les placer dans le dossier \"Build/Files\".");
        msgBox.exec();
        return;
    }

    ui->setupUi(this);

    scene = new QGraphicsScene{this};

    lelog << "J'ai survécu la lecture." << endl;

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // pour avoir une image lisse
                                                             // Anti-Aliasing = Anti-crénelage
    ui->graphicsView->setFixedSize(800,600);
    ui->graphicsView->resize(800,600);
    ui->graphicsView->scale(0.75, 0.75);

    Fin >> chara >> chara >> chara >> chara >> d_nbLignes;
    Fin >> chara >> chara >> chara >> d_temps;

    d_temps *= 1000; // le temps étant en secondes dans le fichier, on le converti en millisecondes

    reseauDeLignes = new Ligne* [d_nbLignes];
    reseauDeTrams = new ReseauTram* [d_nbLignes];

    QString* TabDeNomsLignes = new QString[d_nbLignes];
    QString* TabDeNomsTrams = new QString[d_nbLignes];
    QString iLigne;

    for (int i = 0 ; i < d_nbLignes; i++)
    {
        iLigne.setNum(i+1);

        TabDeNomsLignes[i] = "Ligne " + iLigne + ".txt"; // sert à automatiquement charger les fichiers de lignes
        TabDeNomsTrams[i] = "Trams " + iLigne + ".txt"; // sert à automatiquement charger les fichiers de trams

        lelog << TabDeNomsLignes[i].toStdString() << " " << TabDeNomsTrams[i].toStdString() << endl;

        reseauDeLignes[i] = new Ligne{TabDeNomsLignes[i].toStdString()};
        reseauDeTrams[i] = new ReseauTram{reseauDeLignes[i], TabDeNomsTrams[i].toStdString(), scene};

        reseauDeLignes[i]->draw(scene);
    }

    delete [] TabDeNomsLignes;
    delete [] TabDeNomsTrams;

    timer = new QTimer{this};
    QTimer::singleShot(d_temps, this, SLOT(arret()));
    connect (timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(1);
}

Dialog::~Dialog()
{
    for (int i = 0 ; i < d_nbLignes; i++)
    {
        delete reseauDeLignes[i];
    }

    delete [] reseauDeLignes;

    for(int i =0 ; i < d_nbLignes; i++)
    {
        delete reseauDeTrams[i];
    }

    delete [] reseauDeTrams;

    delete ui;
}

int Dialog::getNbLignes()
{
    return d_nbLignes;
}

void Dialog::arret()
{
    this->ui->graphicsView->close();
    this->close();
    delete this->ui->graphicsView;
    delete this;
    return;
}
