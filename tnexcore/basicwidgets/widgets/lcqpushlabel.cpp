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

#include "lcqpushlabel.h"
#include <QMouseEvent>
#include <qnamespace.h>
#include <QDebug>
#include <QTimer>

//==============================================================================
using TActionsList = LCQPushLabel::TActionsList;
//==============================================================================
static bool s_ActionsExecute(const TActionsList& _actions);

//==============================================================================
LCQPushLabel::LCQPushLabel(
      TChangeView _setViewPush, 
      TChangeView _setViewRelease,
      const TActionsList& _actionsPush, 
      const TActionsList& _actionsRelease,
      int _pushDelay) : 
  QLabel(nullptr)
  ,mSetViewPush(_setViewPush)
  ,mSetViewRelease(_setViewRelease)
  ,mActionsPush(_actionsPush)
  ,mActionsRelease(_actionsRelease)
  ,mpTimer(new QTimer(this))
  ,mState(EState::released)
{
  mpTimer->setSingleShot(true);
  mpTimer->setInterval(_pushDelay);
  mSetViewRelease(this);

  connect(mpTimer, &QTimer::timeout, 
      [this]()
      {
        s_ActionsExecute(mActionsPush);
      });
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
}

//------------------------------------------------------------------------------
bool LCQPushLabel::event(QEvent* _event)
{
  bool ret = false;
  //---------------------------------------state_released[]
  auto state_released = 
    [this, &ret](QEvent* _event)
    {
      auto to_state_pressed = 
        [this, &ret]()
        {
          mSetViewPush(this);
          if(mActionsPush.size() != 0)
          {
            mpTimer->start();
            ret = true;
          }
          return EState::pressed;
        };

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonPress:
      /* case QEvent::Type::TouchBegin: */
          return to_state_pressed();
        break;
      default:
        break;
      }
      return EState::released;
    };

  //---------------------------------------state_pressed[]
  auto state_pressed = 
    [this, &ret](QEvent* _event)
    {
      auto to_state_released = 
        [this, &ret]()
        {
          mpTimer->stop();
          mSetViewRelease(this);
          ret = s_ActionsExecute(mActionsRelease);
          return EState::released;
        };

      switch(_event->type())
      {
      /* case QEvent::Type::TouchEnd: */
      case QEvent::Type::MouseButtonRelease:
          return to_state_released();
        break;

      default:
        break;
      }
      return EState::pressed;
    };

  switch(mState)
  {

  case EState::released:
    mState = state_released(_event);
    break;

  case EState::pressed:
    mState = state_pressed(_event);
    break;
  }

  if(!ret) QLabel::event(_event);

  return ret;
}

//==============================================================================
static bool s_ActionsExecute(const TActionsList& _actions)
{
  if(_actions.size() == 0) return false;
  for(auto it = _actions.begin(); it != _actions.end(); it++)
  {
    (*it)();
  }
  return true;
}

