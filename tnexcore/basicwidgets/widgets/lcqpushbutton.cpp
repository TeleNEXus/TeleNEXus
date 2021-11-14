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
static void s_ActionsExecute(const TActionsList _actions)
{
  for(auto it = _actions.begin(); 
      it != _actions.end(); 
      it++)
  {
    (*it)();
  }
}

//==============================================================================
struct SLocalData
{
  QTimer timer;
  int counter = 0;

  TActionsList pushActions;
  TActionsList releaseActions;

  SLocalData()
  {
    timer.setSingleShot(true);
  }

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

  ld.pushActions = _pushActions;
  ld.releaseActions = _releaseActions;
  ld.timer.setInterval(_pushDelay);
  ld.timer.setSingleShot(true);

  connect(&(ld.timer), &QTimer::timeout, 
      [this]()
      {
        s_ActionsExecute(ld.pushActions);
      });
}




//------------------------------------------------------------------------------
LCQPushButton::~LCQPushButton()
{
  delete toLocalData(mpLocal);
}

//------------------------------------------------------------------------------
bool LCQPushButton::event(QEvent* _event)
{
  bool ret = false;
  switch(_event->type())
  {

  case QEvent::Type::TouchCancel:
    qDebug() << "Button touch cancel.";
    break;

  case QEvent::Type::TouchUpdate:
    qDebug() << "Button touch Update.";
    break;

  case QEvent::Type::TouchEnd:
    qDebug() << "Button touch End.";
    break;

  case QEvent::Type::TouchBegin:
    qDebug() << "Button touch Begin.";
    break;

  case QEvent::Type::KeyPress:
    qDebug() << "Button Key press";
    break;

  case QEvent::Type::KeyRelease:
    qDebug() << "Button Key release";
    break;

  case QEvent::Type::MouseButtonPress:
    ld.counter++;
    qDebug() << "Button mouse press." << ld.counter;
    if(ld.pushActions.size() == 0) break;
      ld.timer.start();
      ret = true;
    break;

  case QEvent::Type::MouseButtonRelease:
      qDebug() << "Button mouse release."<< ld.counter;
      ld.timer.stop();
      if(ld.pushActions.size() != 0) 
      {
        s_ActionsExecute(ld.releaseActions);
        ret = true;
      }

      break;

  /* case QEvent::Type::TouchUpdate: */
  /* case QEvent::Type::TouchBegin: */
  /*   if(ld.pushActions.size() == 0) break; */
  /*   ld.timer.start(); */
  /*   ret = true; */
  /*   break; */

  /* case QEvent::Type::TouchEnd: */
  /*   ld.timer.stop(); */
  /*   if(ld.pushActions.size() != 0) */ 
  /*   { */
  /*     s_ActionsExecute(ld.releaseActions); */
  /*     ret = true; */
  /*   } */
  /*   break; */

  default:
    break;
  }

  QPushButton::event(_event); 
  return  ret;
}

