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
#ifndef LIAPPLICATION_H
#define LIAPPLICATION_H

#include <QSharedPointer>
#include <QEvent>
#include <functional>

class QString;
class QDir;
class QDomElement;

class LIXmlRemoteDataSourceBuilder;
class LIXmlLayoutBuilder;
class LIXmlWidgetBuilder;
class LIWindow;
class LIKeyboard;
class QDomDocument;
class LIDataFormatter;
class LIJScriptService;
class QIODevice;

#include "LIRemoteDataSource.h"

class LIApplication
{
public:

  enum class EMessageType
  {
    Untyped,
    Deploy,
    Runtime
  };

  LIApplication(){}

  virtual QString getProjectPath() const = 0;
  
  virtual QString getCurrentPath() const = 0;

  virtual QString toProjectRelativeFilePath(const QString& _file) const = 0;

  virtual QString toProjectRelativePath(const QString& _file) const = 0;

  virtual QSharedPointer<QIODevice> 
    getFileDevice(const QString& _fileName) const = 0;

  virtual QDomDocument 
    loadDomDocument(const QString& _fileName) const = 0; 

  virtual void buildFromFile(
      const QDomElement& _element, 
      const std::function<void(const QDomElement&)>& _builder,
      const QString& _fileAttribute = QString("file")) const = 0;

  virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
    getDataSourceBuilder(const QString& _name) const = 0;

  virtual QSharedPointer<LIXmlLayoutBuilder> 
    getLayoutBuilder(const QString& _name) const = 0;

  virtual QSharedPointer<LIXmlWidgetBuilder> 
    getWidgetBuilder(const QString& _name) const = 0;

  virtual QSharedPointer<LIRemoteDataSource> 
    getDataSource(const QString& _name) const = 0;

  virtual QSharedPointer<LIWindow> 
    getWindow(const QString& _windowId) const = 0;

  virtual QSharedPointer<LIKeyboard> 
    getKeyboard(const QString& _keyboardId) const = 0;

  virtual QSharedPointer<LIDataFormatter> 
    getDataFormatter(const QString& _formatter) const = 0;

  virtual QSharedPointer<LIJScriptService> 
    getScriptService(const QString& _scriptId) const = 0;

  virtual QString getWidgetStyle(const QString& _styleId) const = 0;

  /*
   * Set widget unique name and return them
   */
  virtual QString setWidgetUniqName(QWidget* _widget) const = 0;

  virtual void setWidgetStyle(
      const QString& _style, 
      QWidget* _widget,
      const QString& _objectName  = QString()) const = 0;

  virtual void message(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const = 0;

  virtual void warning(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const = 0;

  virtual void error(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const = 0;

  virtual QObject* createSecurityEventFilter(
      const QString& _accessId,
      const QSet<QEvent::Type>& _events) const = 0;

 /* 
  * Parses the data description string presented in the format: 
  *       'SourceId:DataId:formatterId'
  * Return 'true' if parsing was successful, otherwise 'false'.
  */
  virtual bool parseDataSpecification(
      const QString& _stringToParse,
      QString& _sourceId, 
      QString& _dataId, 
      QString& _formatterId) const = 0;

};

#endif // LIAPPLICATION_H
