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
#include "QString"

QString CApplicationInterface::getProjectPath() const   { return __slXmlMainFilePath;}


QDir getProjectDir() const   {return __slXmlMainFileDir;}

QSharedPointer<LIXmlRemoteDataSourceBuilder> getDataSourceBuilder( const QString& _name) const  
{
  return builders::sources::getBuilder(_name);
}

QSharedPointer<LIRemoteDataSource> 
getDataSource(const QString& _name) const  
{
  auto it = __slRemoteDataSourceMap.find(_name);
  if(it == __slRemoteDataSourceMap.end()) return nullptr;
  return it.value();
}

QSharedPointer<LIXmlLayoutBuilder> 
getLayoutBuilder(const QString& _name) const  
{
  return builders::layouts::getBuilder(_name);
}

QSharedPointer<LIXmlWidgetBuilder> 
getWidgetBuilder(const QString& _name) const  
{
  return builders::widgets::getBuilder(_name);
}

QDomDocument getDomDocument(
    const QString& _fileName) const  
{
  return loadDomElement( _fileName);
}

QSharedPointer<LIWindow> getWindow(
    const QString& _windowId) const  
{
  return uploadwindows::getWindow(_windowId);
}

QSharedPointer<LIKeyboard> getKeyboard(
    const QString& _keyboardId) const  
{
  return uploadkeyboards::getKeyboard(_keyboardId);

}

QString getFontStyle(const QString& _fontId) const  
{
  return LCXmlFonts::instance().getFontStyle(_fontId);
}

QSharedPointer<LIDataFormatter> 
getDataFormatter(const QString& _formatter) const  
{
  auto format = stddataformatterfactory::getFormatter(_formatter);
  if(format.isNull())
  {
    format  = uploaddataformatters::getDataFormatter(_formatter);
  }
  return format;
}

QSharedPointer<LIJScriptService> getScriptService(const QString& _scriptId) const  
{
  return uploadjscripts::getScript(_scriptId);
}

