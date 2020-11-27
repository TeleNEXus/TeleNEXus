#include "lcxmlremcombolabelbuilder.h"
#include "lcqremcombolabel.h"
#include "lcxmlformatterfactory.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include "LIMovieAccess.h"

#include <QDomElement>
#include <qdom.h>
#include <qnamespace.h>
#include <qwidget.h>
#include <QLabel>
#include <QDebug>

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
  /* QString source      = "source"; */
  QString format      = "format";
  /* QString text        = "text"; */
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
    QSharedPointer<LIDataFormatter> _format,
    const QString& _baseStyle,
    const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlRemComboLabelBuilder::buildLocal( 
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  LCQRemComboLabel *remlabel= nullptr;
  QString dataread;
  QString attr = element.attribute(LCBuildersCommon::mAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  QString style;

  if(attr.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  source = app.getDataSource(attr);

  if(source.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  dataread = element.attribute(__attrNames.dataread);

  if(dataread.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  format = LCXmlStdDataFormatterFactory::instance().
    createStringFormatter(element.attributes());
  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  style = LCBuildersCommon::getBaseStyleSheet(element, app);

  remlabel = new LCQRemComboLabel(dataread, source, format);
  buildComboLabel(element, remlabel, format, style, app);

LABEL_WRONG_EXIT:
  QWidget* ret = remlabel;
  if(ret == nullptr) 
  {
    ret = new QLabel(element.tagName());
    ret->setStyleSheet(style);
  }

  LCBuildersCommon::initPosition(element, *ret);

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
static QString readStylePicture(const QDomElement& _element, 
    const LIApplication& _app);

static void buildComboLabel( const QDomElement& _element, 
    LCQRemComboLabel* _cl,
    QSharedPointer<LIDataFormatter> _format,
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

    QString attr_data= el.attribute(LCBuildersCommon::mAttributes.text);
    if(!attr_data.isNull())
    {

      switch(mode)
      {
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

      default: //normal
        {
          QString attr_value = el.attribute(__attrNames.value);

          if(!attr_value.isNull())
          {
            attr_value = _format->normalize(attr_value);
            if(attr_value.isNull()) continue;
          }
          label = new QLabel(attr_data);
          _cl->addItem(label, attr_value);
        }
      }

      if(label != nullptr)
      {
        label->setStyleSheet(_baseStyle);
      }
      continue;
    }

    //-------------------------------------picture
    attr_data= el.attribute(__attrNames.picture);
    if(attr_data.isNull()) continue;

    auto movie_access = LCBuildersCommon::getMovie(attr_data, _app);  
    if(movie_access.isNull()) continue;

    label = nullptr;
    
    switch(mode)
    {
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

    default: //normal
      {
        QString attr_value = el.attribute(__attrNames.value);

        if(!attr_value.isNull())
        {
          attr_value = _format->normalize(attr_value);
          if(attr_value.isNull()) continue;
        }
        label = new CQMovieLabel(movie_access);
        _cl->addItem(label, attr_value);

      }
      break;
    }

    if(label != nullptr)
    {
      QString style = readStylePicture(el,_app);
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
/*     QSharedPointer<LIDataFormatter> _format, */
/*     EItemMode _mode, */
/*     std::function<void(QLabel**)> _newLabel) */
/* { */
/* } */

//==============================================================================
static QString readStylePicture(const QDomElement& _element, 
    const LIApplication& _app)
{
  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);

  return style;
}
