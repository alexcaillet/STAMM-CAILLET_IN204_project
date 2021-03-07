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

QString fileName;

void MainWindow::open(){
  fileName = QFileDialog::getOpenFileName(this);
  std::cerr << fileName.toStdString() << std::endl;
  select_button->setText("Scène choisie");
}

void MainWindow::calcul(){
    std::string str = fileName.toStdString();
    const char* filename = str.c_str();
    moteur_rendu* moteur = new moteur_rendu();
    moteur->calcul(filename);
    delete moteur;
}

