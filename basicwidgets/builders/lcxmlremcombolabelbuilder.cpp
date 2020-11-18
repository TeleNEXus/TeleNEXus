#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlstddataformatterfactory.h"
#include "LIApplication.h"
#include "LIMovieAccess.h"

#include <QDomElement>
#include <qdom.h>
#include <qnamespace.h>
#include <qwidget.h>
#include <QLabel>
#include <QDebug>
#include "builderscommon.h"

//==============================================================================
class CQMovieLabel : public QLabel
{
private:
  QSharedPointer<LIMovieAccess> mspMovieAccess;
public:
  explicit CQMovieLabel(
      QSharedPointer<LIMovieAccess> _movieAccess, 
      QWidget* _parent = nullptr) : 
    QLabel(_parent),
    mspMovieAccess(_movieAccess)
  {
    setMovie(mspMovieAccess->getMovie());
    /* adjustSize(); */
  }

  virtual void showEvent(QShowEvent* _event) override
  {
    Q_UNUSED(_event);
    mspMovieAccess->start();
  }

  virtual void hideEvent(QHideEvent* _event) override
  {
    Q_UNUSED(_event);
    mspMovieAccess->stop();
  }
};

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
enum class EItemMode
{
  normal,
  undef,
  wrong
};
//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
    LCQRemComboLabel* _cl,
    QSharedPointer<LCStringDataFormatterBase> _format,
    const QString& _baseStyle,
    const LIApplication& _app)
{
  for( QDomNode node = _element.firstChildElement(__elementNames.item);
      !node.isNull();
      node = node.nextSiblingElement(__elementNames.item))
  {
    QDomElement el = node.toElement();

    QLabel* label = nullptr;


    QString attr = el.attribute(__attrNames.mode.attr_name);

    EItemMode  mode = EItemMode::normal;
    if(!attr.isNull())
    {
      if(attr == __attrNames.mode.vals.undef)
      {
        mode = EItemMode::undef;
      }
      else if(attr == __attrNames.mode.vals.wrong)
      {
        mode = EItemMode::wrong;
      }
    }

    QString attr_data= el.attribute(__attrNames.text);
    if(!attr_data.isNull())
    {
      switch(mode)
      {
      case EItemMode::normal:
        {
          QString attr_value = el.attribute(__attrNames.value);

          if(!attr_value.isNull())
          {
            attr_value = _format->normalizeString(attr_value);
            if(attr_value.isNull()) continue;
          }
          label = new QLabel(attr_data);
          _cl->addItem(label, attr_value);
        }
        break;
      case EItemMode::undef:
        {
          label = new QLabel(attr_data);
          _cl->addItemUndef(label);
        }
        break;
      case EItemMode::wrong:
        {
          label = new QLabel(attr_data);
          _cl->addItemWrong(label);
        }
        break;
      }

      if(label != nullptr)
      {
        QString style = readStyle(el,_app);
        style = _baseStyle + style;
        label->setStyleSheet(style);
      }
      continue;
    }

    //-------------------------------------picture
    attr_data= el.attribute(__attrNames.picture);
    if(attr_data.isNull()) continue;

    auto movie_access = LCWidgetBuildersCommon::getMovie(attr_data, _app);  
    if(movie_access.isNull()) continue;

    label = nullptr;
    
    switch(mode)
    {
    case EItemMode::normal:
      {
        QString attr_value = el.attribute(__attrNames.value);

        if(!attr_value.isNull())
        {
          attr_value = _format->normalizeString(attr_value);
          if(attr_value.isNull()) continue;
        }
        label = new CQMovieLabel(movie_access);
        _cl->addItem(label, attr_value);

      }
      break;
    case EItemMode::undef:
      {
        label = new CQMovieLabel(movie_access);
        _cl->addItemUndef(label);
      }
      break;
    case EItemMode::wrong:
      {
        label = new CQMovieLabel(movie_access);
        _cl->addItemWrong(label);
      }
      break;
    }

    if(label != nullptr)
    {
      QString style = readStyle(el,_app);
      style = _baseStyle + style;
      label->setStyleSheet(style);
    }
  }
}

/* #include <functional> */
/* static void addItem( */
/*     LCQRemComboLabel& _comboLabel, */ 
/*     const QDomElement& el, */ 
/*     const QString& _baseStyle, */ 
/*     QSharedPointer<LCStringDataFormatterBase> _format, */
/*     EItemMode _mode, */
/*     std::function<void(QLabel**)> _newLabel) */
/* { */
/* } */

//==============================================================================
static QString readStyle(const QDomElement& _element, const LIApplication& _app)
{
  return LCWidgetBuildersCommon::getBaseStyleSheet(_element, _app);
  /* QString style; */

  /* QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fontId); */

  /* if(!attr.isNull()) */
  /* { */
  /*   QString font = _app.getFontStyle(attr); */
  /*   if(!font.isNull()) */
  /*   { */
  /*     style += "font :" + font + "; "; */
  /*   } */
  /* } */
  /* else */
  /* { */
  /*   attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.font); */
  /*   if(!attr.isNull()) */
  /*   { */
  /*     style += "font : " + attr + "; "; */
  /*   } */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colorbg); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("background: %1; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.colortext); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("color: %1; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.minwidth); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("min-width: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.minheight); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("min-height: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.maxwidth); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("max-width: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.maxheight); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("max-height: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fixwidth); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("max-width: %1px; ").arg(attr); */
  /*   style += QString("min-width: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.fixheight); */
  /* if(!attr.isNull()) */
  /* { */
  /*   style += QString("max-height: %1px; ").arg(attr); */
  /*   style += QString("min-height: %1px; ").arg(attr); */
  /* } */

  /* attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.aligns.attrName); */
  /* if(!attr.isNull()) */
  /* { */
  /*   attr = LCWidgetBuildersCommon::toAlignString(attr); */
  /*   if(!attr.isNull()) style += QString("qproperty-alignment: '%1' ;").arg(attr); */
  /* } */

  /* return style; */
}

