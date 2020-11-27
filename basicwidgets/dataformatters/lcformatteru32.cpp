#include "lcformatteru32.h"

//==============================================================================LCQDataStringFormatterUint32
LCFormatterU32::
LCFormatterU32( 
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
}

//------------------------------------------------------------------------------toString
QString LCFormatterU32::toString(const QByteArray& _data)
{
  if(_data.size() < 4)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( ((quint32*)_data.constData())[0], 
      mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCFormatterU32::normalize(const QString& _str)
{
  bool    ok  = false;
  qint16  val = _str.toUShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterU32::toBytes(const QString& _str)
{
  bool ok = false;
  quint32 r = ((quint32)_str.toUInt(&ok, mBase));
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 4);
}

//------------------------------------------------------------------------------validator
QValidator* LCFormatterU32::validator()
{
  return &mValidator;
}

