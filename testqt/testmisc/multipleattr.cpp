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

int main(int argc, char** argv)
{
  QString separator = ";";
  QString eqsign = "=";
  QString attributes = ";;;; attr1 = val1; attr2 = val2; attr3 = val3 ;;;;";
  /* QString attributes = ";;;; attr1 = ;val1; attr2; = val2; attr3 = val3 ;;;;"; */
  /* QString attributes = "     attr1=val1 attr2=val2 attr3=val3     "; */
  qDebug() << "attributes = " << attributes;
  if(separator != QStringLiteral(" ")) {attributes.remove(QString(" "));}
  attributes.remove(QRegularExpression(QString("^%1{1,}|%1{1,}$").arg(separator)));
  qDebug() << "attributes = " << attributes;

  auto attr_list = attributes.split(separator);
  qDebug() << "attributes list = " << attr_list;

  for(auto lit = attr_list.begin(); lit != attr_list.end(); lit++)
  {
    auto record_list = (*lit).split(eqsign);
    if(record_list.size() != 2) continue;
    qDebug() << QString(" %1 equal %2").arg(record_list[0]).arg(record_list[1]);
  } 

}
