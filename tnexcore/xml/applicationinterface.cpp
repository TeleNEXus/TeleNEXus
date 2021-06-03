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
#include "uploadwindows.h"
#include "uploadkeyboards.h"

#include "lcxmlfonts.h"
#include "lcxmlformatterfactory.h"
#include "uploaddataformatters.h"
#include "uploadjscripts.h"

#include <QDir>

//==============================================================================
struct SLocalData
{
  QString xmlMainPath;
  QDir    xmlMainDir;
};
#define toLocalData(p) (static_cast<SLocalData*>(p))
#define ld (*toLocalData(mpLocal))

//==============================================================================
CApplicationInterface::CApplicationInterface()
{
  mpLocal = new SLocalData;
}

//------------------------------------------------------------------------------
CApplicationInterface::~CApplicationInterface()
{
  delete toLocalData(mpLocal);
}

//------------------------------------------------------------------------------
QString CApplicationInterface::getProjectPath() const
{
  return ld.xmlMainPath;
}

//------------------------------------------------------------------------------
QDir CApplicationInterface::getProjectDir() const
{
  return ld.xmlMainDir;
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
  return xmlcommon::loadDomDocument(_fileName, *this);
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
QString CApplicationInterface::getFontStyle(const QString& _fontId) const
{
  return LCXmlFonts::instance().getFontStyle(_fontId);
}

//------------------------------------------------------------------------------
QSharedPointer<LIDataFormatter>
CApplicationInterface::getDataFormatter(const QString& _formatter) const
{
  auto format = stddataformatterfactory::getFormatter(_formatter);
  if(format.isNull())
  {
    format  = uploaddataformatters::getDataFormatter(_formatter);
  }
  return format;
}

//------------------------------------------------------------------------------
QSharedPointer<LIJScriptService>
CApplicationInterface::getScriptService(const QString& _scriptId) const
{
  return uploadjscripts::getScript(_scriptId);
}

