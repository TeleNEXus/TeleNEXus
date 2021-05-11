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
#include "lcformatterkey.h"
#include <QDebug>
#include <functional>
#include <QRegularExpression>
#include <QMap>
#include <qnamespace.h>

struct SKey
{
  quint32 code;
  quint32 modif;
};

QMap<QString, SKey> __slControlKeyMap;

static void initControlMap()
{
  static bool first_flag = true;
  if(!first_flag) return;
  first_flag = false;
  __slControlKeyMap.insert("[Enter]",     {Qt::Key::Key_Return,     Qt::KeyboardModifier::NoModifier});
  __slControlKeyMap.insert("[Backspace]", {Qt::Key::Key_Backspace,  Qt::KeyboardModifier::NoModifier});
  __slControlKeyMap.insert("[Clear]",     {Qt::Key::Key_Clear,      Qt::KeyboardModifier::NoModifier});
}

//==============================================================================
LCFormatterKey::LCFormatterKey()
{
  initControlMap();
}

//------------------------------------------------------------------------------
LCFormatterKey::~LCFormatterKey()
{
}

//------------------------------------------------------------------------------toString
QString LCFormatterKey::toString(const QByteArray& _data)
{
  return _data.toHex();
}

//------------------------------------------------------------------------------fitting
QString LCFormatterKey::fitting(const QString& _instr)
{
  return _instr;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterKey::toBytes(const QString& _str)
{
  QByteArray ret;
  auto control_key = [&ret](QString _instr)
  {
    if(!_instr.contains(QRegularExpression("^\\[[a-zA-Z]{1,}\\]$"))) return false;
    auto it = __slControlKeyMap.find(_instr);
    if(it == __slControlKeyMap.end()) return false;
    ret.append(reinterpret_cast<char*>(&it.value().code), sizeof(quint32));
    ret.append(reinterpret_cast<char*>(&it.value().modif), sizeof(quint32));
    return true;
  };

  if(control_key(_str)) return ret;

  return ret;
}

