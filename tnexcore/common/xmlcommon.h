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

class QDomDocument;

namespace xmlcommon
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

/* 
 * Parses the string with values and returns a QStringList with them.
 *  --------------------------------
 *  Parameters:
 *  _values - string for parsing;
 *  _separator - values separator;
 *  _err - error return functor;
 */
QStringList parseValues(
    const QString& _values, 
    const QChar& _separator = QChar(';'),
    std::function<void(const QString& _error)> _err = [](const QString&){});

/* Parses the attributes string in the format 
 * "attribute1 = value1; attribute2 = * value2; ... ;attributeN = valueN",
 * and return a map of values in the form [attribute; value].
 * -------------------------------
 *  Parameters:
 *  _attributes - string for parsing;
 *  _separator - attributes separator;
 *  _equal - equal symbol;
 *  _err = error return functor;
 */
QMap<QString, QString> parseAttributes(
    const QString& _attributes, 
    const QChar& _separator = QChar(';'), 
    const QChar& _equal = QChar('='),
    std::function<void(const QString& _error)> _err = [](const QString&){});

SDataSpecification parseDataSpecification(const QString _dataSpec,
    std::function<void(const QString& _error)> _err = [](const QString&){});


QDomDocument loadDomDocument(const QString& _fileName);
}

#endif /* TNEXCOMMON_H_ */
