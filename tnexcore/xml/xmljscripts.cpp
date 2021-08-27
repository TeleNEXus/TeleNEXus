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

#include "xmljscripts.h"
#include "lcjscriptservice.h"

#include "LIJScriptService.h"
#include "applicationinterface.h"

#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QFile>

#define __smMessageHeader "JScripts:"
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)

static const struct
{
  QString file = "file";
  QString interval = "interval";
  QString id = "id";
  QString scriptId = "scriptId";

  struct
  {
    QString attr = "state";
    struct
    {
      QString start = "start";
      QString stop = "stop";
    }vals;
  }state;

}__slAttributes;

static const struct
{
  QString rootTag     = "SCRIPTS";
  QString script      = "script";
  QString attributes  = "attributes";
  QString launch      = "launch";
  QString execute     = "execute";
}__slTags;

//==============================================================================
QMap<QString, QSharedPointer<LIJScriptService>> __slScriptMap;


//==============================================================================scriptUpload
static void scriptUpload(const QDomElement &_element)
{

  for(QDomNode node = _element.firstChildElement(__slTags.script);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.script))
  {
    QDomElement el = node.toElement();

    QString attr_file = el.attribute(__slAttributes.file);
    if(attr_file.isNull()) continue;

    QString attr_id = el.attribute(__slAttributes.id);
    if(attr_id.isNull()) continue;

    QFile scriptFile(attr_file);
    if (!scriptFile.open(QIODevice::ReadOnly)) 
    {
      continue;
    }

    QTextStream stream(&scriptFile);
    QString script_str = stream.readAll();
    scriptFile.close();

    if(script_str.isNull()) 
    {
      continue;
    }

    //upload attributes.
    QMap<QString, QString> attributes_map;
    for(QDomNode node = el.firstChildElement(__slTags.attributes);
       !node.isNull();
      node = node.nextSiblingElement(__slTags.attributes))
    {
      auto attributes = node.toElement().attributes();
      for(int i = 0; i < attributes.count(); i++)
      {
        auto attr = attributes.item(i).toAttr();
        attributes_map.insert(attr.name(), attr.value());
      }
    } 

    //create servece.
    auto jscriptservice = LCJScriptService::create(script_str, attributes_map, attr_file);
    __slScriptMap.insert(attr_id, jscriptservice);
  }
}

//==============================================================================scriptLaunch
static void scriptLaunch(const QDomElement &_element)
{
  for(auto el = _element.firstChildElement(__slTags.launch);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.launch))
  {
    QString attr = el.attribute(__slAttributes.interval);
    if(attr.isNull()) continue;

    bool flag = false;
    int interval = attr.toInt(&flag);

    if(!flag) continue; 

    attr = el.attribute(__slAttributes.scriptId);
    if(attr.isNull()) continue;
    
    auto it = __slScriptMap.find(attr);
    if(it == __slScriptMap.end()) continue;

    it.value()->launch(interval);
  }
}

//==============================================================================scriptExecute
static void scriptExecute(const QDomElement &_element)
{
  for(auto el = _element.firstChildElement(__slTags.execute);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.execute))
  {
    QString attr = el.attribute(__slAttributes.scriptId);
    if(attr.isNull()) continue;
    
    auto it = __slScriptMap.find(attr);
    if(it == __slScriptMap.end()) continue;

    it.value()->execute();
  }
}

static void uploadLocal(const QDomElement& _element)
{
  static const LIApplication& app = CApplicationInterface::getInstance();
  QString attr_file = _element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == _element.tagName())
      {
        uploadLocal(el);
      }
    }
    return;
  }
  scriptUpload(_element);
  scriptLaunch(_element);
  scriptExecute(_element);
}

//==============================================================================
namespace xmljscripts
{

//------------------------------------------------------------------------------getScript
QSharedPointer<LIJScriptService> getScript(const QString& _scriptId)
{
  auto it = __slScriptMap.find(_scriptId);
  if(it == __slScriptMap.end()) 
  {
    __smMessage(QString("%1 can't find script with id '%2'")
        .arg(__smMessageHeader).arg(_scriptId));
    return nullptr;
  }
  return it.value();
}

//------------------------------------------------------------------------------upload
void upload( const QDomElement &_rootElement)
{
  QDomElement el = _rootElement.firstChildElement(__slTags.rootTag);
  if(el.isNull()) 
  {
    __smMessage(QString("%1 document element has no elements with tag %2")
        .arg(__smMessageHeader).arg(__slTags.rootTag));
    return;
  }
  uploadLocal(el);
}
}


