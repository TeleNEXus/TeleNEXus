#include "lcxmlfonts.h"
#include "lcxmlcommon.h"
#include "LIApplication.h"

#include <QFont>
#include <QMap>
#include <QDomElement>
#include <QDebug>

//==============================================================================
QMap<QString, QString> __slFontStyleMap;

//==============================================================================
static const struct
{
  QString file    = "file";
  QString fontId  = "id";
  QString style   = "style";
}__slAttributes;

//------------------------------------------------------------------------------
static const struct
{
  QString font = "font";
}__slTags;

//==============================================================================
LCXmlFonts& LCXmlFonts::instance()
{
  static LCXmlFonts instance;
  return instance;
}

//------------------------------------------------------------------------------
LCXmlFonts::LCXmlFonts()
{
}

//------------------------------------------------------------------------------
LCXmlFonts::~LCXmlFonts()
{
}

//------------------------------------------------------------------------------
static void addFonts(const QDomElement &_element);
//------------------------------------------------------------------------------
void LCXmlFonts::create(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  QString attr_file =  _element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == LCXmlCommon::mBaseTags.fonts) create(el, _app);
    }
    return;
  }
  addFonts(_element);
}

//------------------------------------------------------------------------------
QString LCXmlFonts::getFontStyle(const QString& _fontId)
{
  auto it = __slFontStyleMap.find(_fontId);
  if(it == __slFontStyleMap.end()) return QString();
  return it.value();
}

//==============================================================================
static void addFonts(const QDomElement &_element)
{
  for(QDomNode node = _element.firstChildElement(__slTags.font);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.font))
  {
    QDomElement el = node.toElement();
    QString attr_id = el.attribute(__slAttributes.fontId);

    if(attr_id.isNull()) continue;

    QString attr_style = el.attribute(__slAttributes.style);
    if(!attr_style.isNull())
    {
      __slFontStyleMap.insert(attr_id, attr_style);
    }
  }
}


