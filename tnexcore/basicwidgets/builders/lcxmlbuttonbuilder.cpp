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

#include <QPushButton>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>

//==============================================================================
static const struct
{
  QString press = "press";
  QString release = "release";
}__slTags;

static const struct
{
  QString text="text";
}__slAttributes;

//==============================================================================LCXmlButtonBuilder
LCXmlButtonBuilder::LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}

using TActions = LCXmlStdActionBuilder::TActions;

//------------------------------------------------------------------------------
QWidget* LCXmlButtonBuilder::buildLocal(
const QDomElement& _element, const LIApplication& _app)
{

  QString attr_text = _element.attribute(__slAttributes.text);
  if(attr_text.isNull())
  {
    attr_text = QStringLiteral("Button");
  }

  QPushButton* button = new QPushButton(attr_text);


  TActions actions_pressed = LCXmlStdActionBuilder::instance().build(
      _element.firstChildElement(__slTags.press), _app);


  TActions actions_released = LCXmlStdActionBuilder::instance().build(
      _element.firstChildElement(__slTags.release), _app);


  if(actions_pressed.size() > 0)
  {
    QObject::connect(button, &QPushButton::pressed,
        [actions_pressed]()
        {
          for(auto it = actions_pressed.begin(); it != actions_pressed.end(); it++)
          {
            (*it)();
          }
        });
  }

  if(actions_released.size() > 0)
  {
    QObject::connect(button, &QPushButton::released,
        [actions_released]()
        {
          for(auto it = actions_released.begin(); it != actions_released.end(); it++)
          {
            (*it)();
          }
        });
  }

  auto pixmap = parsePixmap(_element.attribute(QStringLiteral("icon")));

  if(!pixmap.isNull())
  {
    button->setIconSize(pixmap.size());
    button->setIcon(pixmap);
  }

  setWidgetName(      _element, button);
  setWidgetStyle(     _element, button);
  setWidgetSize(      _element, button);
  setWidgetPosition(  _element, button);
  setWidgetFixedSize( _element, button);
  return button;
}


