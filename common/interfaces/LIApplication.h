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

class QString;
class QDir;

class LIXmlRemoteDataSourceBuilder;
class LIXmlLayoutBuilder;
class LIXmlWidgetBuilder;
class LIWindow;
class LIKeyboard;
class QDomDocument;
class LIDataFormatter;
class LIJScriptService;

#include "LIRemoteDataSource.h"

class LIApplication
{
public:
    LIApplication(){}
    /*
     * Возвращает путь к каталогу проекта.
     */
    virtual QString getProjectPath() const = 0;
    /*
     * Возвращает папку главного файла Xml.
     */
    virtual QDir getProjectDir() const = 0;
    /*
     * Возвращает интеллектуальный указатель на источник данных
     */
    virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
        getDataSourceBuilder(const QString& _name) const = 0;

    virtual QSharedPointer<LIRemoteDataSource> 
        getDataSource(const QString& _name) const = 0;
    /*
     * Возвращает интеллектуальный указатель на построитель 
     * компоновцика объектов.
     */
    virtual QSharedPointer<LIXmlLayoutBuilder> 
        getLayoutBuilder(const QString& _name) const = 0;
    /*
     * Возвращает интеллектуальный указатель на построитель визуального объекта.
     */
    virtual QSharedPointer<LIXmlWidgetBuilder> 
        getWidgetBuilder(const QString& _name) const = 0;
    /*
     * Производит поиск и загрузку xml файла в элемент dom используя
     * относительный путь к файлу.
     */
    virtual QDomDocument 
        getDomDocument(const QString& _fileName) const = 0; 

    /*
     * Возвращает указатель на окно с заданным именем. 
     */
    virtual QSharedPointer<LIWindow> 
        getWindow(const QString& _windowId) const = 0;

    /*
     * Возвращает указатель на окно с заданным именем. 
     */
    virtual QSharedPointer<LIKeyboard> 
        getKeyboard(const QString& _keyboardId) const = 0;
    
    /*
     * Возвращает строку стиля шрифта. 
     */
    virtual QString getFontStyle(const QString& _fontId) const = 0;

    //TODO: оставить только один метод получения форматтера данных.
    virtual QSharedPointer<LIDataFormatter> 
      getStdDataFormatter(const QString& _name) const = 0;

    virtual QSharedPointer<LIDataFormatter> 
      getDataFormatter(const QString& _formatterId) const = 0;

    virtual QSharedPointer<LIJScriptService> 
      getScriptService(const QString& _scriptId) const = 0;
};

#endif // LIAPPLICATION_H
