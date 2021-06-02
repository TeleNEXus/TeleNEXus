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

class CApplicationInterface : public LIApplication
{
public:
  CApplicationInterface(){}
  virtual QString getProjectPath() const override { 
    return __slXmlMainFilePath;}
  virtual QDir getProjectDir() const override {return __slXmlMainFileDir;}

  virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
    getDataSourceBuilder(
        const QString& _name) const override
    {
      return builders::sources::getBuilder(_name);
    }

  QSharedPointer<LIRemoteDataSource> 
    getDataSource(const QString& _name) const override
    {
      auto it = __slRemoteDataSourceMap.find(_name);
      if(it == __slRemoteDataSourceMap.end()) return nullptr;
      return it.value();
    }

  QSharedPointer<LIXmlLayoutBuilder> 
    getLayoutBuilder(const QString& _name) const override
    {
      return builders::layouts::getBuilder(_name);
    }

  QSharedPointer<LIXmlWidgetBuilder> 
    getWidgetBuilder(const QString& _name) const override
    {
      return builders::widgets::getBuilder(_name);
    }

  virtual QDomDocument getDomDocument(
      const QString& _fileName) const override
  {
    return loadDomElement( _fileName);
  }

  virtual QSharedPointer<LIWindow> getWindow(
      const QString& _windowId) const override
  {
    return uploadwindows::getWindow(_windowId);
  }

  virtual QSharedPointer<LIKeyboard> getKeyboard(
      const QString& _keyboardId) const override
  {
    return uploadkeyboards::getKeyboard(_keyboardId);
    
  }

  virtual QString getFontStyle(const QString& _fontId) const override
  {
    return LCXmlFonts::instance().getFontStyle(_fontId);
  }

  virtual QSharedPointer<LIDataFormatter> 
    getDataFormatter(const QString& _formatter) const override
    {
      auto format = stddataformatterfactory::getFormatter(_formatter);
      if(format.isNull())
      {
        format  = uploaddataformatters::getDataFormatter(_formatter);
      }
      return format;
    }

  virtual QSharedPointer<LIJScriptService> getScriptService(const QString& _scriptId) const override
  {
    return uploadjscripts::getScript(_scriptId);
  }
};
