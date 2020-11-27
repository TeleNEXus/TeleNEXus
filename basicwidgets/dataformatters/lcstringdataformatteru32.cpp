#include "lcstringdataformatteru32.h"

//==============================================================================LCQDataStringFormatterUint32
LCStringDataFormatterU32::
LCStringDataFormatterU32( 
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCStringDataFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
}

//------------------------------------------------------------------------------toString
QString LCStringDataFormatterU32::toString(const QByteArray& _data)
{
  if(_data.size() < 4)
  {
    return wrongStateString();
  }
  return QString("%1").arg( ((quint32*)_data.constData())[0], 
      mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCStringDataFormatterU32::normalize(const QString& _str)
{
  bool    ok  = false;
  qint16  val = _str.toUShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCStringDataFormatterU32::toBytes(const QString& _str)
{
  bool ok = false;
  quint32 r = ((quint32)_str.toUInt(&ok, mBase));
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------validator
QValidator* LCStringDataFormatterU32::validator()
{
  return &mValidator;
}

