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

#include "lcxmlpushareabuilder.h"
#include "lcxmlstdactionbuilder.h"
#include "LIApplication.h"

#include <QGuiApplication>
#include <QStackedWidget>
#include <QDebug>
#include <QDomElement>
#include <QMouseEvent>

static const struct
{
  QString picture = "picture";
  QString movie = "movie";
}__slAttributes;

static const struct 
{
  QString actions = "actions";
  QString press   = "press";
  QString release = "release";
  QString pressed = "pressed";
  QString released = "released";
}__slTags;

//==============================================================================
using TActions = LCXmlStdActionBuilder::TActions;


namespace lcxmlpushareabuilder 
{
class CEventFilter : public QObject
  {
  private:
    TActions mActionsPress;
    TActions mActionsRelease;
    QStackedWidget* mpStackedWidget;

    int mIndexPressed = -1;
    int mIndexReleased = -1;

  public:

    CEventFilter(
        const TActions& _actionsPress, 
        const TActions& _actionsRelease, 
        QStackedWidget* _stackedWidget) : QObject(_stackedWidget),
    mActionsPress(_actionsPress),
    mActionsRelease(_actionsRelease),
    mpStackedWidget(_stackedWidget)
    {
    }

    void setIndexPressed(int _index) { mIndexPressed = _index;}
    void setIndexReleased(int _index) { mIndexReleased = _index;}
    int getIndexPressed() { return mIndexPressed;}
    int getIndexReleased() { return mIndexReleased;}

    virtual bool eventFilter(QObject* _obj, QEvent* _event) override
    {

      Q_UNUSED(_obj);
      qDebug() << "event filter " << _event->type();

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonPress:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          actionsPress();
        }
        return true;
        break;

      case QEvent::Type::MouseButtonRelease:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          actionsRelease();
        }
        return true;
        break;

      case QEvent::Type::TouchBegin:
        actionsPress();
        return true;
        break;

      case QEvent::Type::TouchEnd:
        actionsRelease();
        return true;
        break;

      default:
        break;
      }
      return false;
    }

  private:

    //----------------------------------------------------------------------------
    void actionsPress()
    {
      mpStackedWidget->setCurrentIndex(mIndexPressed);
      perform(mActionsPress);
    }

    //----------------------------------------------------------------------------
    void actionsRelease()
    {
      mpStackedWidget->setCurrentIndex(mIndexReleased);
      perform(mActionsRelease);
    }

    //----------------------------------------------------------------------------
    void perform(const TActions& _actions)
    {
      for(auto it = _actions.begin(); it != _actions.end(); it++)
      {
        (*it)();
      }
    }
  };
}

//==============================================================================
LCXmlPushAreaBuilder::LCXmlPushAreaBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlPushAreaBuilder::~LCXmlPushAreaBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlPushAreaBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  TActions actions_press;
  TActions actions_release;


  auto element_actions = _element.firstChildElement(__slTags.actions);
  if(!element_actions.isNull())
  {
      actions_press = LCXmlStdActionBuilder::instance().
        build(_element.firstChildElement(__slTags.press), _app);

      actions_release = LCXmlStdActionBuilder::instance().
        build(_element.firstChildElement(__slTags.release), _app);
  }

  QStackedWidget* stacked_widget = new QStackedWidget();

  lcxmlpushareabuilder::CEventFilter* event_filter = 
    new lcxmlpushareabuilder::CEventFilter(
        actions_press, actions_release, stacked_widget);

  /* stacked_widget->installEventFilter(event_filter); */

  auto add_state = 
    [&_element, &_app, &event_filter](const QString& _tagName, 
        std::function<void(QWidget* _widget)> _adder)
    {
      auto state_element = _element.firstChildElement(_tagName);
      if(state_element.isNull()) return;

      for(auto el = state_element.firstChildElement();
         !el.isNull();
        el = el.nextSiblingElement())
      {
        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;
        auto widget = builder->build(el, _app);
        if(widget)
        {
          _adder(widget);
          widget->installEventFilter(event_filter);
          break;
        }
      } 
    };

  add_state(__slTags.pressed, 
      [&stacked_widget, &event_filter](QWidget* _widget)
      {
        event_filter->setIndexPressed(stacked_widget->addWidget(_widget));
      });

  add_state(__slTags.released, 
      [&stacked_widget, &event_filter](QWidget* _widget)
      {
        event_filter->setIndexReleased(stacked_widget->addWidget(_widget));
      });
  
  stacked_widget->setCurrentIndex(event_filter->getIndexReleased());

  return stacked_widget;
}
