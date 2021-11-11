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
enum EState 
{
  released,
  pressed
};
//==============================================================================
struct SLocalData
{
  EState state;
  QTimer timer;
  SLocalData()
  {
    state = EState::released;
    timer.setSingleShot(true);
  }
};

#define toLocalData(p) (static_cast<SLocalData*>(p))

#define ld (*(toLocalData(mpLocal)))
//==============================================================================
LCQPushLabel::LCQPushLabel(QWidget* _widget) :
  QLabel(_widget),
  mpLocal(new SLocalData())
{
  connect(&(ld.timer), &QTimer::timeout, 
      [this]()
      {
        emit press(this);
      });
  this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
  delete toLocalData(mpLocal);
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
        [this, &ret, _event]()
        {
          ld.timer.start();
          ret = true;
          return EState::pressed;
        };

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonDblClick:
      case QEvent::Type::MouseButtonPress:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          return to_state_pressed();
        }
        break;

      case QEvent::Type::KeyPress:
        {
          QKeyEvent& ke = *(static_cast<QKeyEvent*>(_event));
          if(ke.key() == Qt::Key::Key_Space)
          {
            return to_state_pressed();
          }
        }
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
        [this, &ret, _event]()
        {
          if(!(ld.timer.isActive()))
          {
            emit release(this);
          }
          ld.timer.stop();
          ret = true;
          return EState::released;
        };

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonRelease:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          return to_state_released();
        }
        break;

      case QEvent::Type::MouseMove:
        {
          QMouseEvent& me = *(static_cast<QMouseEvent*>(_event));
          if((me.localPos().x() < 0.0) || 
              (me.localPos().y() < 0.0) ||
              (me.localPos().x() > this->size().width()) ||
              (me.localPos().y() > this->size().height()))
          {
            return to_state_released();
          }
        }
        break;

      case QEvent::Type::KeyRelease:
        {
          QKeyEvent& ke = *(static_cast<QKeyEvent*>(_event));
          if((ke.key() == Qt::Key::Key_Space) && (!ke.isAutoRepeat()))
          {
            return to_state_released();
          }
        }
        break;

      default:
        break;
      }
      return EState::pressed;
    };

  switch(ld.state)
  {
  case EState::released:
    ld.state = state_released(_event);
    break;

  case EState::pressed:
    ld.state = state_pressed(_event);
    break;
  }

  if(!ret) QLabel::event(_event);

  return ret;
}

//------------------------------------------------------------------------------
void LCQPushLabel::setPushDelay(int _msec)
{
  if(_msec >= 0) ld.timer.setInterval(_msec);
}

