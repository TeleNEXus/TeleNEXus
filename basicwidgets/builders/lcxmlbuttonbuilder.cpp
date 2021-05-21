/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "lcxmlbuttonbuilder.h"
#include "lcqremwritebutton.h"
#include "LIDataFormatter.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "lcbuilderscommon.h"
/* #include "lcqwidgetvisiblecontrol.h" */
#include <QPushButton>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>

/* //============================================================================== */
/* static const struct */ 
/* { */
/*   /1* QString text      = "text"; *1/ */
/*   /1* QString source    = "sourceName"; *1/ */
/*   QString dataName  = "dataName"; */
/*   QString value     = "value"; */
/*   QString id        = "id"; */

/*   struct */
/*   { */
/*     QString attr = "event"; */
/*     struct */
/*     { */
/*       QString pressed = "pressed"; */
/*       QString released = "released"; */
/*     }vals; */
/*   }event; */

/*   struct */
/*   { */
/*     QString attr = "action"; */
/*     struct */
/*     { */
/*       QString hide = "hide"; */
/*       QString show = "show"; */
/*     }vals; */
/*   }action; */

/* }__slAttributes; */

/* //------------------------------------------------------------------------------ */
/* static const struct */
/* { */
/*   QString data = "data"; */
/*   QString window = "window"; */

/* }__slTags; */

/* //============================================================================== */
/* using TActions = QSharedPointer<QList<std::function<void(void)>>>; */


/* //============================================================================== */
/* class CEventFilter : public QObject */
/* { */
/* public: */

/*   using TActions = QList<std::function<void(void)>>; */
/*   QSharedPointer<TActions> spActionsPush; */
/*   QSharedPointer<TActions> spActionsRelease; */

/* private: */
/*   CEventFilter() = delete; */
/*   CEventFilter(QObject* _parent) : QObject(_parent){} */
/* public: */
/*   static void install( */
/*       QPushButton* _button, */ 
/*       QSharedPointer<TActions> _actionsPush, */ 
/*       QSharedPointer<TActions> _actionsRelease) */
/*   { */
/*     CEventFilter* filter = new CEventFilter(_button); */
/*     filter->spActionsPush = _actionsPush; */
/*     filter->spActionsPush = _actionsRelease; */
/*   } */

/*   virtual bool eventFilter(QObject* _obj, QEvent* _event) override */
/*   { */
/*   } */
/* }; */

/* //==============================================================================LCXmlButtonBuilder */
/* LCXmlButtonBuilder::LCXmlButtonBuilder() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* LCXmlButtonBuilder::~LCXmlButtonBuilder() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* static QPushButton* buildWriteButton( */
/*     const QDomElement& _element, const LIApplication& _app); */

/* static QPushButton* buildControllWindowButton( */
/*     const QDomElement& _element, const LIApplication& _app); */

/* static void setStyleSheet(QPushButton* _button, const QDomElement& _element, */ 
/*     const LIApplication& _app); */

/* //------------------------------------------------------------------------------ */
/* QWidget* LCXmlButtonBuilder::buildLocal( */
/*     QSharedPointer<SBuildData> _buildData) */
/* { */
/*   const QDomElement& element = _buildData->element; */
/*   const LIApplication& app = _buildData->application; */

/*   QPushButton* button = new QPushButton(); */

/*   TActions actions_pressed; */
/*   TActions actions_released; */

/*   //init actions pressed */
/*   { */
/*   } */

/*   //init actions released */ 
/*   { */
/*   } */

/*   if(!actions_pressed.isNull()) */
/*   { */
/*     QObject::connect(button, &QPushButton::pressed, */
/*         [actions_pressed]() */
/*         { */
/*         }); */
/*   } */

/*   if(!actions_released.isNull()) */
/*   { */
/*     QObject::connect(button, &QPushButton::released, */
/*         []() */
/*         { */
/*         }); */
/*   } */

/*   setStyleSheet(button, element, app); */
/*   LCBuildersCommon::initPosition(   element, *button); */

/*   return button; */
/* } */

/* //============================================================================== */
/* static void setStyleSheet(QPushButton* _button, const QDomElement& _element, */ 
/*     const LIApplication& _app) */
/* { */
/*   QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app); */

/*   _button->setStyleSheet(style); */

/*   QString attr_icon = _element.attribute( */
/*       LCBuildersCommon::mAttributes.icon); */

/*   if(!attr_icon.isNull()) */
/*   { */
/*     QPixmap pixmap(LCBuildersCommon::getPixmap(attr_icon, _app)); */

/*     QSize size_pixmap = pixmap.size(); */
/*     QSize size_icon = _button->iconSize(); */

/*     QString attr = _element.attribute( */
/*         LCBuildersCommon::mAttributes.iconscale); */

