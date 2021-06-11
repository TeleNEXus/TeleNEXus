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
#include "lcformatters16.h"

#include <limits>
#include <math.h>
#include <qglobal.h>

//==============================================================================LCQDataFormatterInt16
LCFormatterS16::LCFormatterS16(   
    int     _fieldWidth,
    QChar   _fillChar,
    int     _base) :
  LCFormatterIntBase( _fieldWidth, _fillChar, _base)
{
  mValidator.setRange(std::numeric_limits<qint16>::min(), 
      std::numeric_limits<qint16>::max());
}

//------------------------------------------------------------------------------toString
QString LCFormatterS16::toString(const QByteArray& _data)
{
  if(_data.size() < 2)
  {
    QString str = "Wrong";
    wrongState(str);
    return str;
  }
  return QString("%1").arg( 
      ((qint16*)_data.constData())[0], mFieldWidth, mBase, mFillChar);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterS16::toBytes(const QString& _str)
{
  bool ok = false;
  qint16 r = _str.toShort(&ok, mBase);
  if(!ok) return QByteArray();
  return QByteArray((char*)(&r), 2);
}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterS16::validator()
{
  return &mValidator;
}


