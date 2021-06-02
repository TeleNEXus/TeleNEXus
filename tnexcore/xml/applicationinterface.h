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
#include "LIApplication.h"

class CApplicationInterface : public LIApplication
{
private:

  QString __slXmlMainFilePath;

public:
  CApplicationInterface(){}
  virtual QString getProjectPath() const override;

  virtual QDir getProjectDir() const override;

  virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
    getDataSourceBuilder( const QString& _name) const override;

  QSharedPointer<LIRemoteDataSource> 
    getDataSource(const QString& _name) const override;

  QSharedPointer<LIXmlLayoutBuilder> 
    getLayoutBuilder(const QString& _name) const override;

  QSharedPointer<LIXmlWidgetBuilder> 
    getWidgetBuilder(const QString& _name) const override;

  virtual QDomDocument 
    getDomDocument(const QString& _fileName) const override;

  virtual QSharedPointer<LIWindow> 
    getWindow(const QString& _windowId) const override;

  virtual QSharedPointer<LIKeyboard> 
    getKeyboard(const QString& _keyboardId) const override;

  virtual QString getFontStyle(const QString& _fontId) const override;

  virtual QSharedPointer<LIDataFormatter> 
    getDataFormatter(const QString& _formatter) const override;

  virtual QSharedPointer<LIJScriptService> 
    getScriptService(const QString& _scriptId) const override;
};
