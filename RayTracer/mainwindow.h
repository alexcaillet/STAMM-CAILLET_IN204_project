#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "moteur_rendu.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  void open();
  //Lance le calcul de l'image
  void calcul();
private slots:
private:
  QPushButton *select_button;
  QPushButton *calcul_button;
};
#endif // MAINWINDOW_H
