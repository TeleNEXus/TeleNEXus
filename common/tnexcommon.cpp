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

#include "tnexcommon.h"
#include <QRegularExpression>

namespace tnexcommon
{

static const QRegularExpression __slAttributeCleanExpr(QStringLiteral("(\\s+)|(\\r+)|(\\n+)"));

void setMultipleAttributes(
    const QMap<QString, std::function<void(const QString& _val)>>& _assignActions,
    const QString& _attributes,
    const QString _attrSeparator, 
    const QString _attrEqSign)
{
  QString attrs = _attributes;

  attrs.remove(__slAttributeCleanExpr);
  attrs.remove(QRegularExpression(QString("^%1{1,}|%1{1,}$").arg(_attrSeparator)));

  auto attr_list = attrs.split(_attrSeparator);

  for(auto lit = attr_list.begin(); lit != attr_list.end(); lit++)
  {
    auto record_list = (*lit).split(_attrEqSign);
    if(record_list.size() != 2) continue;
    auto it = _assignActions.find(record_list[0]);
    if(it == _assignActions.end()) continue;
    (*it)(record_list[1]);
  } 
}

void setMultipleValues(
    const QList<std::function<void(const QString& _val)>>& _assigns,
    const QString& _values,
    const QString _valuesSeparator)
{
  QString values = _values;

  values.remove(__slAttributeCleanExpr);

  values.remove(QRegularExpression(QString("^%1{1,}|%1{1,}$").arg(_valuesSeparator)));

  auto values_list = values.split(_valuesSeparator);

  auto it_value = values_list.begin();
  for(auto it_assign = _assigns.begin(); 
      it_assign != _assigns.end(); 
      it_assign++)
  {
    if(it_value != values_list.end())
    {
      (*it_assign)((*it_value));
    }
    else
    {
      (*it_assign)(QString());
    }
    it_value++;
  }
}
}
