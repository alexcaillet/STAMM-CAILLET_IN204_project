#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>

#include "../moteur_rendu.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
private slots:
  void open();
  void calcul();
private:
  QPushButton *select_button;
  QPushButton *calcul_button;
};
#endif // MAINWINDOW_H

