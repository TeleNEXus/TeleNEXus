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
#include <QApplication>

#define __smMessageHeader "Pathes of plugins:"
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)

static const struct
{
  QString rootTag = "PLAGINPATHES";
  QString add = "add";
}__slTags;

static const struct
{
  QString path = "path";
}__slAttributes;

static QStringList __slPlaginLibPaths;

namespace xmlpluginpathes
{
void upload(const QDomElement& _rootElement)
{
  QString default_plugin_path = QString("%1/%2")
      .arg(QApplication::applicationDirPath())
      .arg("plugins/");

  QStringList added_pathes;

  QDomNodeList nodes = _rootElement.elementsByTagName(__slTags.rootTag); 
  auto element_pathes = _rootElement.firstChildElement(__slTags.rootTag);

  auto add_def_path = 
    [&added_pathes, &default_plugin_path]()
    {
      __slPlaginLibPaths << added_pathes << default_plugin_path;
    };

  if(element_pathes.isNull()) { return add_def_path(); }

  for(auto element_item = element_pathes.firstChildElement(__slTags.add);
      !element_item.isNull();
      element_item = element_item.nextSiblingElement(__slTags.add))
  {
    QString path = element_item.attribute(__slAttributes.path);
    if(path.isNull()) continue;

    path = QDir(path).absolutePath();
    if(!path.contains(QRegExp("\\/$")))
    {
      path += "/";
    }
    added_pathes << path;
  }

  if(added_pathes.isEmpty()) { return add_def_path(); }

  add_def_path();

  QString message("Add plugins pathes:\n");

  for(auto it = added_pathes.begin(); it != added_pathes.end(); it++)
  {
    message += QString("\t%1\n").arg(*it);
  }
  __smMessage(message);
}

const QStringList& getPlaginPathes()
{
  return __slPlaginLibPaths;
}

}
