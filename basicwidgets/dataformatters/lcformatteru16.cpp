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
#include "lcformatteru16.h"
#include <limits>
#include <math.h>

//==============================================================================LCQDataFormatterUInt16
LCFormatterU16::
LCFormatterU16(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCFormatterIntBase( 
      _fieldWidth,
      _fillChar,
      _base)
{
  mValidator.setRange(    std::numeric_limits<quint16>::min(), 
      std::numeric_limits<quint16>::max());
}

//------------------------------------------------------------------------------toString
QString LCFormatterU16::toString(const QByteArray& _data)
{
  if(_data.size() < 2)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( ((quint16*)_data.constData())[0], 
      mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------normalize
QString LCFormatterU16::normalize(const QString& _str)
{
  bool    ok  = false;
  qint16  val = _str.toUShort(&ok);
  if(!ok) return QString();
  return QString("%1").arg( val, mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterU16::toBytes(const QString& _str)
{
  bool ok = false;
  quint16 r = _str.toUShort(&ok, mBase);
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 2);
}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterU16::validator()
{
  return &mValidator;
}

