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

#include "lqeventsfilter.h"
#include <QDebug>
#include <functional>
#include <QMap>
#include <QEvent>
#include <QDomElement>
#include <QSharedPointer>
#include <QTimer>
#include <QWidget>
#include <qnamespace.h>


//==============================================================================
static const struct
{
  QString events  = "events";
  QString press   = "press";
  QString release = "release";
  QString show    = "show";
  QString hide    = "hide";
}__slTags;

static const struct
{
  QString delay   = "delay";
}__slAttributes;

//==============================================================================
void doDeleteLater(QObject* _obj)
{
  _obj->deleteLater();
}


//==============================================================================
using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================
static void s_ExecuteActions(const TActions& _actions)
{
  for(auto it = _actions.begin(); it != _actions.end(); it++)
  {
    (*it)();
  }
}

//==============================================================================LQEventsFilter
LQEventsFilter::LQEventsFilter( 
    const TActionsMap& _actionsMap, QObject* _parent) : 
  QObject(_parent)
  ,mActionsMap(_actionsMap)
  ,mStatePress(EStatePress::released)
{
  mpTimer = new QTimer(this);
}

//------------------------------------------------------------------------------
LQEventsFilter::~LQEventsFilter()
{
}

//------------------------------------------------------------------------------
void LQEventsFilter::install(
    QWidget* _target, const QDomElement& _element, const LIApplication& _app) 
{

  _target->setAttribute(Qt::WidgetAttribute::WA_AcceptTouchEvents, true);
  
  QDomElement events_element = _element.firstChildElement(__slTags.events);
  
  if(events_element.isNull()) return;

  TActionsMap actions_map;

  auto get_actions = 
    [&_app, &events_element](const QString _action_name)
    {
      QDomElement el = events_element.firstChildElement(_action_name);
      if(el.isNull()) return TActions();
      return LCXmlStdActionBuilder::instance().build(el, _app);
    };

  auto install_actions =
    [
    &get_actions,
    &actions_map](const QString _action_name, QEvent::Type _event_type)
    {
      TActions actions = get_actions(_action_name);
      if(actions.size() == 0) return;
      actions_map.insert(_event_type, actions);
    };

  install_actions(__slTags.press,   QEvent::MouseButtonPress);
  install_actions(__slTags.release, QEvent::MouseButtonRelease);

  install_actions(__slTags.show,    QEvent::Show);
  install_actions(__slTags.hide,    QEvent::Hide);

  if(actions_map.size() == 0) return;

  LQEventsFilter* filter = new LQEventsFilter(actions_map, _target);

  //delay for press
  [&filter, &events_element]()
  {
    auto it_press_actions = filter->mActionsMap.find(QEvent::MouseButtonPress);
    auto it_release_actions = filter->mActionsMap.find(QEvent::MouseButtonRelease);

    if(( it_press_actions == filter->mActionsMap.end())) return;

    TActions press_actions;
    TActions release_actions;

    if(it_press_actions != filter->mActionsMap.end())
    {
      press_actions = it_press_actions.value();
    }

    if(it_release_actions != filter->mActionsMap.end())
    {
      release_actions = it_release_actions.value();
    }

    QDomElement press_element = 
      events_element.firstChildElement(__slTags.press);
    QString attr_delay = press_element.attribute(__slAttributes.delay);

    bool flag = false;
    int delay = attr_delay.toInt(&flag);
    if(!flag) delay = 0;


    filter->mpTimer->setInterval(delay);
    filter->mpTimer->setSingleShot(true);


    QObject::connect(filter->mpTimer, &QTimer::timeout, 
        [press_actions]()
        {
          qDebug() << "Mouse delay action press.";
          s_ExecuteActions(press_actions);
        });


    auto press_handler = 
      [filter, press_actions]()
      {
        filter->mStatePress = EStatePress::pressed;
        filter->mpTimer->start();
      };

    auto release_handler = 
      [filter, release_actions]()
      {
        qDebug() << "Release handler 0";
        filter->mStatePress = EStatePress::released;

        if(filter->mpTimer->isActive())
        {
          qDebug() << "Release handler 1";
          filter->mpTimer->stop();
        }
        else
        {
          qDebug() << "Release handler 2";
          s_ExecuteActions(release_actions);
        }
      };


    //------------------------------------------touch
    TActions actions_touch_begin = TActions() <<
      [press_handler]()
      {
        press_handler();
      };

    TActions actions_touch_end = TActions() <<
      [release_handler]()
      {
        release_handler();
      };


    //------------------------------------------mouse
    TActions actions_mouse_press = TActions() << 
      [filter, press_handler]()
      {



        QMouseEvent* me = dynamic_cast<QMouseEvent*>(filter->mpCurrentEvent);

        if(me == nullptr) 
        {
          return;
        }

        if(me->source() != Qt::MouseEventSource::MouseEventNotSynthesized) 
        {
          return;
        }
        qDebug() << "Mouse action press.";
        press_handler();
      };

    TActions actions_mouse_release = TActions() << 
      [filter, release_handler]()
      {
        qDebug() << "Mouse action release 0";
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(filter->mpCurrentEvent);

        if(me == nullptr) 
        {
          return;
        }

        qDebug() << "Mouse action release 1";
        if(me->source() != Qt::MouseEventSource::MouseEventNotSynthesized) 
        {
          return;
        }

        qDebug() << "Mouse action release 2";
        release_handler();
      };

    filter->mActionsMap[QEvent::TouchBegin] = actions_touch_begin;
    filter->mActionsMap[QEvent::TouchEnd] = actions_touch_end;
    filter->mActionsMap[QEvent::MouseButtonPress] = actions_mouse_press;
    filter->mActionsMap[QEvent::MouseButtonRelease] = actions_mouse_release; 

  }();

  _target->installEventFilter(filter);
}

//------------------------------------------------------------------------------
bool LQEventsFilter::eventFilter(QObject* _obj, QEvent* _event)
{
  Q_UNUSED(_obj);
  auto actions_it = mActionsMap.find(_event->type());
  if(actions_it == mActionsMap.end()) return false;

  if(actions_it.value().size() == 0) return false;

  mpCurrentEvent = _event;

  s_ExecuteActions(actions_it.value());

  return true;
}
