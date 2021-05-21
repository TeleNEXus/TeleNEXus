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
#include "LIRemoteDataWriter.h"
#include "lcbuilderscommon.h"
/* #include "lcqwidgetvisiblecontrol.h" */
#include <QPushButton>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>

//==============================================================================
static const struct
{
  QString press = "press";
  QString release = "release";
  QString writeData = "writeData";
  QString controlWindow = "controlWindow";
}__slTags;

static const struct
{
  QString sourceId = "sourceId";
  QString dataId = "dataId";
  QString format = "format";
  QString value = "value";
}__slAttributesWriteData;

static const struct
{
  QString windowId = "windowId";
  QString action = "action";
}__slAttributesControlWindow;

static const struct
{
  QString text="text";
}__slAttributes;

//==============================================================================
using TActions = QList<std::function<void(void)>>;

//==============================================================================
class CActionBuilder
{
private:
  QMap<QString, std::function<void(
      TActions&, 
      const QDomElement&, 
      const LIApplication&)>> mActionBuilders;
  CActionBuilder()
  {
  
    //------------------------------------------------writeData[]
    mActionBuilders.insert(__slTags.writeData,
        [](TActions& _actions,
          const QDomElement& _element,
          const LIApplication& _app)
        {
          QString attr_source = _element.attribute(__slAttributesWriteData.sourceId);
          QString attr_data = _element.attribute(__slAttributesWriteData.dataId);
          QString attr_format = _element.attribute(__slAttributesWriteData.format);
          QString attr_value = _element.attribute(__slAttributesWriteData.value);

          if(attr_source.isNull()||
              attr_data.isNull()||
              attr_format.isNull()||
              attr_value.isNull())
            return;

          auto source = _app.getDataSource(attr_source);
          if(source.isNull()) return;
          auto format = _app.getStdDataFormatter(attr_format);
          if(format.isNull())
          {
            format = _app.getDataFormatter(attr_format);
          }
          if(format.isNull()) return;

          auto writer = source->createWriter(attr_data);

          QByteArray data = format->toBytes(attr_value);

          _actions << [writer, data]()
          {
            writer->writeRequest(data);
          };
        });

    //------------------------------------------------controlWindow[]
    mActionBuilders.insert(__slTags.controlWindow,
        [](TActions& _actions,
          const QDomElement& _element,
          const LIApplication& _app)
        {
          QString attr_window = _element.attribute(__slAttributesControlWindow.windowId);
          QString attr_action = _element.attribute(__slAttributesControlWindow.action);

          if(attr_window.isNull()||
              attr_action.isNull())
            return;

          auto window = _app.getWindow(attr_window);
          if(window.isNull()) return;

          _actions << [window, attr_action]()
          {
            qDebug() << "window action " << attr_action;
            window->action(attr_action);
          };
        });
  }
public:
  static CActionBuilder& instance()
  {
    static CActionBuilder builder;
    return builder;
  }

  void build(
      TActions& _actions, 
      const QDomElement& _element, 
      const LIApplication& _app)
  {
    QString action_tag = _element.tagName();
    auto it = mActionBuilders.find(_element.tagName());
    if(it == mActionBuilders.end()) return;
    it.value()(_actions, _element, _app);
  }
};


//==============================================================================LCXmlButtonBuilder
LCXmlButtonBuilder::LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
static void setStyleSheet(QPushButton* _button, const QDomElement& _element, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlButtonBuilder::buildLocal(
    QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;


  QString attr_text = element.attribute(__slAttributes.text);
  if(attr_text.isNull())
  {
    attr_text = QStringLiteral("Button");
  }

  QPushButton* button = new QPushButton(attr_text);

  TActions actions_pressed;
  TActions actions_released;

  auto parse_actions = [&_buildData](
      const QString& _action_tag, TActions& _actions)
  {
    auto element = _buildData->element.firstChildElement(_action_tag);
    if(element.isNull()) return;
    for(auto node = element.firstChild(); !node.isNull(); node = node.nextSibling())
    {
      auto el = node.toElement();
      if(el.isNull()) continue;
      CActionBuilder::instance().build(_actions, el, _buildData->application);
    }
  };

  parse_actions(__slTags.press, actions_pressed);
  parse_actions(__slTags.release, actions_released);

  if(actions_pressed.size() > 0)
  {
    QObject::connect(button, &QPushButton::pressed,
        [actions_pressed]()
        {
          for(auto it = actions_pressed.begin(); it != actions_pressed.end(); it++)
          {
            (*it)();
          }
        });
  }

  if(actions_released.size() > 0)
  {
    QObject::connect(button, &QPushButton::released,
        [actions_released]()
        {
          for(auto it = actions_released.begin(); it != actions_released.end(); it++)
          {
            (*it)();
          }
        });
  }

  setStyleSheet(button, element, app);
  LCBuildersCommon::initPosition(element, *button);

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









/* #include "lcxmlbuttonbuilder.h" */
/* #include "lcqremwritebutton.h" */
/* #include "LIDataFormatter.h" */
/* #include "LIApplication.h" */
/* #include "LIWindow.h" */
/* #include "lcbuilderscommon.h" */
/* /1* #include "lcqwidgetvisiblecontrol.h" *1/ */
/* #include <QPushButton> */
/* #include <QDomElement> */
/* #include <qicon.h> */
/* #include <QDebug> */

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
/* LCXmlButtonBuilder::LCXmlButtonBuilder(EType _type) : mType(_type) */
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

/*   QPushButton* button; */
/*   switch(mType) */
/*   { */
/*   case EType::writeData: */
/*     button = buildWriteButton(element, app); */
/*     break; */

/*   case EType::controlWindows: */
/*     button = buildControllWindowButton(element, app); */
/*     break; */

/*   default: */
/*     button = new QPushButton(); */
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
/*             /1* window->show(); *1/ */
/*             window->action("show"); */
/*             break; */

/*           case SAction::EActionType::HIDE: */
/*             /1* window->hide(); *1/ */
/*             window->action("hide"); */
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

