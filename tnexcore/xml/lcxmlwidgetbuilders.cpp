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
#include "lcxmlwidgetbuilders.h"
#include "lcxmlcommon.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>
#include "basicwidgetbuilders.h"

//==============================================================================
using LQLayoutBuildersMap = QMap<QString, QSharedPointer<LIXmlWidgetBuilder>>;

//==============================================================================
static LQLayoutBuildersMap __buildersMap;

//==============================================================================
LCXmlWidgetBuilders::LCXmlWidgetBuilders() :
        LCXmlBuildersLoader(    LCXmlCommon::mBaseTags.widgetBuilders)
{
  __buildersMap.unite(basicwidgetbuilders::getBuilders());
}

//------------------------------------------------------------------------------
LCXmlWidgetBuilders& LCXmlWidgetBuilders::instance()
{
    static LCXmlWidgetBuilders inst;
    return inst;
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlWidgetBuilder> 
LCXmlWidgetBuilders::getBuilder(const QString _name)
{
    LQLayoutBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlWidgetBuilder>();
}

//------------------------------------------------------------------------------
bool LCXmlWidgetBuilders::add(const QString &_name, void *_builder)
{
    if(__buildersMap.find(_name) != __buildersMap.end()) return false;

    __buildersMap.insert(_name,
                         QSharedPointer<LIXmlWidgetBuilder>
                            (static_cast<LIXmlWidgetBuilder*>(_builder)));
    return true;
}
