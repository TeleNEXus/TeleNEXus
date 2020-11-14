#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <qdom.h>
#include <qnamespace.h>
#include <qwidget.h>

#include <QDebug>
#include "builderscommon.h"

//==============================================================================
LCXmlRemComboLabelBuilder::LCXmlRemComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemComboLabelBuilder::~LCXmlRemComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
    QString dataread    = "read";
    QString source      = "source";
    QString format      = "format";
    QString text        = "text";
    QString value       = "value";
    /* QString movie       = "movie"; */
    QString picture     = "picture";
} __attrNames;

//------------------------------------------------------------------------------
const struct
{
    QString item    = "item";
} __elementNames;

//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
                        LCQRemComboLabel* _label,
                        QSharedPointer<LCStringDataFormatterBase> _format,
                        const LIApplication& _app);

static void setPalete(QPalette& _palette, const QDomElement& _element);

static void setFont(QFont& _font, const QDomElement& _element, 
    const LIApplication& _app);

static void setAlign(Qt::Alignment& _alignment, const QDomElement& _element);
//------------------------------------------------------------------------------
QWidget* LCXmlRemComboLabelBuilder::build(const QDomElement& _element, 
                                            const LIApplication& _app)
{
    LCQRemComboLabel *remlabel= nullptr;
    QString dataread;
    QString attr = _element.attribute(__attrNames.source);
    QSharedPointer<LIRemoteDataSource> source;
    QSharedPointer<LCStringDataFormatterBase> format;

    if(attr.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    source = _app.getDataSource(attr);

    if(source.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    dataread = _element.attribute(__attrNames.dataread);

    if(dataread.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    format = LCXmlStdDataFormatterFactory::instance().
                                createStringFormatter(_element.attributes());
    if(format.isNull())
    {
        goto LABEL_WRONG_EXIT;
    }

    remlabel = new LCQRemComboLabel(dataread, source, format);

    //Параметры текста для всех текстовых сущностей.
    {
      QFont font = remlabel->font();
      QPalette pal = remlabel->palette();
      Qt::Alignment alignment = remlabel->alignment();
      remlabel->setAutoFillBackground(true);
      setPalete(pal, _element);
      setFont(font, _element, _app);
      setAlign(alignment, _element);
      remlabel->setPalette(pal);
      remlabel->setFont(font);
      remlabel->setAlignment(alignment);
      remlabel->adjustSize();
    }

    buildComboLabel(_element, remlabel, format, _app);

LABEL_WRONG_EXIT:
    QLabel* ret = remlabel;
    if(ret == nullptr) ret = new QLabel(_element.tagName());

    LCWidgetBuildersCommon::initSize(_element, *ret);
    LCWidgetBuildersCommon::initFixedSize(_element, *ret);
    LCWidgetBuildersCommon::initPosition(_element, *ret);

    return ret;
}

//------------------------------------------------------------------------------
static bool addTextItem(
    LCQRemComboLabel*     _label,
    const QString&        _value,
    const QDomElement&    _element, 
    const LIApplication&  _app);

static void buildComboLabel( const QDomElement& _element, 
    LCQRemComboLabel* _label,
    QSharedPointer<LCStringDataFormatterBase> _format,
    const LIApplication& _app)
{
  for( QDomNode node = _element.firstChildElement(__elementNames.item);
      !node.isNull();
      node = node.nextSiblingElement(__elementNames.item))
  {
    QDomElement el = node.toElement();
    QString attr_value = el.attribute(__attrNames.value);

    if(!attr_value.isNull())
    {
      attr_value = _format->normalizeString(attr_value);
      if(attr_value.isNull()) continue;
    }

    QString attr_item = el.attribute(__attrNames.text);

    if(addTextItem(_label, attr_value, el, _app))
    {
      continue;
    }

    attr_item = el.attribute(__attrNames.picture);

    if(!attr_item.isNull())
    {
      Qt::Alignment align = _label->alignment();
      QString attr = el.attribute(LCWidgetBuildersCommon::mAttributes.aligns.attrName);

      if(!attr.isNull())
      {
         bool flag = false;
         Qt::Alignment a = LCWidgetBuildersCommon::toAlignFlags(attr, &flag);
         if(flag) align = a;
      }

      _label->addItem(
          LCWidgetBuildersCommon::getMovie(attr_item, _app), 
          attr_value, align);
      continue;
    }

    /* attr_item = el.attribute(__attrNames.picture); */

    /* if(!attr_item.isNull()) */
    /* { */
    /*     _label->addItem( */
    /*             LCWidgetBuildersCommon::getPixmap(attr_item, _app), */ 
    /*             attr_value); */
    /*     continue; */
    /* } */
  }
}

//==============================================================================
static bool addTextItem(
    LCQRemComboLabel*     _label,
    const QString&        _value,
    const QDomElement&    _element, 
    const LIApplication&  _app)
{
    QString attr_item = _element.attribute(__attrNames.text);

    if(attr_item.isNull()) return false;

    QPalette pal = _label->palette();
    QFont font = _label->font();
    Qt::Alignment align = _label->alignment();

    setPalete(pal, _element);
    setFont(font, _element, _app);
    setAlign(align, _element);

    _label->addItem(attr_item, _value, font, pal, align); 

    return true;
}

//==============================================================================
static void setPalete(QPalette& _palette, const QDomElement& _element)
{
  //Цвет фона.
  QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);

  if(!attr.isNull())
  {
    QColor color = LCWidgetBuildersCommon::attributeToColor(attr);
    if(color.isValid())
    {
      _palette.setColor(QPalette::ColorRole::Background, color);
    }
  }

  //Цвет текста.
  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colortext);
  if(!attr.isNull())
  {
    QColor color = LCWidgetBuildersCommon::attributeToColor(attr);
    if(color.isValid())
    {
      _palette.setColor(QPalette::ColorRole::Foreground, color);
    }
  }
}

//==============================================================================
static void setFont(QFont& _font, const QDomElement& _element, 
    const LIApplication& _app)
{
      QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontId);
      if(!attr.isNull())
      {
        bool flag = false;
        QFont f = _app.getFont(attr, &flag);
        if(flag) 
        {
          _font = f;
          return;
        }
      }

      attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontname);
      if(!attr.isNull())
      {
        _font.setFamily(attr);
      }

      attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontsize);
      if(!attr.isNull())
      {
        bool flag = false;
        int size = attr.toInt(&flag);
        if(flag) _font.setPointSize(size);
      }
}

//==============================================================================
static void setAlign(Qt::Alignment& _alignment, const QDomElement& _element)
{
  QString attr = _element.attribute(
      LCWidgetBuildersCommon::mAttributes.aligns.attrName);
  if(attr.isNull()) return;
  bool flag = false;
  Qt::Alignment a = LCWidgetBuildersCommon::toAlignFlags(attr, &flag);
  if(!flag) return;
  _alignment = a;
}


