#include "lcxmlfonts.h"
#include <QFont>
#include <QMap>
#include <QDomElement>
#include "LIApplication.h"
#include "lcxmlapplication.h"
#include <QDebug>

//==============================================================================
QMap<QString, QFont> __slFontMap;

static const QFont __slDefFont;


//==============================================================================
static const struct
{
  QString file = "file";
  QString fontId = "id";
  QString family = "family";
  QString size   = "size";
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
      if(el.tagName() == LCXmlApplication::mBaseTags.fonts) create(el, _app);
    }
    return;
  }
  addFonts(_element);
}

//------------------------------------------------------------------------------
const QFont& LCXmlFonts::getFont(const QString& _fontId, bool* flag)
{
  auto it = __slFontMap.find(_fontId);
  if(it == __slFontMap.end()) return __slDefFont;
  if(flag != nullptr)
  {
    *flag = true;
  }
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

    QFont font;

    QString attr = el.attribute(__slAttributes.family);
    if(!attr.isNull())
    {
      font.setFamily(attr);
      qDebug() << "label set font family = " << attr;
    }

    attr = el.attribute(__slAttributes.size);
    if(!attr.isNull())
    {
      bool flag = false;
      int size = attr.toInt(&flag);
      if(flag)
      {
        font.setPointSize(size);
      }
    }

    attr = el.attribute(__slAttributes.size);
    if(!attr.isNull())
    {
      bool flag = false;
      int size = attr.toInt(&flag);
      if(flag)
      {
        font.setPointSize(size);
      }
    }
    __slFontMap.insert(attr_id, font);
  }

}


