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
#include "xmldatasources.h"
#include "xmlcommon.h"
#include "xmlbuilders.h"
#include "LIRemoteDataSource.h"
#include "LIXmlRemoteDataSourceBuilder.h"
#include "applicationinterface.h"
#include <QDomElement>
#include <QMap>
#include <QSharedPointer>

#define __smMessageHeader "Data sources:"
#define __smMessageDeploy(message) CApplicationInterface::getInstance().messageDeploy(message)
//==============================================================================
static const struct
{
  QString rootTag = "SOURCES";
}__slTags;

static const struct
{
  QString file = "file";
}__slAttributes;

//==============================================================================
static QMap<QString, QSharedPointer<LIRemoteDataSource>> __slSources;

//==============================================================================
namespace xmldatasources
{

void upload(const QDomElement& _rootElement)
{
  auto element = _rootElement.firstChildElement(__slTags.rootTag);

  if(element.isNull()) 
  {
    __smMessageDeploy(QString("%1 document element has no elements with tag %2")
        .arg(__smMessageHeader).arg(__slTags.rootTag));
    return;
  }

  QString attrFile = element.attribute(__slAttributes.file);

  if(!attrFile.isNull())
  {
    auto ddoc = xmlcommon::loadDomDocument(attrFile);
    if(ddoc.isNull()) 
    {
      return;
    }
    element = xmlcommon::loadDomDocument(attrFile).documentElement();
    if(element.tagName() != __slTags.rootTag) { return; }
  }

  //Добавление источников данных.
  QDomNode node = element.firstChild();

  while(!node.isNull())
  {
    QDomElement el = node.toElement();
    auto builder = builders::sources::getBuilder(el.tagName());

    if(!builder.isNull())
    {
      auto sources = builder->build(el, CApplicationInterface::getInstance());
      auto it = sources.begin();
      while(it != sources.end())
      {
        if(__slSources.find(it.key()) != sources.end())
        {
          __slSources.insert(it.key(), it.value());
        }
        it++;
      }
    }
    node = node.nextSibling();
  }
}

QSharedPointer<LIRemoteDataSource> getSource(const QString& _id)
{
  auto it = __slSources.find(_id);
  if(it == __slSources.end()) 
  {
    __smMessageDeploy(QString("%1 can't find data source with id '%2'")
        .arg(__smMessageHeader).arg(_id));
    return nullptr;
  }
  return it.value();
}

} /* namespace datasources */
