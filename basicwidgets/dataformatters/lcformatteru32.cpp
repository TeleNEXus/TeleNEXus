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

//------------------------------------------------------------------------------fitting
QString LCFormatterU32::fitting(const QString& _str)
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

