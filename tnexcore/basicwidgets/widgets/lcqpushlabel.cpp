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

//==============================================================================
LCQPushLabel::LCQPushLabel(QWidget* _widget) :
  QLabel(_widget)
{
}

//------------------------------------------------------------------------------
LCQPushLabel::~LCQPushLabel()
{
}

//------------------------------------------------------------------------------
bool LCQPushLabel::event(QEvent* _event)
{
  switch(_event->type())
  {
  case QEvent::Type::MouseButtonPress:
    if(static_cast<QMouseEvent*>(_event)->button() == 
        Qt::MouseButton::LeftButton)
    {
      emit press(this);
    }
    break;

  case QEvent::Type::MouseButtonRelease:
    if(static_cast<QMouseEvent*>(_event)->button() == 
        Qt::MouseButton::LeftButton)
    {
      emit release(this);
    }
    break;

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
