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

#include "lcqpushbutton.h"
#include <QMouseEvent>
#include <QTimer>
#include <qnamespace.h>
#include <QDebug>


//==============================================================================
using TActionsList = LCQPushButton::TActionsList;

//==============================================================================
static void s_ActionsExecute(const TActionsList _actions);

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

  TActionsList mPressActions;
  TActionsList mReleaseActions;

public:
  explicit CEventFilter() = delete;
  explicit CEventFilter(
      TActionsList _pressActions, 
      TActionsList _releaseActions, 
      int _pushDelay,
      QObject* _parent) : 
    QObject(_parent)
    ,mState(EState::released)
    ,mpTimer(new QTimer(this))
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
          [this, _obj, &ret]()
          {
            ret = true;
            mpTimer->start();
            (static_cast<QPushButton*>(_obj))->setDown(true);
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
          [this, _obj, &ret]()
          {

            ret = true;
            (static_cast<QPushButton*>(_obj))->setDown(false);
            mState = EState::released;

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
struct SLocalData
{
  /* QTimer timer; */
  /* int counter = 0; */

  /* TActionsList pushActions; */
  /* TActionsList releaseActions; */

  /* SLocalData() */
  /* { */
  /*   timer.setSingleShot(true); */
  /* } */

};

#define toLocalData(p) (static_cast<SLocalData*>(p))

#define ld (*(toLocalData(mpLocal)))
//==============================================================================

LCQPushButton::LCQPushButton(
    const QString& _text,
    const TActionsList& _pushActions,
    const TActionsList& _releaseActions,
    int _pushDelay) :
  QPushButton(_text),
  mpLocal(new SLocalData())
{

  setAttribute(Qt::WidgetAttribute::WA_AcceptTouchEvents, true);


  installEventFilter(
      new CEventFilter(_pushActions, _releaseActions, _pushDelay, this));

  /* ld.pushActions = _pushActions; */
  /* ld.releaseActions = _releaseActions; */
  /* ld.timer.setInterval(_pushDelay); */
  /* ld.timer.setSingleShot(true); */

  /* connect(&(ld.timer), &QTimer::timeout, */ 
  /*     [this]() */
  /*     { */
  /*       s_ActionsExecute(ld.pushActions); */
  /*     }); */
}




//------------------------------------------------------------------------------
LCQPushButton::~LCQPushButton()
{
  delete toLocalData(mpLocal);
}

/* //------------------------------------------------------------------------------ */
/* bool LCQPushButton::event(QEvent* _event) */
/* { */
/*   bool ret = false; */
/*   switch(_event->type()) */
/*   { */

/*   case QEvent::Type::TouchCancel: */
/*     qDebug() << "Button touch cancel."; */
/*     break; */

/*   case QEvent::Type::TouchUpdate: */
/*     qDebug() << "Button touch Update."; */
/*     break; */

/*   case QEvent::Type::TouchEnd: */
/*     qDebug() << "Button touch End."; */
/*     break; */

/*   case QEvent::Type::TouchBegin: */
/*     qDebug() << "Button touch Begin."; */
/*     break; */

/*   case QEvent::Type::KeyPress: */
/*     qDebug() << "Button Key press"; */
/*     break; */

/*   case QEvent::Type::KeyRelease: */
/*     qDebug() << "Button Key release"; */
/*     break; */

/*   case QEvent::Type::MouseButtonPress: */
/*     ld.counter++; */
/*     qDebug() << "Button mouse press." << ld.counter; */
/*     if(ld.pushActions.size() == 0) break; */
/*       ld.timer.start(); */
/*       ret = true; */
/*     break; */

/*   case QEvent::Type::MouseButtonRelease: */
/*       qDebug() << "Button mouse release."<< ld.counter; */
/*       ld.timer.stop(); */
/*       if(ld.pushActions.size() != 0) */ 
/*       { */
/*         s_ActionsExecute(ld.releaseActions); */
/*         ret = true; */
/*       } */

/*       break; */

/*   /1* case QEvent::Type::TouchUpdate: *1/ */
/*   /1* case QEvent::Type::TouchBegin: *1/ */
/*   /1*   if(ld.pushActions.size() == 0) break; *1/ */
/*   /1*   ld.timer.start(); *1/ */
/*   /1*   ret = true; *1/ */
/*   /1*   break; *1/ */

/*   /1* case QEvent::Type::TouchEnd: *1/ */
/*   /1*   ld.timer.stop(); *1/ */
/*   /1*   if(ld.pushActions.size() != 0) *1/ */ 
/*   /1*   { *1/ */
/*   /1*     s_ActionsExecute(ld.releaseActions); *1/ */
/*   /1*     ret = true; *1/ */
/*   /1*   } *1/ */
/*   /1*   break; *1/ */

/*   default: */
/*     break; */
/*   } */

/*   QPushButton::event(_event); */ 
/*   return  ret; */
/* } */

//==============================================================================
static void s_ActionsExecute(const TActionsList _actions)
{
  for(auto it = _actions.begin(); 
      it != _actions.end(); 
      it++)
  {
    (*it)();
  }
}
