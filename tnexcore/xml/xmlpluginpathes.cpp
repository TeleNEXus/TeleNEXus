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
#include "xmlpluginpathes.h"
#include "applicationinterface.h"
#include <QDomElement>
#include <QDir>
#include <QDebug>

#define __smMessageHeader "Pathes of plagins:"
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)

static const struct
{
  QString rootTag = "PLAGINPATHES";
  QString item = "item";
}__slTags;

static const struct
{
  QString path = "path";
}__slAttributes;

static QStringList __slPlaginLibPaths;

namespace xmlpluginpathes
{
void upload(const QDomElement& _rootElement,
    const QString& _defaultPluginsPath)
{
  __slPlaginLibPaths << _defaultPluginsPath;
  QDomNodeList nodes = _rootElement.elementsByTagName(__slTags.rootTag); 
  auto element_pathes = _rootElement.firstChildElement(__slTags.rootTag);
  if(element_pathes.isNull()) 
  {
    __smMessage(
        QString("%1 project root element has no elements with tag %2")
        .arg(__smMessageHeader)
        .arg(__slTags.rootTag));
    return;
  }


  for(auto element_item = element_pathes.firstChildElement(__slTags.item);
      !element_item.isNull();
      element_item = element_item.nextSiblingElement(__slTags.item))
  {
    QString path = element_item.attribute(__slAttributes.path);
    if(path.isNull()) continue;
    __slPlaginLibPaths << path;
    __smMessage(
        QString("%1 added plugins path '%2'")
        .arg(__smMessageHeader)
        .arg(path));

  }
}

const QStringList& getPlaginPathes()
{
  return __slPlaginLibPaths;
}

}
