#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  // Create the button, make "this" the parent
  select_button = new QPushButton("Choisi ta scène .xml", this);
  // set size and location of the button
  select_button->setGeometry(QRect(QPoint(0, 0), QSize(200, 50)));

  // Connect button signal to appropriate slot
  connect(select_button, &QPushButton::released, this, &MainWindow::open);
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
}

void MainWindow::calcul(){

}
