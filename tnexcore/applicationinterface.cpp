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

#include <QDateTime>
#include <QDebug>
#include <QDir>

#define __smDateTimeFormat QStringLiteral("yyyy:MM:dd|hh:mm:ss")
//==============================================================================
static struct SLocalData
{
  QString xmlMainFileName;
  QString xmlProjectPath;
  QDir    xmlProjectDir;
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
void CApplicationInterface::setParameters(
    const QString& _mainFileName, 
    const QString& _projectPath,
    const QDir& _projectDir)
{
  __slLocalData.xmlMainFileName = _mainFileName;
  __slLocalData.xmlProjectPath = _projectPath;
  __slLocalData.xmlProjectDir = _projectDir;
}

//------------------------------------------------------------------------------
QString CApplicationInterface::getProjectPath() const
{
  return __slLocalData.xmlProjectPath;
}

//------------------------------------------------------------------------------
QDir CApplicationInterface::getProjectDir() const
{
  return __slLocalData.xmlProjectDir;
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceBuilder>
CApplicationInterface::getDataSourceBuilder( const QString& _id) const
{
  return builders::sources::getBuilder(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataSource>
CApplicationInterface::getDataSource(const QString& _id) const
{
  return xmldatasources::getSource(_id);
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
QDomDocument CApplicationInterface::getDomDocument(
    const QString& _fileName) const
{
  return xmlcommon::loadDomDocument(_fileName);
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
void CApplicationInterface::message(const QString& _msg) const
{
  qDebug("%s", qPrintable( 
    QString("M[%1]> %2")
    .arg(QDateTime::currentDateTime().toString(__smDateTimeFormat))
    .arg(_msg)));
}

//------------------------------------------------------------------------------
void CApplicationInterface::warning(const QString& _msg) const
{
  qDebug("%s", qPrintable( 
    QString("W[%1]> %2")
    .arg(QDateTime::currentDateTime().toString(__smDateTimeFormat))
    .arg(_msg)));
}

//------------------------------------------------------------------------------
void CApplicationInterface::error(const QString& _msg) const
{
  qDebug("%s", qPrintable( 
    QString("E[%1]> %2")
    .arg(QDateTime::currentDateTime().toString(__smDateTimeFormat))
    .arg(_msg)));
}
