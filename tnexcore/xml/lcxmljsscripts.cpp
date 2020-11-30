#include "lcxmljsscripts.h"
#include "lcxmlcommon.h"
#include "lcjscript.h"

#include "LIJScript.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QFile>


//==============================================================================
static const struct
{
  struct
  {
    QString attr = "state";
    struct
    {
      QString start = "start";
      QString stop = "stop";
    }vals;
  }state;
}__slAttr;

//==============================================================================
QMap<QString, QSharedPointer<LIJScript>> __slScriptMap;
static int __slScriptCounter = 0;

//==============================================================================LCXmlJScripts
LCXmlJScripts::LCXmlJScripts()
{
}

//------------------------------------------------------------------------------
LCXmlJScripts::~LCXmlJScripts()
{
}

//------------------------------------------------------------------------------
LCXmlJScripts& LCXmlJScripts::instance()
{
    static LCXmlJScripts instance;
    return instance;
}

//------------------------------------------------------------------------------
QSharedPointer<LIJScript> LCXmlJScripts::getScript(const QString& _scriptId)
{
    return __slScriptMap.find(_scriptId).value();
}

void LCXmlJScripts::load(
        const QDomElement &_element, 
        const LIApplication& _app)
{
  QString attr_file = _element.attribute(LCXmlCommon::mCommonAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == _element.tagName())
      {
        load(el, _app);
      }
    }
    return;
  }

  for(QDomNode node = _element.firstChildElement(LCXmlCommon::mCommonTags.script);
      !node.isNull();
      node = node.nextSiblingElement(LCXmlCommon::mCommonTags.script))
  {
    QDomElement el = node.toElement();
    QString attr = el.attribute(LCXmlCommon::mCommonAttributes.file);
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

    LCJScript* p_jsscript = nullptr;
    int interval = 0;
    attr = el.attribute(LCXmlCommon::mCommonAttributes.interval);
    if(!attr.isNull())
    {
      bool flag = false;
      interval = attr.toInt(&flag);
      if(!flag)
      {
        interval = 0;
      }
    }
    p_jsscript  = new LCJScript(script, interval);
    attr = el.attribute(LCXmlCommon::mCommonAttributes.id);
    if(attr.isNull())
    {
      attr = QString("%1").arg(__slScriptCounter);
    }
    __slScriptMap.insert(attr, QSharedPointer<LIJScript>(p_jsscript));
    __slScriptCounter++;
    attr = el.attribute(__slAttr.state.attr);
    if(!attr.isNull())
    {
      if(attr == __slAttr.state.vals.start)
      {
        p_jsscript->start();
      }
    }
  }
}



