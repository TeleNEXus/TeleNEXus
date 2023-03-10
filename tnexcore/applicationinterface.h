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

#ifndef APPLICATIONINTERFACE_H_
#define APPLICATIONINTERFACE_H_

#include "LIApplication.h"
#include <QDir>

class LIProjectSource;

class CApplicationInterface : public LIApplication
{
private:
  CApplicationInterface();
  CApplicationInterface(const CApplicationInterface&) = delete;
  CApplicationInterface& operator=(const CApplicationInterface&) = delete;
  ~CApplicationInterface();
public:
  static CApplicationInterface& getInstance();

  void setMessageOn(bool _flag);

  void setProjectSource(QSharedPointer<LIProjectSource> _prjSource);

  virtual QString getProjectPath() const override;

  virtual QString getCurrentPath() const override;

  virtual QString toProjectRelativeFilePath(
      const QString& _file) const override;

  virtual QString toProjectRelativePath(
      const QString& _file) const override;

  virtual QSharedPointer<QIODevice> 
    getFileDevice(const QString& _fileName) const override;

  virtual QDomDocument 
    loadDomDocument(const QString& _fileName) const override;

  virtual void buildFromFile(
      const QDomElement& _element, 
      const std::function<void(const QDomElement&)>& _builder,
      const QString& _fileAttribute = QString("file")) const override;

  virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
    getDataSourceBuilder( const QString& _name) const override;

  virtual QSharedPointer<LIXmlLayoutBuilder> 
    getLayoutBuilder(const QString& _name) const override;

  virtual QSharedPointer<LIXmlWidgetBuilder> 
    getWidgetBuilder(const QString& _name) const override;

  virtual QSharedPointer<LIRemoteDataSource> 
    getDataSource(const QString& _name) const override;

  virtual QSharedPointer<LIWindow> 
    getWindow(const QString& _windowId) const override;

  virtual QSharedPointer<LIKeyboard> 
    getKeyboard(const QString& _keyboardId) const override;

  virtual QSharedPointer<LIDataFormatter> 
    getDataFormatter(const QString& _formatter) const override;

  virtual QSharedPointer<LIJScriptService> 
    getScriptService(const QString& _scriptId) const override;

  virtual QString getWidgetStyle(const QString& _styleId) const override;

  virtual QString setWidgetUniqName(QWidget* _widget) const override;

  virtual void setWidgetStyle( 
      const QString& _style, 
      QWidget* _widget,
      const QString& _objectName = QString()) const override;

  virtual void message(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const override;

  virtual void warning(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const override;

  virtual void error(const QString& _msg, 
      EMessageType _mt = EMessageType::Untyped) const override;

  virtual QObject* createSecurityEventFilter(
      const QString& _accessId,
      const QSet<QEvent::Type>& _events) const override;

  virtual bool parseDataSpecification(
      const QString& _stringToParse,
      QString& _sourceId, 
      QString& _dataId, 
      QString& _formatterId) const override;
};

#endif /* APPLICATIONINTERFACE_H_ */
