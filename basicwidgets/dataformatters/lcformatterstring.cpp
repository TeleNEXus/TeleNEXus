#include "lcformatterstring.h"

//==============================================================================LCQDataFormatterUint8
LCFormatterString::LCFormatterString() 
{
}

//------------------------------------------------------------------------------toString
QString LCFormatterString::toString(const QByteArray& _data)
{
  return QString(_data);
}

//------------------------------------------------------------------------------normalize
QString LCFormatterString::normalize(const QString& _str)
{
  return _str;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterString::toBytes(const QString& _str)
{
  return _str.toLatin1();
}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterString::validator()
{
  return nullptr;
}


