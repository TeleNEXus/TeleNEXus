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

#include "xmldataformatters.h"
#include "LIDataFormatter.h"
#include "xmlcommon.h"
#include "lcxmlformatterfactory.h"
#include "lcjsformatter.h"

#include <functional>
#include <QDomElement>
#include <QMap>
#include <QDebug>
#include <QFile>

static const struct
{
  QString file        = "file";
  QString format      = "format";
  QString id          = "id";
}__slAttributes;

static const struct
{
  QString rootTag     = "DATAFORMATS";
  QString script      = "script";
  QString stdformat   = "std";
  QString attributes  = "attributes";
}__slTags;

static QMap<QString, QSharedPointer<LIDataFormatter>> __slFormattersMap;

//==============================================================================
void uploadLocal(const QDomElement& _element)
{
  QString attr_file =  _element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    auto ddoc = xmlcommon::loadDomDocument(attr_file);

    if(!ddoc.isNull())
    {
      QDomElement el = ddoc.documentElement();
      if(!el.isNull())
      {
        if(el.tagName() == __slTags.rootTag) uploadLocal(el);
      }
    }
    return;
  }

  //----------------------------------------------------------upload_script[]
  static auto upload_script = 
    [](const QDomElement& _element)
    {
      QString attr_id = _element.attribute(__slAttributes.id);
      if(attr_id.isNull()) return;

      QString attr_file = _element.attribute(__slAttributes.file);
      if(attr_file.isNull()) return;

      QMap<QString, QString> attr_map;

      for(auto attr_el = _element.firstChildElement(__slTags.attributes);
          !attr_el.isNull();
          attr_el = attr_el.nextSiblingElement(__slTags.attributes))
      {
        auto attr_nodes = attr_el.attributes();
        for(int i = 0; i < attr_nodes.count(); i++)
        {
          QDomNode node = attr_nodes.item(i);
          attr_map.insert(node.nodeName(), node.toAttr().value());
        }
      }

      auto formatter = LCJSFormatter::create(attr_file, attr_map);
      if(formatter.isNull()) return;
      __slFormattersMap.insert(attr_id, formatter);
    };

  //----------------------------------------------------------upload_std[]
  auto upload_std = 
    [](const QDomElement& _element)
    {
      QString attr_id = _element.attribute(__slAttributes.id);
      if(attr_id.isNull()) return;
      auto formatter = stddataformatterfactory::createFormatter(_element);
      if(formatter.isNull()) return;
      __slFormattersMap.insert(attr_id, formatter);
    };


  for(auto node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    auto el = node.toElement();
    if(el.isNull()) {continue;}

    if(el.tagName() == __slTags.script)
    {
      upload_script(el);
    }
    else if(el.tagName() == __slTags.stdformat)
    {
      upload_std(el);
    }
  }
}

//==============================================================================xmldataformatters
namespace xmldataformatters
{

//------------------------------------------------------------------------------
void upload( const QDomElement &_rootElement)
{
  auto element = _rootElement.firstChildElement(__slTags.rootTag);
  if(element.isNull()) return;
  uploadLocal(element);
}

//------------------------------------------------------------------------------
QSharedPointer<LIDataFormatter> getDataFormatter(const QString& _formatterId)
{
  auto it = __slFormattersMap.find(_formatterId);
  if(it == __slFormattersMap.end()) return nullptr;
  return it.value();
}

}


