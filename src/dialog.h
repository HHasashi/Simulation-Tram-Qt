#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QTimer>
#include <QFile>
#include <QTime>

#include "Ligne.h"
#include "Arret.h"
#include "ReseauTram.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(const QString& Info, QWidget *parent = nullptr);
    int getNbLignes ();
    ~Dialog();

    Ligne** reseauDeLignes;

    ReseauTram** reseauDeTrams;

private:
    Ui::Dialog *ui;

    int d_nbLignes;

    int d_temps;

    QGraphicsScene* scene;
    QTimer* timer;

private slots :
    void arret();
};
#endif
