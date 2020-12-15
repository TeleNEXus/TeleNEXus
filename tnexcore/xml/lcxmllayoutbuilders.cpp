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
#include "lcxmllayoutbuilders.h"
#include "lcxmlcommon.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//==============================================================================
static QMap<QString, QSharedPointer<LIXmlLayoutBuilder>> __slBuildersMap;

//==============================================================================
LCXmlLayoutBuilders::LCXmlLayoutBuilders() :
        LCXmlBuildersLoader( LCXmlCommon::mBaseTags.layoutBuilders )
{
}

//------------------------------------------------------------------------------
LCXmlLayoutBuilders& LCXmlLayoutBuilders::instance()
{
    static LCXmlLayoutBuilders inst;
    return inst;
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlLayoutBuilder> 
LCXmlLayoutBuilders::getBuilder(const QString _name)
{
    auto it = __slBuildersMap.find(_name);
    if(it != __slBuildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlLayoutBuilder>();
}

//------------------------------------------------------------------------------
bool LCXmlLayoutBuilders::add(const QString &_name, void *_builder)
{
    if(__slBuildersMap.find(_name) != __slBuildersMap.end()) return false;

    __slBuildersMap.insert(_name,
                         QSharedPointer<LIXmlLayoutBuilder>
                            (static_cast<LIXmlLayoutBuilder*>(_builder)));
    return true;
}
