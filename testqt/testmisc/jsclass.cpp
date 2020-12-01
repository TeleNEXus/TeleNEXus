#include "jsclass.h"
#include <QDebug>

LJsClass::LJsClass(QObject* _parent ) : QObject(_parent)
{
  mCh = new char[10];
}

LJsClass::~LJsClass()
{
  delete[] mCh;
}

QString LJsClass::getJsClassName()
{
  return QString("LJsClass");
}

QString LJsClass::getFullFileName(const QString& _file)
{
  return QString("/home/serg/%1").arg(_file);
}

QVariantList LJsClass::getArray()
{
  QVariantList list;
  list << 0;
  list << 1;
  list << 2;
  list << 3;
  /* v[0] = 0; */
  /* v[1] = 1; */
  /* v[2] = 2; */
  return list;
  /* return mCh; */
}

QVariantList LJsClass::getArray(const QVariantList _list)
{
  QVariantList list;

  for(int i = 0; i < _list.size(); i++)
  {

    QVariant v = _list[i];
    qDebug() << "Variant type = " << v.type();
    if(v.type() == QVariant::Type::Int)
    {
      list << (v.toInt() | 0x000000f0);
    }

  }

  return list;
}

void LJsClass::print(const QString& _str)
{
  qDebug() << _str;
}
