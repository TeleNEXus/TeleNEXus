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

#include "lcxmlbuttonbuilder.h"
#include "LIDataFormatter.h"
#include "LIApplication.h"
#include "xmlcommon.h"
#include "lcxmlstdactionbuilder.h"
#include "lcqpushbutton.h"
#include "lcsecurity.h"

/* #include <QPushButton> */
#include <QSharedPointer>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>
#include <QTimer>

//==============================================================================
static const struct
{
  QString press = "press";
  QString release = "release";
}__slTags;

static const struct
{
  QString icon="icon";
  QString text="text";
  QString pushDelay = "delay";
}__slAttributes;

using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================LCXmlButtonBuilder
LCXmlButtonBuilder::LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlButtonBuilder::buildLocal(
const QDomElement& _element, const LIApplication& _app)
{

  TActions actions_pressed;
  TActions actions_released;
  int push_delay = 0;
  QDomElement el_press = _element.firstChildElement(__slTags.press);
  if(!el_press.isNull())
  {
    actions_pressed = LCXmlStdActionBuilder::instance().build(
        _element.firstChildElement(__slTags.press), _app);
    bool flag = false;
    push_delay = el_press.attribute(__slAttributes.pushDelay).toInt(&flag);
    if(!flag) push_delay = 0;
  }

  actions_released = LCXmlStdActionBuilder::instance().build(
      _element.firstChildElement(__slTags.release), _app);


  auto pixmap = parsePixmap(_element.attribute(__slAttributes.icon));


  QString attr = _element.attribute(__slAttributes.text);
  LCQPushButton* button = new LCQPushButton(
      attr, 
      actions_pressed, 
      actions_released,
      push_delay);

  if(!pixmap.isNull())
  {
    button->setIconSize(pixmap.size());
    button->setIcon(pixmap);
  }

  setWidgetName(button);
  setWidgetStyle(     _element, button);
  setWidgetSize(      _element, button);
  setWidgetPosition(  _element, button);
  setWidgetFixedSize( _element, button);

  QSet<QEvent::Type> events;

  events 
    << QEvent::Type::MouseButtonPress
    << QEvent::Type::MouseButtonDblClick
    << QEvent::Type::TouchBegin;


  QObject* security_filter =  
    LCSecurity::instance().createEventFilter(_element, events);
  if(security_filter != nullptr)
  {
    button->installEventFilter(security_filter);
  }

  return button;
}

