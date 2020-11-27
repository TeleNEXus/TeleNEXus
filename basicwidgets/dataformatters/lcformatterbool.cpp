#include "lcformatterbool.h"

LCFormatterBool::LCFormatterBool(const QString& _strFalse,
    const QString& _strTrue) :  mStrFalse(_strFalse),
  mStrTrue(_strTrue)
{

}

//------------------------------------------------------------------------------
QString LCFormatterBool::toString(const QByteArray& _data)
{
  if(_data.size() == 0)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }

  quint8 byte = 0;
  for(int i = 0; i < _data.size(); i++)
  {
    byte |= _data[i];
  }
  if(byte != 0) return mStrTrue;
  return mStrFalse;
}

//------------------------------------------------------------------------------normalize
QString LCFormatterBool::normalize(const QString& _str)
{
  //Удаление незначащих нулей.
  //Удаление сепараторов.
  return _str;
}

//------------------------------------------------------------------------------
QByteArray LCFormatterBool::toBytes(const QString& _str)
{
  if(_str == mStrTrue) return QByteArray(1,1);
  else if(_str == mStrFalse) return QByteArray(1,0);
  return QByteArray();
}


