#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  // Create the button, make "this" the parent
  select_button = new QPushButton("Choisi ta scène .xml", this);
  calcul_button = new QPushButton("Rendu", this);
  // set size and location of the button
  select_button->setGeometry(QRect(QPoint(0, 0), QSize(200, 50)));
  calcul_button->setGeometry(QRect(QPoint(0, 55), QSize(200, 50)));
  // Connect button signal to appropriate slot
  connect(select_button, &QPushButton::released, this, &MainWindow::open);
  connect(calcul_button, &QPushButton::released, this, &MainWindow::calcul);

}

void MainWindow::open(){
  QString fileName = QFileDialog::getOpenFileName(this);
}

void MainWindow::calcul(){
    moteur_rendu moteur = moteur_rendu();
    moteur.calcul();
}