/*     if(!attr.isNull()) */
/*     { */
/*       bool flag = false; */
/*       float scale = attr.toFloat(&flag); */
/*       if(flag) */
/*       { */
/*         pixmap = pixmap.scaled( */
/*             size_pixmap.width() * scale, size_pixmap.height() * scale); */
/*         _button->setIconSize(pixmap.size()); */
/*       } */
/*     } */
/*     else */
/*     { */
/*       attr = _element.attribute(LCBuildersCommon::mAttributes.iconwidth); */
/*       if(!attr.isNull()) */
/*       { */
/*         bool flag = false; */
/*         int width = attr.toInt(&flag); */
/*         if(flag) size_icon.setWidth(width); */
/*       } */

/*       attr = _element.attribute(LCBuildersCommon::mAttributes.iconheight); */
/*       if(!attr.isNull()) */
/*       { */
/*         bool flag = false; */
/*         int height = attr.toInt(&flag); */
/*         if(flag) size_icon.setHeight(height); */
/*       } */

/*       if(size_icon != _button->iconSize()) */
/*       { */
/*         pixmap = pixmap.scaled(size_icon.width(), size_icon.height()); */
/*         _button->setIconSize(size_icon); */
/*       } */
/*     } */
/*     _button->setIcon(pixmap); */
/*   } */
/* } */

/* //============================================================================== */
/* static QPushButton* buildWriteButton( */
/*     const QDomElement& _element, const LIApplication& _app) */
/* { */
/*   LCQRemWriteButton* button = */ 
/*     new LCQRemWriteButton( */
/*         _element.attribute(LCBuildersCommon::mAttributes.text)); */

/*   /1* LCQWidgetVisibleControl::build(_element, button, _app); *1/ */

/*   for(    QDomNode node = _element.firstChild(); */ 
/*       !node.isNull(); */ 
/*       node = node.nextSibling()) */
/*   { */
/*     QDomElement el = node.toElement(); */
/*     if(el.isNull()) continue; */
/*     if(el.tagName() != __slTags.data) continue; */

/*     auto format = _app.getStdDataFormatter( */
/*         el.attribute(LCBuildersCommon::mAttributes.dataformatter)); */

/*     if(format.isNull()) continue; */
/*     auto source = _app.getDataSource( */
/*         el.attribute(LCBuildersCommon::mAttributes.source)); */
/*     if(source.isNull()) continue; */
/*     auto data_name = el.attribute(__slAttributes.dataName); */
/*     if(data_name.isNull()) continue; */
/*     auto value = el.attribute(__slAttributes.value); */
/*     if(value.isNull()) continue; */
/*     auto data = format->toBytes(value); */
/*     if(data.isNull()) continue; */
/*     button->addDataWrite(source, data_name, data); */
/*   } */ 
/*   return button; */
/* } */

/* //------------------------------------------------------------------------------ */
/* struct SAction */
/* { */
/*   enum class EActionType */ 
/*   { */
/*     SHOW, */
/*     HIDE */
/*   }; */
/*   EActionType type; */
/*   QString window; */
/* }; */

/* //------------------------------------------------------------------------------ */
/* static void addAction( */
/*     QList<SAction>& _actionList, */ 
/*     QString _action, */
/*     QString _windowId); */

/* static QPushButton* buildControllWindowButton( */
/*     const QDomElement& _element, const LIApplication& _app) */
/* { */
/*   /1* Q_UNUSED(_app); *1/ */

/*   QPushButton* button = new QPushButton( */
/*       _element.attribute(LCBuildersCommon::mAttributes.text)); */

/*   /1* LCQWidgetVisibleControl::build(_element, button, _app); *1/ */

/*   QList<SAction> pressActions; */
/*   QList<SAction> releaseActions; */

/*   for(QDomNode node = _element.firstChildElement( __slTags.window); */
/*       !node.isNull(); */
/*       node = node.nextSiblingElement( __slTags.window )) */
/*   { */
/*     QDomElement el = node.toElement(); */

/*     QString attr_id     = el.attribute(__slAttributes.id); */
/*     QString attr_action = el.attribute(__slAttributes.action.attr); */
/*     QString attr_event  = el.attribute(__slAttributes.event.attr); */

/*     if(attr_id.isNull() || attr_action.isNull() || attr_event.isNull()) */ 
/*       continue; */

/*     if(attr_event == __slAttributes.event.vals.pressed) */
/*     { */
/*       addAction(pressActions, attr_action, attr_id); */
/*     } */
/*     else if(attr_event == __slAttributes.event.vals.released) */
/*     { */
/*       addAction(releaseActions, attr_action, attr_id); */
/*     } */
/*   } */

/*   //Подключение обработчика сигнала нажатия. */
/*   QObject::connect(button, &QPushButton::pressed, */ 
/*       [pressActions, &_app]() */
/*       { */

/*         for(auto it = pressActions.begin(); it != pressActions.end(); it++) */
/*         { */
/*           QSharedPointer<LIWindow> window = _app.getWindow(it->window); */

/*           if(window.isNull()) return; */

/*           switch(it->type) */
/*           { */
/*           case SAction::EActionType::SHOW: */
/*             window->show(); */
/*             break; */

