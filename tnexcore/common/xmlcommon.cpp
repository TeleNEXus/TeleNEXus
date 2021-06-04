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

#include "xmlcommon.h"
#include <QRegularExpression>
#include <QDomDocument>
#include <QFile>
#include <QPixmap>
#include <QDebug>

//==============================================================================removeSpaces
static QString removeSpaces(
    const QString& _instr, const QChar& _ignor_border, 
    std::function<void(const QString&)> _err = [](const QString&){})
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

  if(state == ST_IN_STRING) 
  {
    _err(QStringLiteral("String border error"));
    return QString();
  }

  return out.replace(QString("\\%1").arg(_ignor_border), _ignor_border);
};

//==============================================================================
static const QRegularExpression __slRemoveEndLine("(\\r+)|(\\n+)");

//==============================================================================
namespace xmlcommon
{

//------------------------------------------------------------------------------parseAction
SAction parseAction( const QString& _actionString, 
    std::function<void(const QString& _error)> _err)
{

  //----------------------------------------------------------ret_ok[]
  auto ret_ok = [](const QString& _func, const QStringList& _params)
  {
    /* if(!_func.isNull()) */
    /* { */
      return SAction{_func, _params};
    /* } */
    /* return SAction{}; */
  };

  //----------------------------------------------------------ret_wrong[]
  auto ret_wrong = [&_err](const QString& _errStr)
  {
    _err(_errStr);
    return SAction{};
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

  /* if(func.isNull()) */
  /* { */
  /*   return ret_wrong("No function name"); */
  /* } */

  QString params_string = _actionString.right(_actionString.size() - func.size());


  QString error;

  params_string = removeSpaces(
      params_string, 
      QChar('\''), 
      [&error](const QString& _str)
      {
        error = _str;
      });

  if(!error.isNull()) 
  {
    return ret_wrong(error);
  }

  bool flag = false;
  auto params = get_params(params_string, &flag);

  if(!flag) return ret_wrong(QString("Close bracket error"));
  return ret_ok(func, params);
}

//------------------------------------------------------------------------------parseValues
QStringList parseValues(
    const QString& _values, 
    const QChar& _separator,
    std::function<void(const QString& _error)> _err)
{
  QString error;
  QString values = removeSpaces(_values, '\'', 
     [&error](const QString& _str)
     {
       error = _str;
     });

  if(!error.isNull()) 
  {
    _err(error);
    return QStringList();
  }
  values.remove(__slRemoveEndLine);
  values.remove(QRegularExpression(QString("^%1{1,}|%1{1,}$").arg(_separator)));
  auto values_list = values.split(_separator);
  return values_list;
}

//------------------------------------------------------------------------------parseAttributes
QMap<QString, QString> parseAttributes(
    const QString& _attributes, 
    const QChar& _separator, 
    const QChar& _equal,
    std::function<void(const QString& _error)> _err)
{
  using TRetMap = QMap<QString, QString>;
  QString error;
  QString attrs = removeSpaces(_attributes, '\'', 
      [&error](const QString& _str)
      {
        error = _str;
      });

  if(!error.isNull())
  {
    _err(error);
    return TRetMap();
  }

  //TODO: change parse algorithm!!!!!!
  
  attrs.remove(__slRemoveEndLine);
  attrs.remove(QRegularExpression(QString("^%1{1,}|%1{1,}$").arg(_separator)));
  auto attributes = attrs.split(
      QRegularExpression(QString("(?<!\\\\)(%1)").arg(_separator)));

  TRetMap ret_map;

  for(auto ait = attributes.begin(); ait != attributes.end(); ait++)
  {
    auto record_list = (*ait).split(
        QRegularExpression(QString("(?<!\\\\)(%1)").arg(_equal)));

    if(record_list.size() == 0) continue;

    auto it = record_list.begin();
    QString name;
    QString value;
    if(it != record_list.end()) name = (*it);
    it++;
    if(it != record_list.end()) value = (*it);
    value.remove(QRegularExpression(QStringLiteral("^'|'$")));
    value.replace(QRegularExpression(QString("(?<!\\\\)(\\\\%1)").arg(_separator)), _separator);
    value.replace(QRegularExpression(QString("(?<!\\\\)(\\\\%1)").arg(_equal)), _equal);
    value.replace(QStringLiteral("\\\\"), QStringLiteral("\\"));
    ret_map.insert(name, value);
  } 
  return ret_map;
}

//------------------------------------------------------------------------------parseDataSpecification
SDataSpecification parseDataSpecification(const QString _dataSpec,
    std::function<void(const QString& _error)> _err)
{
  QString error;
  auto spec_list = parseValues(_dataSpec, QChar(':'), [&error](const QString& _str)
      {
        error = _str;
      });
  if(!error.isNull())
  {
    _err(error);
    return SDataSpecification{};
  }
  QString sourceId;
  QString dataId;
  QString formatId;
  auto it = spec_list.begin();
  if(it != spec_list.end()) { sourceId = (*it); it++; }
  if(it != spec_list.end()) { dataId   = (*it); it++; }
  if(it != spec_list.end()) { formatId = (*it); }
  return SDataSpecification{sourceId, dataId, formatId};
}

//------------------------------------------------------------------------------
QDomDocument loadDomDocument (const QString& _fileName)
{
  QFile file(_fileName);

  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;

  if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
  {
    qDebug() << 
      "Parse file "       << file.fileName() << 
      " error at line:"   << errorLine <<
      " column:"          << errorColumn << 
      " msg: "            << errorStr;
  }
  return domDoc;
}


} /* namespace xmlcommon */

