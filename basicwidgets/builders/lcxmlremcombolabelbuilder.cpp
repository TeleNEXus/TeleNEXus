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

static QString getFontStyle(const QDomElement& _element, 
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
    QPalette pal = remlabel->palette();
    Qt::Alignment alignment = remlabel->alignment();
    remlabel->setAutoFillBackground(true);
    setPalete(pal, _element);
    QString font_style = getFontStyle(_element, _app);
    setAlign(alignment, _element);
    remlabel->setPalette(pal);
    remlabel->setStyleSheet(font_style);
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
      QString attr = el.attribute(
          LCWidgetBuildersCommon::mAttributes.aligns.attrName);

      Qt::Alignment align = 0;
      if(!LCWidgetBuildersCommon::toAlignFlags(attr, align))
      {
        align = _label->alignment();
      }

      attr = el.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);
      QPalette pal = _label->palette();

      if(!attr.isNull())
      {
        QColor color = LCWidgetBuildersCommon::attributeToColor(attr);
        if(color.isValid())
        {
          pal.setColor(QPalette::ColorRole::Background, color);
        }
      }

      _label->addItem(
          LCWidgetBuildersCommon::getMovie(attr_item, _app), 
          attr_value, pal, align);
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
  Qt::Alignment align = _label->alignment();

  setPalete(pal, _element);
  QString fontStyle = getFontStyle(_element, _app);
  setAlign(align, _element);

  _label->addItem(attr_item, _value, fontStyle, pal, align); 

  return true;
}

//==============================================================================
static void setPalete(QPalette& _palette, const QDomElement& _element)
{
  //Цвет фона.
  QString attr = 
    _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);

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
static QString getFontStyle(const QDomElement& _element, 
    const LIApplication& _app)
{
  QString attr = _element.attribute(
      LCWidgetBuildersCommon::mAttributes.fontId);

  if(!attr.isNull())
  {
    attr = _app.getFontStyle(attr);
    if(!attr.isNull()) attr = ".QLabel {font: " + attr + ";}";
  }
}

//==============================================================================
static void setAlign(Qt::Alignment& _alignment, const QDomElement& _element)
{
  QString attr = _element.attribute(
      LCWidgetBuildersCommon::mAttributes.aligns.attrName);
  LCWidgetBuildersCommon::toAlignFlags(attr, _alignment);
}

