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
#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmlcommon.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//==============================================================================
using LQWidgetBuildersMap = QMap<QString, 
      QSharedPointer<LIXmlRemoteDataSourceBuilder>>;

//==============================================================================
static LQWidgetBuildersMap sl_BuildersMap;

//==============================================================================
LCXmlRemoteDataSourceBuilders::LCXmlRemoteDataSourceBuilders() :
  LCXmlBuildersLoader(LCXmlCommon::mBaseTags.sourceBuilders)
{
}

//------------------------------------------------------------------------------
LCXmlRemoteDataSourceBuilders& LCXmlRemoteDataSourceBuilders::instance()
{
  static LCXmlRemoteDataSourceBuilders inst;
  return inst;
}

//------------------------------------------------------------------------------
  QSharedPointer<LIXmlRemoteDataSourceBuilder> 
LCXmlRemoteDataSourceBuilders::getBuilder(const QString _name)
{
  auto it = sl_BuildersMap.find(_name);
  if(it != sl_BuildersMap.end())
  {
    return it.value();
  }
  return nullptr;
}

//------------------------------------------------------------------------------
bool LCXmlRemoteDataSourceBuilders::add(const QString &_name, void *_builder)
{
  if(sl_BuildersMap.find(_name) != sl_BuildersMap.end()) return false;

  sl_BuildersMap.insert(_name,
      QSharedPointer<LIXmlRemoteDataSourceBuilder>
      (static_cast<LIXmlRemoteDataSourceBuilder*>(_builder)));
  return true;
}

//------------------------------------------------------------------------------
bool LCXmlRemoteDataSourceBuilders::noItems()
{
  return sl_BuildersMap.isEmpty();
}
