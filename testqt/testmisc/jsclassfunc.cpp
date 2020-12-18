#include "jsclassfunc.h"
#include <QDebug>

LJsClass::LJsClass(QObject* _parent ) : QObject(_parent)
{
}

LJsClass::~LJsClass()
{
}

void LJsClass::debug(const QString& _str)
{
  qDebug() << _str;
}
