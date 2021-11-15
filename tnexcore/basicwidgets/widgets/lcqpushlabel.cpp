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
using TChangeView = LCQPushLabel::TChangeView;

//==============================================================================
static bool s_ActionsExecute(const TActionsList& _actions);

//==============================================================================
LCQPushLabel::LCQPushLabel(
      TChangeView _setViewPressed, 
      TChangeView _setViewReleased,
      const TActionsList& _pressActions, 
      const TActionsList& _releaseActions,
      int _pushDelay) : 
  QLabel(nullptr)
  ,mState(EState::released)
  ,mpTimer(new QTimer(this))
  ,mSetViewPressed(_setViewPressed)
  ,mSetViewReleased(_setViewReleased)
  ,mPressActions(_pressActions)
  ,mReleaseActions(_releaseActions)
{

  mpTimer->setInterval(_pushDelay);
  mpTimer->setSingleShot(true);
  connect(mpTimer, &QTimer::timeout, 
      [this]()
      {
        s_ActionsExecute(mPressActions);
      });

  setAttribute(Qt::WidgetAttribute::WA_AcceptTouchEvents, true);
  _setViewReleased(this);
  installEventFilter(this);
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
}

//------------------------------------------------------------------------------
bool LCQPushLabel::eventFilter(QObject* _obj, QEvent* _event)
{
  Q_UNUSED(_obj);

  bool ret = false;

  auto state_released = 
    [this, _event, &ret]()
    {

      auto act_press = 
        [this, &ret]()
        {
          ret = true;
          mpTimer->start();
          mSetViewPressed(this);
          mState = EState::pressed;
        };

      switch(_event->type())
      {
      case QEvent::Type::TouchBegin:
        act_press();
        break;

      case QEvent::Type::TouchEnd:
        ret = true;
        break;

      case QEvent::Type::MouseButtonPress:
      case QEvent::Type::MouseButtonDblClick:
        if(static_cast<QMouseEvent*>(_event)->source() != 
            Qt::MouseEventSource::MouseEventNotSynthesized)
        {
          ret = true;
          break;
        }
        act_press();
        break;

      case QEvent::Type::MouseButtonRelease:
        ret = true;
        break;

      default:
        break;
      }
    };

  auto state_pressed = 
    [this, _event, &ret]()
    {
      auto act_release = 
        [this, &ret]()
        {
          ret = true;
          mState = EState::released;
          mSetViewReleased(this);

          if(mpTimer->isActive()) 
          {
            mpTimer->stop();
          }
          else
          {
            s_ActionsExecute(mReleaseActions);
          }
        };

      switch(_event->type())
      {
      case QEvent::Type::TouchBegin:
        ret = true;
        break;

      case QEvent::Type::TouchEnd:
        act_release();
        break;

      case QEvent::Type::MouseButtonPress:
      case QEvent::Type::MouseButtonDblClick:
        ret = true;
        break;

      case QEvent::Type::MouseButtonRelease:
        if(static_cast<QMouseEvent*>(_event)->source() != 
            Qt::MouseEventSource::MouseEventNotSynthesized)
        {
          ret = true;
          break;
        }
        act_release();
        break;

      default:
        break;
      }
    };

  switch(mState)
  {
  case EState::released:
    state_released();
    break;

  case EState::pressed:
    state_pressed();
    break;

  default:
    break;
  }

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

