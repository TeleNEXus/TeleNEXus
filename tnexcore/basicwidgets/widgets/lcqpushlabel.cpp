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
struct SLocalData
{
  QTimer timer;
  SLocalData()
  {
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
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
  delete toLocalData(mpLocal);
}

//------------------------------------------------------------------------------
bool LCQPushLabel::event(QEvent* _event)
{

  switch(_event->type())
  {
  case QEvent::Type::MouseButtonDblClick:
  case QEvent::Type::MouseButtonPress:
    if(static_cast<QMouseEvent*>(_event)->button() == 
        Qt::MouseButton::LeftButton)
    {
      ld.timer.start();
      return true;
    }
    break;

  case QEvent::Type::MouseButtonRelease:
    if(static_cast<QMouseEvent*>(_event)->button() == 
        Qt::MouseButton::LeftButton)
    {
      if(!(ld.timer.isActive()))
      {
        emit release(this);
      }
      ld.timer.stop();
    }
    break;

  /* case QEvent::Type::MouseMove: */
  /*   { */
  /*     QMouseEvent* me = static_cast<QMouseEvent*>(_event); */
  /*     if((me->localPos().x() < 0.0) || */ 
  /*         (me->localPos().y() < 0.0) || */
  /*         (me->localPos().x() > this->size().width()) || */
  /*         (me->localPos().y() > this->size().height())) */
  /*     { */
  /*       emit release(this); */
  /*     } */
  /*   } */
  /*   break; */

  case QEvent::Type::Show:
    emit shown(this);
    break;

  case QEvent::Type::Hide:
    emit hidden(this);
    break;

  default:
    break;
  }
  return QLabel::event(_event);
}

//------------------------------------------------------------------------------
void LCQPushLabel::setPushDelay(int _msec)
{
  if(_msec >= 0) ld.timer.setInterval(_msec);
}
