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

#include "cqeventsfilter.h"
#include "lcxmlstdactionbuilder.h"

#include <QDebug>
#include <functional>
#include <QMap>
#include <QEvent>
#include <QDomElement>
#include <QSharedPointer>

using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================
static const struct
{
  QString events  = "events";
  QString press    = "press";
  QString release = "release";
  QString show    = "show";
  QString hide    = "hide";
}__slTags;

//==============================================================================
struct SLocalData
{
  QSharedPointer<QMap<QEvent::Type, TActions>> mspEventActions;
  SLocalData()
  {
  }
};

#define toLocalData(p) (reinterpret_cast<SLocalData*>(p))
#define ld  (*(toLocalData(mpLocalData)))

//==============================================================================CQEventsFilter
CQEventsFilter::CQEventsFilter(QObject* _parent) : 
  QObject(_parent)
{
  mpLocalData = new SLocalData;
}

//------------------------------------------------------------------------------
CQEventsFilter::~CQEventsFilter()
{
  delete toLocalData(mpLocalData);
}

//------------------------------------------------------------------------------
void CQEventsFilter::install(
    QObject* _target, const QDomElement& _element, const LIApplication& _app) 
{
  QDomElement events_element = _element.firstChildElement(__slTags.events);
  
  if(events_element.isNull()) return;

  QSharedPointer<QMap<QEvent::Type, TActions>> 
    event_actions(new QMap<QEvent::Type, TActions>);

  auto get_actions =
    [&_app, 
    &event_actions, 
    &events_element](const QString _action_name, QEvent::Type _event_type)
    {
      QDomElement el = events_element.firstChildElement(_action_name);
      if(el.isNull()) return;
      TActions actions = LCXmlStdActionBuilder::instance().build(el, _app);
      if(actions.size() == 0) return;
      event_actions->insert(_event_type, actions);
    };

  get_actions(__slTags.press,   QEvent::MouseButtonPress);
  get_actions(__slTags.release, QEvent::MouseButtonRelease);
  get_actions(__slTags.show,    QEvent::Show);
  get_actions(__slTags.hide,    QEvent::Hide);

  if(event_actions->size() == 0) return;

  CQEventsFilter* filter = new CQEventsFilter(_target);

  toLocalData(filter->mpLocalData)->mspEventActions = event_actions;
  _target->installEventFilter(filter);
}

//------------------------------------------------------------------------------
bool CQEventsFilter::eventFilter(QObject* _obj, QEvent* _event)
{
  Q_UNUSED(_obj);
  auto it = ld.mspEventActions->find(_event->type());
  if(it == ld.mspEventActions->end()) return false;

  for(auto handle_it = it.value().begin(); 
      handle_it != it.value().end(); 
      handle_it++)
  {
    (*handle_it)();
  }
  return false;
}
