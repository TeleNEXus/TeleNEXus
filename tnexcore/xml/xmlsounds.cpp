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

#include "lcsounds.h"
#include "xmlsounds.h"
#include "xmlcommon.h"
#include "applicationinterface.h"

#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

#define __smMessage(msg) CApplicationInterface::getInstance().message(msg, LIApplication::EMessageType::Deploy)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg, LIApplication::EMessageType::Deploy)

static const struct
{
  QString sounds = "SOUNDS";
}__slTags;

static const struct
{
  QString file = "file";
}__slAttributes;


namespace xmlsounds
{
void upload(const QDomElement& _rootElement)
{
  auto end_upload = 
    []()
    {
      __smMessage("\n\tEnd deploy sounds.\n");
    };

  __smMessage( "\n\tBegin deploy sounds.\n");

  QDomElement element = _rootElement.firstChildElement(__slTags.sounds);

  if(element.isNull()) 
  {
    __smMessage("\n\tProject has no sounds block.\n");
    return end_upload();
  }

  auto builder = 
    [](const QDomElement& _element)
    {
      LCSounds::instance().addSounds(_element);
    };

  CApplicationInterface::getInstance().buildFromFile(element, builder); 

  end_upload();
}

}
