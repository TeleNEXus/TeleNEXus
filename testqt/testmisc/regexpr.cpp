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

void foo(QString& str)
{
  auto ret = [&str]()
  {
    str = "Hello!!!";
  };
  return ret();
}

int main(int argc, char** argv)
{

  QString regexpr1 = QStringLiteral("'string1'; val1; 'string2 ; string2");


  /* QApplication app(argc, argv); */
  /* return app.exec(); */
}
