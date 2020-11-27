#include "lcstringdataformatteru16.h"
#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterUInt16
LCStringDataFormatterU16::
LCStringDataFormatterU16(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCStringDataFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
  mValidator.setRange(    std::numeric_limits<quint16>::min(), 
      std::numeric_limits<quint16>::max());
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterU16::toString(const QByteArray& _data)
{
  if(_data.size() < 2)
  {
    return wrongStateString();
  }
  return QString("%1").arg( ((quint16*)_data.constData())[0], 
      mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCStringDataFormatterU16::normalize(const QString& _str)
{
  bool    ok  = false;
  qint16  val = _str.toUShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterU16::toBytes(const QString& _str)
{
  bool ok = false;
  quint16 r = _str.toUShort(&ok, mBase);
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 2);
}


//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterU16::validator()
{
  return &mValidator;
}

