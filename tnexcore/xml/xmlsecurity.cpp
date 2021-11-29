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
#include "xmlsecurity.h"
#include "lcsecurity.h"
#include "xmlcommon.h"
#include "applicationinterface.h"

#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg)

static const struct
{
  QString security = "SECURITY";
}__slTags;

static const struct
{
  QString file = "file";
}__slAttributes;


namespace xmlsecurity
{
void upload(const QDomElement& _rootElement)
{
  auto end_upload = 
    []()
    {
      __smMessage("\n\t++++End deploy security.\n");
    };

  __smMessage("\n\t++++Begin deploy security.\n");
  QDomElement element = _rootElement.firstChildElement(__slTags.security);
  if(element.isNull()) 
  {
    __smMessage("\n\t++++Project has no security block.\n");
    end_upload();
    return;
  }
  QString attr_file = element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    element = xmlcommon::loadDomDocument(attr_file).documentElement();
    if(element.isNull()) { return end_upload(); }
  }

  LCSecurity::instance().init(element, CApplicationInterface::getInstance());
  end_upload();
}

LCSecurity& getSecurity()
{
  return LCSecurity::instance();
}

}
