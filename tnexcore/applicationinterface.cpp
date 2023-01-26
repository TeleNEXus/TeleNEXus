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

#include "applicationinterface.h"
#include "xmlbuilders.h"
#include "xmldatasources.h"
#include "xmlcommon.h"
#include "xmlwindows.h"
#include "xmlkeyboards.h"
#include "xmlwidgetstyles.h"
#include "lcxmlformatterfactory.h"
#include "xmldataformatters.h"
#include "xmljscripts.h"
#include "lcsecurity.h"
#include "projectsource.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <iostream>
#include <QBuffer>
#include <QFileInfo>
#include <QRegularExpression>
#include <QWidget>

#define lm_DateTimeFormat QStringLiteral("yyyy:MM:dd|hh:mm:ss")

static QString lv_CurrentPath;


//==============================================================================
static struct SLocalData
{
  bool mFlagMessageOn = true;
  QSharedPointer<LIProjectSource> mspProjectSource;
} __slLocalData;

//==============================================================================
CApplicationInterface::CApplicationInterface()
{
}

//------------------------------------------------------------------------------
CApplicationInterface::~CApplicationInterface()
{
}

//------------------------------------------------------------------------------
CApplicationInterface& CApplicationInterface::getInstance()
{
  static CApplicationInterface instance;
  return instance;
}

//------------------------------------------------------------------------------
void CApplicationInterface::setMessageOn(bool _flag)
{
  __slLocalData.mFlagMessageOn = _flag;
}

//------------------------------------------------------------------------------
void CApplicationInterface::setProjectSource(
    QSharedPointer<LIProjectSource> _prjSource)
{
  __slLocalData.mspProjectSource = _prjSource;
}

//------------------------------------------------------------------------------
QString CApplicationInterface::getProjectPath() const
{
  return QDir::currentPath();
}

//------------------------------------------------------------------------------
QString CApplicationInterface::getCurrentPath() const
{
  return lv_CurrentPath;
}

//------------------------------------------------------------------------------
QString CApplicationInterface::toProjectRelativePath(
    const QString& _fileName) const
{
  QString file_path = toProjectRelativeFilePath(_fileName);
  QString file_name = QFileInfo(file_path).fileName();
  return
    file_path.remove(QRegExp(QString("\\/*%1$").arg(file_name)));
}

//------------------------------------------------------------------------------
QString CApplicationInterface::toProjectRelativeFilePath(
    const QString& _fileName) const
{

  auto relative_to_path =
    [](const QString& _path, const QString& _file)
    {
      QString file = _file;
      QString path = _path;

      QRegExp updir("^\\.\\.\\/");
      QRegExp tail("\\/?[^\\/]*$");
      QRegExp root("^\\$\\/");

      //Check project root.
      if(file.contains(root))
      {
        return file.remove(root);
      }

      //Deleting an unsignificant part of the path
      file.remove(QRegExp("^\\.\\/"));
      file.remove(QRegExp("^\\/"));
      path.remove(QRegExp("^\\/"));
      path.remove(QRegExp("^\\.\\/"));
      path.remove(QRegExp("\\/*$"));

      while(file.contains(updir))
      {
        file.remove(updir);
        if(path.length() == 0) return QString();
        path.remove(tail);
      }
      return (path.length() == 0) ? file : QString("%1/%2").arg(path).arg(file);
    };
  return relative_to_path(lv_CurrentPath, _fileName);
}

//------------------------------------------------------------------------------
QSharedPointer<QIODevice> CApplicationInterface::getFileDevice(
    const QString& _fileName) const
{
  return __slLocalData.mspProjectSource->getFileDevice(_fileName);
}

//------------------------------------------------------------------------------
QDomDocument CApplicationInterface::loadDomDocument(
    const QString& _fileName) const
{
  auto fd = getFileDevice(_fileName);

  if(fd.isNull())
  {
    warning(
        QString("Load DOM Document error:\n\t from file '%1'")
        .arg(_fileName));
    return QDomDocument();
  }

  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;

  if(!domDoc.setContent(fd.data(), true, &errorStr, &errorLine, &errorColumn))
  {
    warning(
        QString("Load DOM Document error:\n"
          "\tparse file %1\n"
          "\terror at line: %2\n"
          "\tcolumn: %3\n"
          "\tmsg: %4\n")
        .arg(_fileName)
        .arg(errorLine)
        .arg(errorColumn)
        .arg(errorStr));
  }
  else
  {
    message(
        QString("Load DOM Document:\n\t\
          document with element '%1' from file '%2' is loaded")
        .arg(domDoc.documentElement().tagName())
        .arg(_fileName));
  }
  return domDoc;
}

