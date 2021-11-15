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

//==============================================================================CEventFilter
class CEventFilter : public QObject
{
private:
  enum class EState
  {
    released,
    pressed
  };

  EState mState;
  QTimer* mpTimer;

  TChangeView  mSetViewPressed;
  TChangeView  mSetViewReleased;
  TActionsList mPressActions;
  TActionsList mReleaseActions;

public:
  explicit CEventFilter() = delete;
  explicit CEventFilter(
      TChangeView   _setViewPressed,
      TChangeView   _setViewReleased,
      TActionsList _pressActions, 
      TActionsList _releaseActions, 
      int _pushDelay,
      QObject* _parent) : 
    QObject(_parent)
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
  }

  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {
    bool ret = false;

    auto state_released = 
      [this, _event, &ret, _obj]()
      {

        auto act_press = 
          [this, &ret, _obj]()
          {
            ret = true;
            mpTimer->start();
            mSetViewPressed(static_cast<QLabel*>(_obj));
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
      [this, _event, &ret, _obj]()
      {
        auto act_release = 
          [this, &ret, _obj]()
          {
            ret = true;
            mState = EState::released;
            mSetViewReleased(static_cast<QLabel*>(_obj));

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
};

//==============================================================================
LCQPushLabel::LCQPushLabel(
      TChangeView _setViewPressed, 
      TChangeView _setViewReleased,
      const TActionsList& _actionsPush, 
      const TActionsList& _actionsRelease,
      int _pushDelay) : 
  QLabel(nullptr)
  /* ,mSetViewPush(_setViewPush) */
  /* ,mSetViewRelease(_setViewRelease) */
  /* ,mActionsPush(_actionsPush) */
  /* ,mActionsRelease(_actionsRelease) */
  /* ,mpTimer(new QTimer(this)) */
  /* ,mState(EState::released) */
{

  installEventFilter( 
      new CEventFilter(
        _setViewPressed, _setViewReleased,
        _actionsPush, _actionsRelease, 
        _pushDelay, this));
  _setViewReleased(this);
  /* mpTimer->setSingleShot(true); */
  /* mpTimer->setInterval(_pushDelay); */
  /* mSetViewRelease(this); */

  /* connect(mpTimer, &QTimer::timeout, */ 
  /*     [this]() */
  /*     { */
  /*       s_ActionsExecute(mActionsPush); */
  /*     }); */
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
}

/* //------------------------------------------------------------------------------ */
/* bool LCQPushLabel::event(QEvent* _event) */
/* { */
/*   bool ret = false; */
/*   //---------------------------------------state_released[] */
/*   auto state_released = */ 
/*     [this, &ret](QEvent* _event) */
/*     { */
/*       auto to_state_pressed = */ 
/*         [this, &ret]() */
/*         { */
/*           mSetViewPush(this); */
/*           if(mActionsPush.size() != 0) */
/*           { */
/*             mpTimer->start(); */
/*             ret = true; */
/*           } */
/*           return EState::pressed; */
/*         }; */

/*       switch(_event->type()) */
/*       { */
/*       case QEvent::Type::MouseButtonPress: */
/*       /1* case QEvent::Type::TouchBegin: *1/ */
/*           return to_state_pressed(); */
/*         break; */
/*       default: */
/*         break; */
/*       } */
/*       return EState::released; */
/*     }; */

/*   //---------------------------------------state_pressed[] */
/*   auto state_pressed = */ 
/*     [this, &ret](QEvent* _event) */
/*     { */
/*       auto to_state_released = */ 
/*         [this, &ret]() */
/*         { */
/*           mpTimer->stop(); */
/*           mSetViewRelease(this); */
/*           ret = s_ActionsExecute(mActionsRelease); */
/*           return EState::released; */
/*         }; */

/*       switch(_event->type()) */
/*       { */
/*       /1* case QEvent::Type::TouchEnd: *1/ */
/*       case QEvent::Type::MouseButtonRelease: */
/*           return to_state_released(); */
/*         break; */

/*       default: */
/*         break; */
/*       } */
/*       return EState::pressed; */
/*     }; */

/*   switch(mState) */
/*   { */

/*   case EState::released: */
/*     mState = state_released(_event); */
/*     break; */

/*   case EState::pressed: */
/*     mState = state_pressed(_event); */
/*     break; */
/*   } */

/*   if(!ret) QLabel::event(_event); */

/*   return ret; */
/* } */

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

