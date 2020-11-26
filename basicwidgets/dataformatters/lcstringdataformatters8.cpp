#include "lcstringdataformatters8.h"

#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterInt8
LCStringDataFormatterS8::
LCStringDataFormatterS8(  
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCStringDataFormatterIntBase( _fieldWidth,
      _fillChar,
      _base)
{
  mValidator.setRange(    std::numeric_limits<qint8>::min(), 
      std::numeric_limits<qint8>::max());
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterS8::toString(const QByteArray& _data)
{
  if(_data.size() < 1)
  {
    return wrongStateString();
  }
  qint8 r = ((qint8*)_data.constData())[0];
  return QString("%1").arg( r, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalizeString
QString LCStringDataFormatterS8::normalizeString(const QString& _str)
{
  bool    ok  = false;
  qint8   val = _str.toShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterS8::toBytes(const QString& _str)
{
  bool ok = false;
  qint8 r = (qint8)_str.toShort(&ok, mBase);
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 1);
}


//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterS8::validator()
{
  return &mValidator;
}



