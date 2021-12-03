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
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg)
/* #define __smError(msg) CApplicationInterface::getInstance().error(msg) */
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

  auto ret = []()
     {
        __smMessage(
            QString("\n==================End deploy of data sources.\n"));
     };
   
  __smMessage("\n==================Begin deploy of data sources.\n");

  auto element = _rootElement.firstChildElement(__slTags.rootTag);

  if(element.isNull()) 
  {
    return ret();
  }

  QString attr_file = element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    auto ddoc = xmlcommon::loadDomDocument(attr_file);
    if(ddoc.isNull()) 
    {
      return ret();
    }

    element = ddoc.documentElement();

    if(element.tagName() != __slTags.rootTag) 
    { 
      __smWarning(QString("Wrong element tag name '%1' in file '%2'").arg(element.tagName()));
      return ret(); 
    }
  }

  //Addin data sources
  QDomNode node = element.firstChild();

  for(auto node = element.firstChildElement(); 
      !node.isNull(); 
      node = node.nextSiblingElement())
  {
    QDomElement el = node.toElement();
    __smMessage(QString("Begining add sources type of %1").arg(el.tagName()));
    auto builder = builders::sources::getBuilder(el.tagName());

    if(!builder.isNull())
    {
      auto sources = builder->build(el, CApplicationInterface::getInstance());
      auto it = sources.begin();
      if(it != sources.end())
      {
        while(it != sources.end())
        {
          if(__slSources.find(it.key()) != sources.end())
          {
            __slSources.insert(it.key(), it.value());
          }
          it++;
        }
      }
    }
    else
    {
      __smWarning(QString("Can't find builder for source %1").arg(el.tagName()));
    }
    __smMessage(QString("End add sources type of %1").arg(el.tagName()));
  }
  return ret();
}

QSharedPointer<LIRemoteDataSource> getSource(const QString& _id)
{
  auto it = __slSources.find(_id);
  if(it == __slSources.end()) 
  {
    return nullptr;
  }
  return it.value();
}

} /* namespace datasources */
