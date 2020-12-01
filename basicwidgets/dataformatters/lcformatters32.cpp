﻿#include "lcformatters32.h"

//==============================================================================LCQDataStringFormatterInt32
LCFormatterS32::
LCFormatterS32( 
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCFormatterIntBase( _fieldWidth,
      _fillChar,
      _base)
{
}

//------------------------------------------------------------------------------toString
QString LCFormatterS32::toString(const QByteArray& _data)
{
  if(_data.size() < 4)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( ((qint32*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCFormatterS32::normalize(const QString& _str)
{
  bool    ok = false;
  qint32  val = _str.toInt(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterS32::toBytes(const QString& _str)
{
  bool ok = false;
  qint32 r = ((qint32)_str.toInt(&ok, mBase));
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 4);
}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterS32::validator()
{
  return &mValidator;
}