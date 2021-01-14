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

#include "lcxmljsscripts.h"
#include "lcjscriptservice.h"

#include "LIJScriptService.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QFile>


static const struct
{
  QString file = "file";
  QString interval = "interval";
  QString id = "id";
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
  QString script      = "script";
  QString attributes  = "attributes";
  QString launch      = "launch";
  QString execute     = "execute";
}__slTags;

//==============================================================================
QMap<QString, QSharedPointer<LIJScriptService>> __slScriptMap;
static int __slScriptCounter = 0;

//==============================================================================
namespace xmluploadjsscripts
{

//------------------------------------------------------------------------------getScript
QSharedPointer<LIJScriptService> getScript(const QString& _scriptId)
{
  return __slScriptMap.find(_scriptId).value();
}

//------------------------------------------------------------------------------upload
void upload( const QDomElement &_element, const LIApplication& _app)
{
  QString attr_file = _element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == _element.tagName())
      {
        upload(el, _app);
      }
    }
    return;
  }

  for(QDomNode node = _element.firstChildElement(__slTags.script);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.script))
  {
    QDomElement el = node.toElement();
    QString attr = el.attribute(__slAttributes.file);
    if(attr.isNull()) continue;

    QString fileName = _app.getProjectPath() + attr;
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly)) 
    {
      qDebug() << "LCXmlJScripts::load message: can't open script file " << fileName;
      continue;
    }

    QTextStream stream(&scriptFile);
    QString script = stream.readAll();
    scriptFile.close();

    if(script.isNull()) 
    {
      qDebug() << "LCXmlJScripts::load message: empty script file " << fileName;
      continue;
    }

    int interval = 0;
    attr = el.attribute(__slAttributes.interval);
    if(!attr.isNull())
    {
      bool flag = false;
      interval = attr.toInt(&flag);
      if(!flag)
      {
        interval = 0;
      }
    }

    auto jscriptservice = LCJScriptService::create(script, _app, interval);

    attr = el.attribute(__slAttributes.id);
    if(attr.isNull())
    {
      attr = QString("%1").arg(__slScriptCounter);
    }
    __slScriptMap.insert(attr, jscriptservice);
    __slScriptCounter++;
    attr = el.attribute(__slAttributes.state.attr);
    if(!attr.isNull())
    {
      if(attr == __slAttributes.state.vals.start)
      {
        jscriptservice->launch();
      }
    }
  }
}

}


