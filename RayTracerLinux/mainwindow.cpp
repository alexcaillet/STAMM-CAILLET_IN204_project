#include "mainwindow.h"

QString fileName;


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  last_progress_value = 0;
  progress_value = 0;

  // Create the button, make "this" the parent
  select_button = new QPushButton("Choisis ta scène .xml", this);
  calcul_button = new QPushButton("Rendu", this);
  // set size and location of the button
  select_button->setGeometry(QRect(QPoint(0, 0), QSize(200, 50)));
  calcul_button->setGeometry(QRect(QPoint(0, 55), QSize(200, 50)));
  // Connect button signal to appropriate slot
  connect(select_button, &QPushButton::released, this, &MainWindow::open);
  connect(calcul_button, &QPushButton::released, this, &MainWindow::calcul);

  progress = new QProgressBar(this);
  progress->setGeometry(QRect(QPoint(0, 110), QSize(200, 50)));
  progress->setRange(0, 100);
  progress->setValue(progress_value);
  QObject::connect(progress, SIGNAL(valueChanged(progress_value)), this, SLOT(MainWindow::updtateProgress(progress_value)));
}

void MainWindow::updtateProgress(int value){
    if (value != progress->value()) {
        progress->setValue(value);
        emit valueChanged(value);
        }
}

void MainWindow::open(){
  fileName = QFileDialog::getOpenFileName(this);
  std::cerr << fileName.toStdString() << std::endl;
  select_button->setText("Scène choisie");
  progress_value = 0;
  progress->reset();
}

void MainWindow::calcul(){
    std::string str = fileName.toStdString();
    const char* filename = str.c_str();
    moteur_rendu* moteur = new moteur_rendu();
    moteur->calcul(filename, &progress_value);
    progress->setValue(progress_value);
    delete moteur;
}