/*           case SAction::EActionType::HIDE: */
/*             window->hide(); */
/*             break; */
/*           } */
/*         } */
/*       }); */

/*   //Подключение обработчика сигнала отжатия. */
/*   QObject::connect(button, &QPushButton::released, */ 
/*       [releaseActions, &_app]() */
/*       { */
/*         for(auto it = releaseActions.begin(); */ 
/*             it != releaseActions.end(); it++) */
/*         { */
/*           QSharedPointer<LIWindow> window = _app.getWindow(it->window); */

/*           if(window.isNull()) return; */

/*           switch(it->type) */
/*           { */
/*           case SAction::EActionType::SHOW: */
/*             window->show(); */
/*             break; */

/*           case SAction::EActionType::HIDE: */
/*             window->hide(); */
/*             break; */
/*           } */
/*         } */
/*       }); */


/*   return button; */ 
/* } */

/* //------------------------------------------------------------------------------ */
/* static void addAction( */
/*     QList<SAction>& _actionList, */ 
/*     QString _action, */
/*     QString _windowId) */
/* { */
/*   if(_action == __slAttributes.action.vals.show) */
/*   { */
/*     _actionList << SAction{ SAction::EActionType::SHOW, _windowId}; */
/*   } */
/*   else if(_action == __slAttributes.action.vals.hide) */
/*   { */
/*     _actionList << SAction{ SAction::EActionType::HIDE, _windowId}; */
/*   } */
/* } */









#include "lcxmlbuttonbuilder.h"
#include "lcqremwritebutton.h"
#include "LIDataFormatter.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "lcbuilderscommon.h"
/* #include "lcqwidgetvisiblecontrol.h" */
#include <QPushButton>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>

//==============================================================================
static const struct 
{
  /* QString text      = "text"; */
  /* QString source    = "sourceName"; */
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

//------------------------------------------------------------------------------
QWidget* LCXmlButtonBuilder::buildLocal(
    QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QPushButton* button;
  switch(mType)
  {
  case EType::writeData:
    button = buildWriteButton(element, app);
    break;

  case EType::controlWindows:
    button = buildControllWindowButton(element, app);
    break;

  default:
    button = new QPushButton();
  }


  setStyleSheet(button, element, app);

  LCBuildersCommon::initPosition(   element, *button);

  return button;
}

//==============================================================================
static void setStyleSheet(QPushButton* _button, const QDomElement& _element, 
    const LIApplication& _app)
{
  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);

  _button->setStyleSheet(style);

  QString attr_icon = _element.attribute(
      LCBuildersCommon::mAttributes.icon);

  if(!attr_icon.isNull())
  {
    QPixmap pixmap(LCBuildersCommon::getPixmap(attr_icon, _app));

    QSize size_pixmap = pixmap.size();
    QSize size_icon = _button->iconSize();

    QString attr = _element.attribute(
        LCBuildersCommon::mAttributes.iconscale);

    if(!attr.isNull())
    {
      bool flag = false;
      float scale = attr.toFloat(&flag);
      if(flag)
      {
        pixmap = pixmap.scaled(
            size_pixmap.width() * scale, size_pixmap.height() * scale);
        _button->setIconSize(pixmap.size());
      }
    }
    else
    {
      attr = _element.attribute(LCBuildersCommon::mAttributes.iconwidth);
      if(!attr.isNull())
      {
        bool flag = false;
        int width = attr.toInt(&flag);
        if(flag) size_icon.setWidth(width);
      }

      attr = _element.attribute(LCBuildersCommon::mAttributes.iconheight);
      if(!attr.isNull())
      {
        bool flag = false;
        int height = attr.toInt(&flag);
        if(flag) size_icon.setHeight(height);
      }

      if(size_icon != _button->iconSize())
      {
        pixmap = pixmap.scaled(size_icon.width(), size_icon.height());
        _button->setIconSize(size_icon);
      }
    }
    _button->setIcon(pixmap);
  }
}

//==============================================================================
static QPushButton* buildWriteButton(
    const QDomElement& _element, const LIApplication& _app)
{
  LCQRemWriteButton* button = 
    new LCQRemWriteButton(
        _element.attribute(LCBuildersCommon::mAttributes.text));

  /* LCQWidgetVisibleControl::build(_element, button, _app); */

  for(    QDomNode node = _element.firstChild(); 
      !node.isNull(); 
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    if(el.tagName() != __slTags.data) continue;

    auto format = _app.getStdDataFormatter(
        el.attribute(LCBuildersCommon::mAttributes.dataformatter));

    if(format.isNull()) continue;
    auto source = _app.getDataSource(
        el.attribute(LCBuildersCommon::mAttributes.source));
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
  /* Q_UNUSED(_app); */

  QPushButton* button = new QPushButton(
      _element.attribute(LCBuildersCommon::mAttributes.text));

  /* LCQWidgetVisibleControl::build(_element, button, _app); */

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
            /* window->show(); */
            window->action("show");
            break;

          case SAction::EActionType::HIDE:
            /* window->hide(); */
            window->action("hide");
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

