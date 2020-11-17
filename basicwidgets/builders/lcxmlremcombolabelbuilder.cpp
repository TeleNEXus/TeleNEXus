#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"

#include <QDomElement>
#include <qdom.h>
#include <qnamespace.h>
#include <qwidget.h>
#include <QLabel>
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
  struct
  {
    QString attr_name = "mode";
    struct
    {
      QString normal = "normal";
      QString undef  = "undef";
      QString wrong  = "wrong";
    }vals;
  }mode;

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
    const QString& _baseStyle,
    const LIApplication& _app);

/* static void setPalete(QPalette& _palette, const QDomElement& _element); */

/* static QString getFontStyle(const QDomElement& _element, */ 
/*     const LIApplication& _app); */

/* static void setAlign(Qt::Alignment& _alignment, const QDomElement& _element); */
static QString readStyle(const QDomElement& _element, const LIApplication& _app);
//------------------------------------------------------------------------------
QWidget* LCXmlRemComboLabelBuilder::build(const QDomElement& _element, 
    const LIApplication& _app)
{
  LCQRemComboLabel *remlabel= nullptr;
  QString dataread;
  QString attr = _element.attribute(__attrNames.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LCStringDataFormatterBase> format;

  QString style;

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

  style = readStyle(_element, _app);

  remlabel = new LCQRemComboLabel(dataread, source, format);
  /* remlabel->setAlignment(Qt::AlignmentFlag::AlignCenter); */

  qDebug() << "build style = " << style;

  buildComboLabel(_element, remlabel, format, style, _app);

LABEL_WRONG_EXIT:
  QWidget* ret = remlabel;
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
    LCQRemComboLabel* _cl,
    QSharedPointer<LCStringDataFormatterBase> _format,
    const QString& _baseStyle,
    const LIApplication& _app)
{
  enum class EMode
  {
    normal,
    undef,
    wrong
  };
  for( QDomNode node = _element.firstChildElement(__elementNames.item);
      !node.isNull();
      node = node.nextSiblingElement(__elementNames.item))
  {
    QDomElement el = node.toElement();

    QLabel* label = nullptr;

    QString attr_text = el.attribute(__attrNames.text);
    if(attr_text.isNull()) continue;

    QString attr = el.attribute(__attrNames.mode.attr_name);

    EMode  mode = EMode::normal;
    if(!attr.isNull())
    {
      if(attr == __attrNames.mode.vals.undef)
      {
        mode = EMode::undef;
      }
      else if(attr == __attrNames.mode.vals.wrong)
      {
        mode = EMode::wrong;
      }
    }

    switch(mode)
    {
    case EMode::normal:
      {
        QString attr_value = el.attribute(__attrNames.value);

        if(!attr_value.isNull())
        {
          attr_value = _format->normalizeString(attr_value);
          if(attr_value.isNull()) continue;
        }
        label = new QLabel(attr_text);
        _cl->addItem(label, attr_value);

      }
      break;
    case EMode::undef:
      {
        label = new QLabel(attr_text);
        _cl->addItemUndef(label);
      }
      break;
    case EMode::wrong:
      {
        label = new QLabel(attr_text);
        _cl->addItemWrong(label);
      }
      break;
    }
    if(label != nullptr)
    {
      QString style = readStyle(el,_app);
      style = _baseStyle + style;
      label->setStyleSheet(style);
      qDebug() << "label style sheet = " << style;
    }
  }

}

//==============================================================================
static QString readStyle(const QDomElement& _element, const LIApplication& _app)
{
  /* QString style = ".LCQRemComboLabel { "; */
  QString style;

  QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontId);

  if(!attr.isNull())
  {
    QString font = _app.getFontStyle(attr);
    if(!font.isNull())
    {
      style += "font :" + font + "; ";
    }
  }
  else
  {
    attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.font);
    if(!attr.isNull())
    {
      style += "font : " + attr + "; ";
    }
  }


  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg);
  if(!attr.isNull())
  {
    style += QString("background: %1; ").arg(attr);
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colortext);
  if(!attr.isNull())
  {
    style += QString("color: %1; ").arg(attr);
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.minwidth);
  if(!attr.isNull())
  {
    style += QString("min-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.minheight);
  if(!attr.isNull())
  {
    style += QString("min-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.aligns.attrName);
  if(!attr.isNull())
  {
    attr = LCWidgetBuildersCommon::toAlignString(attr);
    if(!attr.isNull()) style += QString("qproperty-alignment: '%1' ;").arg(attr);
  }
    /* style += "font : 43pt ; "; */
    /* style += "color: green ; "; */

  /* style += "}"; */
  return style;
}

//==============================================================================
static bool addTextItem(
    LCQRemComboLabel*     _label,
    const QString&        _value,
    const QDomElement&    _element, 
    const LIApplication&  _app)
{
  /* if(_element.isNull()) */ 
  /* { */
  /*   return false; */
  /* } */

  /* QString attr_item = _element.attribute(__attrNames.text); */

  /* if(attr_item.isNull()) return false; */
  /* if(_label == nullptr) */ 
  /* { */
  /*   return false; */
  /* } */

  /* QPalette pal = _label->palette(); */
  /* Qt::Alignment align = _label->alignment(); */

  /* setPalete(pal, _element); */
  /* QString fontStyle = getFontStyle(_element, _app); */
  /* setAlign(align, _element); */

  /* _label->addItem(attr_item, _value, fontStyle, pal, align); */ 

  return false;
}

//==============================================================================
static void setPalete(QPalette& _palette, const QDomElement& _element)
{
  //Цвет фона.
  qDebug() << "----------------11111111 set palette 0";
  return;
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