//------------------------------------------------------------------------------
void CApplicationInterface::buildFromFile(
    const QDomElement& _element,
    const std::function<void(const QDomElement&)>& _builder,
    const QString& _fileAttribute) const
{
  QString file = _element.attribute(_fileAttribute);


  if(file.isNull())
  {
    _builder(_element);
    return;
  }

  QDomDocument ddoc = loadDomDocument(file);

  if(ddoc.isNull())
  {
    _builder(QDomElement());
    return;
  }

  QDomElement loaded_element = ddoc.documentElement();

  if(loaded_element.tagName() != _element.tagName())
  {
    _builder(QDomElement());
    return;
  }

  QString pathbuff = lv_CurrentPath;

  lv_CurrentPath = toProjectRelativePath(file);

  //Add all attributes except file namt.
  for(int i = 0; i < _element.attributes().length(); i++)
  {
    auto attr = _element.attributes().item(i).toAttr();
    if(attr.name() != _fileAttribute)
    {
      loaded_element.setAttribute(attr.name(), attr.value());
    }
  }

  if(loaded_element.hasAttribute(_fileAttribute))
  {
    buildFromFile(loaded_element, _builder, _fileAttribute);
  }
  else
  {
    _builder(loaded_element);
  }

  lv_CurrentPath = pathbuff;
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceBuilder>
CApplicationInterface::getDataSourceBuilder( const QString& _id) const
{
  return builders::sources::getBuilder(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlLayoutBuilder>
CApplicationInterface::getLayoutBuilder(const QString& _id) const
{
  return builders::layouts::getBuilder(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlWidgetBuilder>
CApplicationInterface::getWidgetBuilder(const QString& _id) const
{
  return builders::widgets::getBuilder(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataSource>
CApplicationInterface::getDataSource(const QString& _id) const
{
  return xmldatasources::getSource(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LIWindow> CApplicationInterface::getWindow(
    const QString& _windowId) const
{
  return xmlwindows::getWindow(_windowId);
}

//------------------------------------------------------------------------------
QSharedPointer<LIKeyboard> CApplicationInterface::getKeyboard(
    const QString& _keyboardId) const
{
  return xmlkeyboards::getKeyboard(_keyboardId);
}

//------------------------------------------------------------------------------
QSharedPointer<LIDataFormatter>
CApplicationInterface::getDataFormatter(const QString& _formatter) const
{
  auto format = stddataformatterfactory::getFormatter(_formatter);
  if(format.isNull())
  {
    format  = xmldataformatters::getDataFormatter(_formatter);
  }
  return format;
}

//------------------------------------------------------------------------------
QSharedPointer<LIJScriptService>
CApplicationInterface::getScriptService(const QString& _scriptId) const
{
  return xmljscripts::getScript(_scriptId);
}

//------------------------------------------------------------------------------
QString CApplicationInterface::getWidgetStyle(const QString& _styleId) const
{
  return xmlwidgetstyles::getWidgetStyle(_styleId);
}

//------------------------------------------------------------------------------
QString CApplicationInterface::setWidgetUniqName(QWidget* _widget) const
{
  static quint64 counter = 0;

  QString name = QString("%1_%2")
    .arg(_widget->metaObject()->className())
    .arg(counter);

  _widget->setObjectName(name);
  counter++;
  return name;
}

//------------------------------------------------------------------------------
void CApplicationInterface::setWidgetStyle(
    const QString& _style, QWidget* _widget,
    const QString& _objectName) const
{
  QString outstyle = _style;

  const QRegularExpression rexpr_style_id_desc(
      QStringLiteral("\\$\\([^\\(\\)\\$]+\\)"));

  const QRegularExpression rexpr_style_id_extract(
      QStringLiteral("(?<=\\$\\()(.*)(?=\\))"));

  auto mit= rexpr_style_id_desc.globalMatch(_style);

  while(mit.hasNext())
  {
    QString style_desc = mit.next().captured();
    QString style_id =
      rexpr_style_id_extract.match(style_desc).captured();

    QString load_style =
      CApplicationInterface::getInstance().getWidgetStyle(style_id);
    if(load_style.isNull())
    {
      outstyle.remove(style_desc);
    }
    else
    {
      outstyle.replace(style_desc, load_style);
    }
  }

  auto set_style =
    [_widget, &outstyle](const QString& _objectName)
    {
      outstyle.replace(QStringLiteral("(...)"), _objectName);
      _widget->setStyleSheet(outstyle);
    };

  if(_objectName.isNull())
  {
    set_style(QString("%1#%2")
        .arg(_widget->metaObject()->className())
        .arg(_widget->objectName()));
  }
  else
  {
    set_style(_objectName);
  }
}

//------------------------------------------------------------------------------
void CApplicationInterface::message(const QString& _msg,
    EMessageType _mt) const
{
  Q_UNUSED(_mt);
  if(!__slLocalData.mFlagMessageOn) return;
  qDebug("%s", qPrintable(
        QString("M[%1]> %2")
        .arg(QDateTime::currentDateTime().toString(lm_DateTimeFormat))
        .arg(_msg)));
}

//------------------------------------------------------------------------------
void CApplicationInterface::warning(const QString& _msg,
    EMessageType _mt) const
{
  Q_UNUSED(_mt);
  if(!__slLocalData.mFlagMessageOn) return;
  qDebug("%s", qPrintable(
        QString("W[%1]> %2")
        .arg(QDateTime::currentDateTime().toString(lm_DateTimeFormat))
        .arg(_msg)));
}

//------------------------------------------------------------------------------
void CApplicationInterface::error(const QString& _msg,
    EMessageType _mt) const
{
  Q_UNUSED(_mt);
  if(!__slLocalData.mFlagMessageOn) return;
  qDebug("%s", qPrintable(
        QString("E[%1]> %2")
        .arg(QDateTime::currentDateTime().toString(lm_DateTimeFormat))
        .arg(_msg)));
}

//------------------------------------------------------------------------------
QObject* CApplicationInterface::createSecurityEventFilter(
    const QString& _accessId,
    const QSet<QEvent::Type>& _events) const
{
  return LCSecurity::instance().createEventFilter(_accessId, _events);
}

//------------------------------------------------------------------------------
bool CApplicationInterface::parseDataSpecification(
    const QString& _stringToParse,
    QString& _sourceId,
    QString& _dataId,
    QString& _formatterId) const
{
  bool err_flag = false;
  xmlcommon::SDataSpecification data_spec =
    xmlcommon::parseDataSpecification(
        _stringToParse,
        [&err_flag](const QString& _errStr)
        {
          Q_UNUSED(_errStr);
          err_flag = true;
        });
  if(err_flag) return false;
  _sourceId = data_spec.sourceId;
  _dataId = data_spec.dataId;
  _formatterId = data_spec.formatterId;
  return true;
}

