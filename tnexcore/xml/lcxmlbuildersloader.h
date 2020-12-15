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
#ifndef LCXMLBUILDERSLOADER_H
#define LCXMLBUILDERSLOADER_H

#include <QDomElement>
#include <functional>

class LCXmlBuildersLoader
{
private:
    const QString mTagRoot;
    const QString mAttrFile         = "file";
    const QString mAttrLib          = "lib";
    const QString mAttrLibHandler   = "libhandler";
public:
    LCXmlBuildersLoader(const QString& _tagRoot);
    virtual ~LCXmlBuildersLoader();
    virtual int load(const QDomElement& _element, const QString& _pathPrj, const QStringList& _libPaths) final;
    virtual bool add(const QString& _name, void* _builder) = 0;
private:
    int loadBuilders(const QDomElement& _element, const QStringList& _libPaths);
};

#endif // LCXMLBUILDERSLOADER_H
