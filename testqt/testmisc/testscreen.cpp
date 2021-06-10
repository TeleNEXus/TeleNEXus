#include <QApplication>
#include <QtWidgets>
#include <cmath>
#include <ostream>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <QIntValidator>
#include <qglobal.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <QScreen>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QWidget* w = new QWidget();
  auto screens = QApplication::screens();
  qDebug() << "Screens coutn = " << screens.size();
  qDebug() << " screen 0 geometry = " << screens[0]->geometry();
  qDebug() << " screen 1 geometry = " << screens[1]->geometry();
  QPoint pos(150, 150);
  w->move(screens[0]->geometry().topLeft() + pos);
  w->show();
  app.exec();
}


