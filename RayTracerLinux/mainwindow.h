#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QFileDialog>
#include <QProgressBar>

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
public slots:
  void updtateProgress(int value);

signals:
  void valueChanged(int newValeur);

private:
  int last_progress_value;
  int progress_value;
  QPushButton *select_button;
  QPushButton *calcul_button;
  QProgressBar *progress;
};
#endif // MAINWINDOW_H
