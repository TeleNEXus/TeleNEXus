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

//==============================================================================removeSpaces
static QString removeSpaces(
    const QString& _instr, const QChar& _ignor_border, bool* _ef = nullptr)
{
  enum
  {
    ST_OUT_STRING,
    ST_IN_STRING
  } state = ST_OUT_STRING;

  const QChar space(' ');
  const QChar slash('\\');

  QChar prch;

  QString out;

  auto ret = [&_ef](const QString& _out, bool _flag)
  {
    if(_ef) *_ef = _flag;
    return _out;
  };

  for(int i = 0; i < _instr.size(); i++)
  {
    QChar ch = _instr[i];
    switch(state)
    {
    case ST_OUT_STRING:
      if(ch == _ignor_border) 
      {
        state = ST_IN_STRING;
        out.append(ch);
      }
      else
      {
        if(ch != space) out.append(ch);
      }
      break;

    case ST_IN_STRING:
      if((ch == _ignor_border)&&(prch != slash))
      {
        state = ST_OUT_STRING;
      }
      out.append(ch);
      break;
    }
    prch = ch;
  };

  if(state == ST_IN_STRING) return ret(QStringLiteral(), false);
  return ret(out.replace(QString("\\%1").arg(_ignor_border), _ignor_border), true);
};


//==============================================================================
namespace tnexcommon
{

//------------------------------------------------------------------------------parseAction
QStringList parseAction( const QString& _actionString, 
    std::function<void(const QString& _error)> _err)
{

  //----------------------------------------------------------ret_ok[]
  auto ret_ok = [](const QString& _func, const QStringList& _params)
  {
    QStringList ret;
    if(!_func.isNull())
    {
      ret << _func << _params;
    }
    return ret;
  };

  //----------------------------------------------------------ret_wrong[]
  auto ret_wrong = [&_err](const QString& _errStr)
  {
    _err(_errStr);
    return QStringList();
  };

  //----------------------------------------------------------get_func[]
  auto get_func = [](const QString& _in)
  {
    QString out;
    for(int i = 0; i < _in.size(); i++)
    {
      if(_in.at(i) == QStringLiteral("(")) break;
      out.append(_in.at(i));
    }
    return out;
  };

  //----------------------------------------------------------get_params[]
  auto get_params = [](const QString& _in, bool* _flag)
  {
    int bc = 0;
    QString param;
    QStringList params;

    for(int i=0; i < _in.size(); i++)
    {
      QChar ch = _in.at(i);
      if(ch == QStringLiteral("("))
      {
        bc++;
        if(bc > 1)
        {
          param.append(ch);
        }
        continue;
      }

      if(ch == QStringLiteral(")"))
      {
        bc--;
        if(bc == 0)
        {
          if(!param.isNull())
          {
            params << param.remove(
                QRegularExpression("^'|'$"));
          }
          break;
        }
        else
        {
          param.append(ch);
          continue;
        }
      }

      if(ch == QStringLiteral(","))
      {
        if(bc > 1)
        {
          param.append(ch);
        }
        else
        {
          if(!param.isNull()) 
            params << param.remove(
                QRegularExpression("^'|'$"));
          param.clear();
        }
      }
      else
      {
        param.append(ch);
      }
    }

    if((bc != 0)) 
    {
      *_flag = false;
      return QStringList();
    }

    *_flag = true;
    return params;
  };

  QString func = get_func(_actionString);

  if(func.isNull())
  {
    return ret_wrong("No function name");
  }

  QString params_string = _actionString.right(_actionString.size() - func.size());

  bool flag = false;

  params_string = removeSpaces(params_string, QChar('\''), &flag);

  if(!flag) 
  {
    return ret_wrong(QString("String border error"));
  }

  auto params = get_params(params_string, &flag);

  if(!flag) return ret_wrong(QString("Close bracket error"));
  return ret_ok(func, params);
}

//------------------------------------------------------------------------------
static const QRegularExpression __slRemoveEndLine("(\\r+)|(\\n+)");

//------------------------------------------------------------------------------setMultipleAttributes
void setMultipleAttributes(
    const QMap<QString, std::function<void(const QString& _val)>>& _assigns,
    const QString& _attributes,
    const QString _attrSeparator, 
    const QString _attrEqSign)
{

  QString attrs = removeSpaces(_attributes, '\'');
  attrs.remove(__slRemoveEndLine);
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
    auto it = _assigns.find(record_list[0]);
    if(it == _assigns.end()) continue;
    (*it)(value);
  } 
}

//------------------------------------------------------------------------------setMultipleValues
void setMultipleValues(
    const QList<std::function<void(const QString& _val)>>& _assigns,
    const QString& _values,
    const QString _valuesSeparator)
{
  QString values = removeSpaces(_values, '\'');
  values.remove(__slRemoveEndLine);
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

void performParamAction(
    const QString& _actionString, 
    const QMap<QString, std::function<void(const QStringList&)>>& _functors)
{
  //TODO: debug message
  QString parse_error;
  auto pef = [&parse_error](const QString& _err)
  {
    parse_error = _err;
  };

  auto parse_res = parseAction(_actionString, pef);
  if(!parse_error.isNull()) return;

  auto action_name = parse_res.first();
  parse_res.removeFirst();

  auto actit = _functors.find(action_name);
  if(actit == _functors.end()) return;
  actit.value()(parse_res);
}

} /* namespace tnexcommon */

