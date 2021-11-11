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
/* struct SLocalData */
/* { */
/*   QSharedPointer<QMap<QEvent::Type, TActions>> mspEventActions; */
/*   QTimer* mpTimer; */
/*   SLocalData() = delete; */
/*   SLocalData(LQEventsFilter* _filter) */
/*   { */
/*     mpTimer = new QTimer(_filter); */
/*   } */
/* }; */

/* #define toLocalData(p) (reinterpret_cast<SLocalData*>(p)) */
/* #define ld  (*(toLocalData(mpLocalData))) */

//==============================================================================LQEventsFilter
LQEventsFilter::LQEventsFilter( 
    const TActionsMap& _actionsMap, QObject* _parent) : 
  QObject(_parent)
  ,mActionsMap(_actionsMap)
{
  mpTimer = new QTimer(this);
}

//------------------------------------------------------------------------------
LQEventsFilter::~LQEventsFilter()
{
}

//------------------------------------------------------------------------------
void LQEventsFilter::install(
    QObject* _target, const QDomElement& _element, const LIApplication& _app) 
{
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

  /* auto action_press_with_delay = */
  /*   [&get_actions, &_app, &events_element]() */
  /*   { */
  /*     QDomElement el = events_element.firstChildElement(__slTags.press); */
  /*     if(el.isNull()) return false; */
  /*     QString attr_delay = el.attribute(__slAttributes.delay); */
  /*     if(attr_delay.isNull()) return false; */

  /*     bool flag = false; */
  /*     int delay = attr_delay.toInt(&flag); */
  /*     if(flag == false) return false; */
  /*     if(delay <= 0) return false; */


  /*     TActions actions_press = get_actions(__slTags.press); */
  /*     if(actions_press.size() == 0) return false; */

  /*     TActions actions_press_handle; */ 
  /*     /1* actions_press_handle << [&actions_press]() *1/ */ 
  /*     /1* { *1/ */ 
  /*     /1* }; *1/ */

  /*     return true; */
  /*   }; */


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
    if(it_press_actions == filter->mActionsMap.end()) return;
    TActions press_actions = it_press_actions.value();
    QDomElement press_element = events_element.firstChildElement(__slTags.press);
    QString attr_delay = press_element.attribute(__slAttributes.delay);
    if(attr_delay.isNull()) return;
    bool flag = false;
    int delay = attr_delay.toInt(&flag);
    if(!flag) return;

    filter->mpTimer->setInterval(delay);
    filter->mpTimer->setSingleShot(true);

    QObject::connect(filter->mpTimer, &QTimer::timeout, 
        [press_actions]()
        {
          for(auto it = press_actions.begin(); 
              it != press_actions.end(); 
              it++)
          {
            (*it)();
          }
        });

    press_actions.clear();
    press_actions << 
      [filter]()
      {
        filter->mpTimer->start();
      };

    filter->mActionsMap[QEvent::MouseButtonPress] = press_actions;

    filter->mActionsMap[QEvent::MouseButtonRelease] = 
      filter->mActionsMap[QEvent::MouseButtonRelease] << 
      [filter]()
      {
        filter->mpTimer->stop();
      };
  }();

  _target->installEventFilter(filter);
}

//------------------------------------------------------------------------------
bool LQEventsFilter::eventFilter(QObject* _obj, QEvent* _event)
{
  Q_UNUSED(_obj);
  auto it = mActionsMap.find(_event->type());
  if(it == mActionsMap.end()) return false;

  for(auto handle_it = it.value().begin(); 
      handle_it != it.value().end(); 
      handle_it++)
  {
    (*handle_it)();
  }
  return false;
}
