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
#ifndef TNEXCOMMON_H_
#define TNEXCOMMON_H_

#include <QString>
#include <QMap>
#include <functional>


namespace tnexcommon
{

struct SAction
{
  QString name;
  QStringList parameters;
};

struct SDataSpecification
{
  QString sourceId;
  QString dataId;
  QString formatterId;
};

/*
 * ENG>
 * Parses the string describing the action and returns a QStringList, 
 * where the first element is the name of the 
 * action and the next elements are its parameters.
 *  --------------------------------
 *  Parameters:
 * _actionString  - string whith format "action_name(p1,p2,...,pn);
 * _err           - error return functor;
 */
SAction parseAction( 
    const QString& _actionString, 
    std::function<void(const QString& _error)> _err = [](const QString&){});

QStringList parseValues(
    const QString& _values, 
    const QChar& _separator = QChar(';'),
    std::function<void(const QString& _error)> _err = [](const QString&){});

QMap<QString, QString> parseAttributes(
    const QString& _attributes, 
    const QChar& _separator = QChar(';'), 
    const QChar& _equal = QChar('='),
    std::function<void(const QString& _error)> _err = [](const QString&){});

SDataSpecification parseDataSpecification(const QString _dataSpec,
    std::function<void(const QString& _error)> _err = [](const QString&){});





/*
 * ENG>
 *  Decodes a string with multiple attributes and executes 
 *  actions according to their names.
 *  --------------------------------
 *  Parameters:
 *  _assignActions - action map of the format [attribute_name, action];
 *  _attributes    - string with attributes and their values
 *                   "attribute1 = val1; attribute2 = val2; attribure3 = val3";
 *  _attrSeparator - attribute separator;
 *  _attrEqSign    - equality symbol.
 *
 * RU>
 * Дeкодирует строку с множественными аттрибутами и выполняет 
 * дейсвие применения значения в соответствии с их именами.
 * Формат строки:
 *  "attribute1 = val1; attribute2 = val2; attribure3 = val3"
 *  --------------------------------
 *  Параметры:
 *  _assignActions - карта действий формата [attribute_name, action];
 *  _attributes    - строка формата
 *                   "attribute1 = val1; attribute2 = val2; attribure3 = val3";
 *  _attrSeparator - разделитель атрибутов;
 *  _attrEqSign    - символ равенства.
 *
 */
/* void setMultipleAttributes( */
/*     const QMap<QString, std::function<void(const QString& _val)>>& _assigns, */
/*     const QString& _attributes, */
/*     const QString _attrSeparator = QString(";"), */ 
/*     const QString _attrEqSign = QString("=")); */

/*
 * ENG>
 *  Decodes a string with multiple values and executes 
 *  assign actions
 *  --------------------------------
 *  Parameters:
 *  _assigns          - assign actions list;
 *  _values           - string with values
 *                      "val1;val2;val3";
 *  _valuesSeparator  - values separator;
 *
 * RU>
 * Дeкодирует строку с множественными значениями и выполняет 
 *  и выполняет действия назначения.
 * Формат строки:
 *  "val1;val2;val3"
 *  --------------------------------
 *  Параметры:
 *  _assigns          - список действий назначения;
 *  _values           - строка со значениями "val1;val2;val3";
 *  _valuesSeparator  - разделитель значений;
 *
 */
/* void setMultipleValues( */
/*     const QList<std::function<void(const QString& _val)>>& _assigns, */
/*     const QString& _values, */
/*     const QString _valuesSeparator = QString(";")); */

/* RU>
 * Выполняет параметризированное действие (формата: action(p1, p2, ..., pN)),
 * выбирая их из переданной карты и передавая параметры через список строк.
 *
 * EN>
 * Performs a parameterized action (format: action (p1, p2,..., pN)),
 * selecting them from the passed map and passing the 
 * parameters through a list of strings.
 *
 */
/* void performParamAction( */
/*     const QString& _actionString, */ 
/*     const QMap<QString, std::function<void(const QStringList&)>>& _functors); */
}

#endif /* TNEXCOMMON_H_ */
