/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
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

//------------------------------------------------------------------------------fitting
QString LCFormatterBool::fitting(const QString& _str)
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


