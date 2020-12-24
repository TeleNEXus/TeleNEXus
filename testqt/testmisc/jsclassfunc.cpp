#include "jsclassfunc.h"
#include <QDebug>
#include <iostream>

LJsClass::LJsClass(QObject* _parent ) : QObject(_parent)
{
}

LJsClass::~LJsClass()
{
}

void LJsClass::debug(const QString& _str)
{
  qDebug("%s", qPrintable(_str));
}
