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
    QString value;
    auto record_list = (*lit).split(_attrEqSign);
    if(record_list.size() == 2) 
    {
      value = record_list[1];
    }
    auto it = _assignActions.find(record_list[0]);
    if(it == _assignActions.end()) continue;
    (*it)(value);
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

//------------------------------------------------------------------------------
static struct SAction
{
  QString name;
  QStringList params;
};

static SAction parseAction(const QString& _actionString)
{
  SAction action;

  QString act = _actionString;
  act.remove(" ");
  auto al = act.split("(");
  auto it = al.begin();
  if(it == al.end()) return action;
  action.name = (*it);

  if(al.size() > 1)
  {
    it++;
    (*it).remove(")");
    action.params = (*it).split(",");
  }

  return action;
}

void performParamAction(
    const QString& _actionString, 
    const QMap<QString, std::function<void(const QStringList&)>>& _functors)
{
  //TODO: add debug message group
  QString act = _actionString;
  auto al = act.split("(");

  auto it = al.begin();

  auto perform = [&_functors](const QString& _actionString, const QStringList& _params)
  {
    auto actit = _functors.find(_actionString);
    if(actit == _functors.end()) return;
    actit.value()(_params);
  };

  if(it == al.end()) return;
  auto action_name = (*it).remove(" ");
  if(al.size() > 1)
  {
    it++;
    (*it).remove(" ");
    if(!(*it).contains(QRegularExpression("\\)$")))return;
    (*it).remove(")");
    auto params = (*it).split(",");
    perform(action_name, params);
  }
  else
  {
    perform(action_name, QStringList());
  }
}

void performParamAction(
    const QString& _actionString, 
    const std::function<void(const QStringList&)>& _functor)
{
  //TODO: add debug message group
  QString act = _actionString;
  auto al = act.split("(");

  auto it = al.begin();

  auto perform = [&_functors](const QString& _actionString, const QStringList& _params)
  {
    auto actit = _functors.find(_actionString);
    if(actit == _functors.end()) return;
    actit.value()(_params);
  };

  if(it == al.end()) return;
  auto action_name = (*it).remove(" ");
  if(al.size() > 1)
  {
    it++;
    (*it).remove(" ");
    if(!(*it).contains(QRegularExpression("\\)$")))return;
    (*it).remove(")");
    auto params = (*it).split(",");
    perform(action_name, params);
  }
  else
  {
    perform(action_name, QStringList());
  }
}

} /* namespace tnexcommon */

