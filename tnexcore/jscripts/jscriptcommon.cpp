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

#include "jscriptcommon.h"
#include <QJSValue>
#include <QJSEngine>
#include <QJSValueIterator>
#include <QFile>
#include "applicationinterface.h"

namespace jscriptcommon
{

//------------------------------------------------------------------------------
void emitEvaluateError(const QJSValue& _value, const QString& _fileName)
{
  QString file_name;
  if(!_fileName.isNull())
  {
    file_name = _fileName;
  }
  else
  {
    file_name = _value.property("fileName").toString();
  }

  CApplicationInterface::getInstance().message(
      QString("jsengine: Uncaught exception in file '%1' at line %2 : \"%3\"")
      .arg(file_name)
      .arg(_value.property("lineNumber").toInt())
      .arg(_value.toString()));
}

//------------------------------------------------------------------------------
void emitRuntimeError(const QJSValue& _value, const QString& _fileName)
{
  QString file_name;
  if(!_fileName.isNull())
  {
    file_name = _fileName;
  }
  else
  {
    file_name = _value.property("fileName").toString();
  }

  CApplicationInterface::getInstance().message(
    QString("jsengine: Uncaught exception in file '%1' at line %2 : \"%3\"")
    .arg(file_name)
    .arg(_value.property("lineNumber").toInt())
    .arg(_value.toString()));
}

//------------------------------------------------------------------------------
void importModule(
    QJSEngine& _targetEngine, 
    const QString& _fileName, 
    const QString& _propertyName)
{

  QJSEngine engine;

  QFile file(_fileName);
  QString script;
  if (file.open(QIODevice::ReadOnly)) 
  {
    script = QString(file.readAll());
    file.close();
  }
  else
  {
    _targetEngine.throwError(QString("Can't open file '%1' for export").
        arg(_fileName));
    return;
  }

  QJSValue jsvalue = _targetEngine.importModule(_fileName);

  if(jsvalue.isError())
  {
    _targetEngine.throwError(QString("Can't import from file \"%1\" ").
        arg(_fileName));
    return;
  }

  if(_propertyName != QString())
  {
    QJSValue jsprop = jsvalue.property(_propertyName);
    if(jsprop.isUndefined())
    {
      _targetEngine.throwError(
          QString("Can't import property \"%1\" from module \"%2\" ").
          arg(_propertyName).
          arg(_fileName));
      return;
    }
    _targetEngine.globalObject().setProperty(_propertyName, jsprop);
    return;
  }

  QJSValueIterator it(jsvalue);
  while(it.hasNext())
  {
    it.next();
    _targetEngine.globalObject().setProperty(it.name(), it.value());
  }
}

}/* namespace */

