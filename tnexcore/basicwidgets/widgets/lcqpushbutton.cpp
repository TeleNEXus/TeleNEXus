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
LCQPushButton::LCQPushButton(QWidget* _parent) :
  QPushButton(_parent),
  mpLocal(new SLocalData())
{
  connect(&(ld.timer), &QTimer::timeout, 
      [this]()
      {
        static QMouseEvent event = QMouseEvent(
            QEvent::Type::MouseButtonPress,
            QPointF(),
            QPointF(),
            Qt::MouseButton::LeftButton,
            Qt::MouseButtons(),
            Qt::KeyboardModifiers()
            );
        QPushButton::event(&event);
      });
}

//------------------------------------------------------------------------------
LCQPushButton::LCQPushButton(const QString& _text, QWidget* _parent) :
  LCQPushButton(_parent)
{
  setText(_text);
}

//------------------------------------------------------------------------------
LCQPushButton::~LCQPushButton()
{
  delete toLocalData(mpLocal);
}

//------------------------------------------------------------------------------
bool LCQPushButton::event(QEvent* _event)
{
  switch(_event->type())
  {
  case QEvent::Type::MouseButtonDblClick:
  case QEvent::Type::MouseButtonPress:
    if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
    {
      if(!ld.timer.isActive()) ld.timer.start();
      return true;
    }
    break;

  case QEvent::Type::MouseButtonRelease:
    if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
    {
      ld.timer.stop();
    }
    break;

  case QEvent::Type::TouchBegin:
      ld.timer.start();
      return true;

  case QEvent::Type::TouchEnd:
      ld.timer.stop();
      break;

    break;
  default:
    break;
  }
  return QPushButton::event(_event); 
}

//------------------------------------------------------------------------------
void LCQPushButton::setPushDelay(int _msec)
{
  if(_msec >= 0) ld.timer.setInterval(_msec);
}
