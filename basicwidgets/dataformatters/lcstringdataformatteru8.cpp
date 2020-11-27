#include "lcstringdataformatteru8.h"

#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterUint8
LCStringDataFormatterU8::LCStringDataFormatterU8(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCStringDataFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
  mValidator.setRange(
      std::numeric_limits<quint8>::min(), 
      std::numeric_limits<quint8>::max());
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterU8::toString(const QByteArray& _data)
{
  if(_data.size() < 1)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( ((quint8*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCStringDataFormatterU8::normalize(const QString& _str)
{
  bool    ok  = false;
  qint16  val = _str.toUShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterU8::toBytes(const QString& _str)
{
  bool ok = false;
  quint16 r = ((quint16)_str.toUShort(&ok, mBase));
  if((!ok) || ((r & 0xff00) != 0)) return QByteArray();
  return QByteArray((char*)(&r), 1);
}


//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterU8::validator()
{
  return &mValidator;
}


