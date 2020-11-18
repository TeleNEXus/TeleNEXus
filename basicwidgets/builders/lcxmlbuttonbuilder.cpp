#include "lcxmlbuttonbuilder.h"
#include "lcxmlstddataformatterfactory.h"
#include "lcqremwritebutton.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "builderscommon.h"
#include <QPushButton>
#include <QDomElement>

//==============================================================================
static const struct 
{
  QString text      = "text";
  QString source    = "sourceName";
  QString dataName  = "dataName";
  QString value     = "value";
  QString id        = "id";

  struct
  {
    QString attr = "event";
    struct
    {
      QString pressed = "pressed";
      QString released = "released";
    }vals;
  }event;

  struct
  {
    QString attr = "action";
    struct
    {
      QString hide = "hide";
      QString show = "show";
    }vals;
  }action;

}__slAttributes;

//------------------------------------------------------------------------------
static const struct
{
  QString data = "data";
  QString window = "window";

}__slTags;

//==============================================================================
LCXmlButtonBuilder::LCXmlButtonBuilder(EType _type) : mType(_type)
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
static QPushButton* buildWriteButton(
    const QDomElement& _element, const LIApplication& _app);

static QPushButton* buildControllWindowButton(
    const QDomElement& _element, const LIApplication& _app);

static void setStyleSheet(QPushButton* _button, const QDomElement& _element, 
    const LIApplication& _app);

QWidget* LCXmlButtonBuilder::build( const QDomElement& _element, 
    const LIApplication& _app)
{
  QPushButton* button;
  switch(mType)
  {
  case EType::writeData:
    button = buildWriteButton(_element, _app);
    break;

  case EType::controlWindows:
    button = buildControllWindowButton(_element, _app);
    break;

  default:
    button = new QPushButton();
  }


  setStyleSheet(button, _element, _app);

  /* LCWidgetBuildersCommon::initSize(       _element, *button); */
  /* LCWidgetBuildersCommon::initFixedSize(  _element, *button); */
  LCWidgetBuildersCommon::initPosition(   _element, *button);

  return button;
}

//==============================================================================
static void setStyleSheet(QPushButton* _button, const QDomElement& _element, 
    const LIApplication& _app)
{
  QString style = LCWidgetBuildersCommon::getBaseStyleSheet(_element, _app);

  _button->setStyleSheet(style);

  QString attr = _element.attribute(LCWidgetBuildersCommon::mAttributes.icon);
  if(!attr.isNull())
  {
    _button->setIcon(LCWidgetBuildersCommon::getPixmap(attr, _app));
  }
}

//==============================================================================
static QPushButton* buildWriteButton(
    const QDomElement& _element, const LIApplication& _app)
{
  LCQRemWriteButton* button = 
    new LCQRemWriteButton(_element.attribute(__slAttributes.text));

  for(    QDomNode node = _element.firstChild(); 
      !node.isNull(); 
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    if(el.tagName() != __slTags.data) continue;
    auto format = LCXmlStdDataFormatterFactory::
      instance().createStringFormatter(el.attributes());
    if(format.isNull()) continue;
    auto source = _app.getDataSource(
        el.attribute(__slAttributes.source));
    if(source.isNull()) continue;
    auto data_name = el.attribute(__slAttributes.dataName);
    if(data_name.isNull()) continue;
    auto value = el.attribute(__slAttributes.value);
    if(value.isNull()) continue;
    auto data = format->toBytes(value);
    if(data.isNull()) continue;
    button->addDataWrite(source, data_name, data);
  } 
  return button;
}

//------------------------------------------------------------------------------
struct SAction
{
  enum class EActionType 
  {
    SHOW,
    HIDE
  };
  EActionType type;
  QString window;
};

//------------------------------------------------------------------------------
static void addAction(
        QList<SAction>& _actionList, 
        QString _action,
        QString _windowId);

static QPushButton* buildControllWindowButton(
    const QDomElement& _element, const LIApplication& _app)
{
    Q_UNUSED(_app);

    QPushButton* button = new QPushButton(_element.attribute(__slAttributes.text));

    QList<SAction> pressActions;
    QList<SAction> releaseActions;

    for(QDomNode node = _element.firstChildElement( __slTags.window);
            !node.isNull();
            node = node.nextSiblingElement( __slTags.window ))
    {
        QDomElement el = node.toElement();

        QString attr_id     = el.attribute(__slAttributes.id);
        QString attr_action = el.attribute(__slAttributes.action.attr);
        QString attr_event  = el.attribute(__slAttributes.event.attr);

        if(attr_id.isNull() || attr_action.isNull() || attr_event.isNull()) 
          continue;

        if(attr_event == __slAttributes.event.vals.pressed)
        {
            addAction(pressActions, attr_action, attr_id);
        }
        else if(attr_event == __slAttributes.event.vals.released)
        {
            addAction(releaseActions, attr_action, attr_id);
        }
    }
    
    //Подключение обработчика сигнала нажатия.
    QObject::connect(button, &QPushButton::pressed, 
            [pressActions, &_app]()
            {

            for(auto it = pressActions.begin(); it != pressActions.end(); it++)
            {
                QSharedPointer<LIWindow> window = _app.getWindow(it->window);
                 
                if(window.isNull()) return;

                switch(it->type)
                {
                case SAction::EActionType::SHOW:
                    window->show();
                break;

                case SAction::EActionType::HIDE:
                    window->hide();
                break;
                }
            }
            });

    //Подключение обработчика сигнала отжатия.
    QObject::connect(button, &QPushButton::released, 
            [releaseActions, &_app]()
            {
            for(auto it = releaseActions.begin(); 
                it != releaseActions.end(); it++)
            {
                QSharedPointer<LIWindow> window = _app.getWindow(it->window);
                 
                if(window.isNull()) return;

                switch(it->type)
                {
                case SAction::EActionType::SHOW:
                    window->show();
                break;

                case SAction::EActionType::HIDE:
                    window->hide();
                break;
                }
            }
            });

    return button; 
}

//------------------------------------------------------------------------------
static void addAction(
        QList<SAction>& _actionList, 
        QString _action,
        QString _windowId)
{
    if(_action == __slAttributes.action.vals.show)
    {
        _actionList << SAction{ SAction::EActionType::SHOW, _windowId};
    }
    else if(_action == __slAttributes.action.vals.hide)
    {
        _actionList << SAction{ SAction::EActionType::HIDE, _windowId};
    }
}

