#include "lcxmlcontrolwindowbuttonbuilder.h"
#include <QEvent>
#include <QPushButton>
#include <qevent.h>
#include <qnamespace.h>
#include <QDomElement>
#include <LIWindow.h>
#include <LIApplication.h>

#include <QDebug>
#include "builderscommon.h"
//------------------------------------------------------------------------------
static const struct
{
    QString label = "label";
}__slAttrs;

static const struct
{
    struct
    {
        QString tag = "window";

        struct
        {
            QString id = "id";
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

        }attrs;

    }window;
}__slTags;

//------------------------------------------------------------------------------
enum class EActionType
{
    SHOW,
    HIDE
};

//------------------------------------------------------------------------------
struct SAction
{
    EActionType type;
    QString window;
};

//==============================================================================
LCXmlControlWindowButtonBuilder::LCXmlControlWindowButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlControlWindowButtonBuilder::~LCXmlControlWindowButtonBuilder()
{
}

//------------------------------------------------------------------------------
static void addAction(
        QList<SAction>& _actionList, 
        QString _action,
        QString _windowId);

QWidget* LCXmlControlWindowButtonBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    Q_UNUSED(_app);

    QPushButton* button = new QPushButton;
    QString attr_label = _element.attribute(__slAttrs.label);

    QList<SAction> pressActions;
    QList<SAction> releaseActions;

    if(!attr_label.isNull())
    {
        button->setText(attr_label);
    }
    else
    {
        
        button->setText(_element.tagName());
    }

    for(QDomNode node = _element.firstChildElement( __slTags.window.tag);
            !node.isNull();
            node = node.nextSiblingElement( __slTags.window.tag ))
    {
        QDomElement el = node.toElement();

        QString attr_id     = el.attribute(__slTags.window.attrs.id);
        QString attr_action = el.attribute(__slTags.window.attrs.action.attr);
        QString attr_event  = el.attribute(__slTags.window.attrs.event.attr);

        if(attr_id.isNull() || attr_action.isNull() || attr_event.isNull()) 
          continue;

        if(attr_event == __slTags.window.attrs.event.vals.pressed)
        {
            addAction(pressActions, attr_action, attr_id);
        }
        else if(attr_event == __slTags.window.attrs.event.vals.released)
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
                case EActionType::SHOW:
                    window->show();
                break;

                case EActionType::HIDE:
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
                case EActionType::SHOW:
                    window->show();
                break;

                case EActionType::HIDE:
                    window->hide();
                break;
                }
            }
            });

    LCWidgetBuildersCommon::initSize(_element, *button);
    LCWidgetBuildersCommon::initFixedSize(_element, *button);
    LCWidgetBuildersCommon::initPosition(_element, *button);

    return button; 
}

//------------------------------------------------------------------------------
static void addAction(
        QList<SAction>& _actionList, 
        QString _action,
        QString _windowId)
{
    if(_action == __slTags.window.attrs.action.vals.show)
    {
        _actionList << SAction{ EActionType::SHOW, _windowId};
    }
    else if(_action == __slTags.window.attrs.action.vals.hide)
    {
        _actionList << SAction{ EActionType::HIDE, _windowId};
    }
}

