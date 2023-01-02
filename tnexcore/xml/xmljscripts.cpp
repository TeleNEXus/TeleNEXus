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


#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg)

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

  const auto upload_script = [](const QDomElement& _scriptElement)
  {
    QString attr_file = _scriptElement.attribute(__slAttributes.file);

    if(attr_file.isNull())
    {
      __smWarning("Script file is not specified\n");
      return;
    }

    QString attr_id = _scriptElement.attribute(__slAttributes.id);
    if(attr_id.isNull())
    {
      __smWarning("Script id is not specified\n");
      return;
    }

    auto fd = CApplicationInterface::getInstance().getFileDevice(attr_file);
    if(fd.isNull()) return;

    if (!fd->open(QIODevice::ReadOnly)) 
    {
      __smWarning(
          QString("Can't open script file '%1' for read\n")
          .arg(attr_file));
      return;
    }

    QTextStream stream(fd.data());
    QString script_str = stream.readAll();
    fd->close();

    if(script_str.isNull()) 
    {
      __smWarning(
          QString("Script file '%1' is empty\n")
          .arg(attr_file));
      return;
    }

    //upload attributes.
    QMap<QString, QString> attributes_map;

    for(auto el = _scriptElement.firstChildElement(__slTags.attributes);
       !el.isNull();
      el= el.nextSiblingElement(__slTags.attributes))
    {
      auto attributes = el.attributes();
      for(int i = 0; i < attributes.count(); i++)
      {
        auto attr = attributes.item(i).toAttr();
        attributes_map.insert(attr.name(), attr.value());
      }
    } 

    //create servece.
    auto jscriptservice = LCJScriptService::create(
        script_str, attributes_map, attr_file, attr_id);
    __slScriptMap.insert(attr_id, jscriptservice);
  };



  for(auto el = _element.firstChildElement(__slTags.script);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.script))
  {
    __smMessage(
        QString("\n\tBegining deploy of script described in line %1\n")
        .arg(el.lineNumber()));
    upload_script(el);
    __smMessage(
        QString("\n\tEnd deploy of script described in line %1\n")
        .arg(el.lineNumber()));
  }
}

//==============================================================================scriptLaunch
static void scriptLaunch(const QDomElement &_element)
{
  auto launch_script = [](const QDomElement& _element)
  {
    QString attr = _element.attribute(__slAttributes.interval);
    if(attr.isNull())
    {
      __smWarning("Script repetition interval is not specified");
      return false;
    }

    bool flag = false;
    int interval = attr.toInt(&flag);

    if(!flag)
    {
      __smWarning("Wrong value of script repetition interval");
      return false;
    }

    attr = _element.attribute(__slAttributes.scriptId);
    if(attr.isNull()) 
    {
      __smWarning("Script id is not specified");
      return false;
    }
    
    auto it = __slScriptMap.find(attr);
    if(it == __slScriptMap.end()) 
    {
      __smMessage(
          QString("Script whith id '%1' is not exist")
          .arg(attr));
      return false;
    }

    it.value()->launch(interval);
    return true;
  };

  for(auto el = _element.firstChildElement(__slTags.launch);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.launch))
  {
    __smMessage(QString("Launch script in line %1").arg(el.lineNumber()));
    if(launch_script(el))
    {
      __smMessage(QString("Script in line %1 is launched").arg(el.lineNumber()));
    }
  }
}

//==============================================================================scriptExecute
static void scriptExecute(const QDomElement &_element)
{
  auto execute = [](const QDomElement& _element)
  {
    QString attr = _element.attribute(__slAttributes.scriptId);
    if(attr.isNull())
    {
      __smWarning("Wrong script id");
      return;
    }
    
    auto it = __slScriptMap.find(attr);
    if(it == __slScriptMap.end())
    {
      __smWarning(QString("Script with id '%1' is not exists").arg(attr));
      return;
    }

    it.value()->execute();
    return;
  };

  for(auto el = _element.firstChildElement(__slTags.execute);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.execute))
  {
    __smMessage(QString("Execute script in line %1").arg(el.lineNumber()));
    execute(el);
  }
}

static void uploadLocal(const QDomElement& _element)
{
  auto builder = 
    [](const QDomElement& _element)
    {
      scriptUpload(_element);
      scriptLaunch(_element);
      scriptExecute(_element);
    };
  CApplicationInterface::getInstance().buildFromFile(_element, builder);
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
    return;
  }
  __smMessage("\n\tBegining deploy of scripts\n");
  uploadLocal(el);
  __smMessage("\n\tEnd deploy of scripts\n");
}
}


