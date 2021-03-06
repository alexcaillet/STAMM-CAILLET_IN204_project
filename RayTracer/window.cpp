#include "window.h"

#include <QApplication>
#include <QPushButton>

window::window(QWidget *parent) :
    QWidget(parent)
    {
     // Set size of the window
     setFixedSize(100, 50);
    // Create and position the button
     m_button = new QPushButton("Hello World", this);
     m_button->setGeometry(10, 10, 80, 30);
