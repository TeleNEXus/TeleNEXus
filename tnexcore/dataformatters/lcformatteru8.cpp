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
#include "lcformatteru8.h"

#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterUint8
LCFormatterU8::LCFormatterU8(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
  mValidator.setRange(
      std::numeric_limits<quint8>::min(), 
      std::numeric_limits<quint8>::max());
}

//------------------------------------------------------------------------------toString
QString LCFormatterU8::toString(const QByteArray& _data)
{
  if(_data.size() < 1)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( ((quint8*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterU8::toBytes(const QString& _str)
{
  bool ok = false;
  quint16 r = ((quint16)_str.toUShort(&ok, mBase));
  if((!ok) || ((r & 0xff00) != 0)) return QByteArray();
  return QByteArray((char*)(&r), 1);
}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterU8::validator()
{
  return &mValidator;
}


