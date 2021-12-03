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

#include "xmlglobalstyle.h"
#include "applicationinterface.h"

#include <QDomElement>
#include <QTextStream>
#include <QApplication>
#include <QDebug>

static const struct
{
  QString file = "file";
}__slAttributes;

static const struct
{
  QString rootTag = "GLOBALSTYLE"; 
}__slTags; 

namespace xmlglobalstyle
{

void upload(const QDomElement& _rootElement, QApplication& _qapp)
{
  auto element = _rootElement.firstChildElement(__slTags.rootTag);

  if(element.isNull()) 
  {
    return;
  }
  QString message("Load global style:\n");
  
  QString attr_file = element.attribute( __slAttributes.file);

  if(attr_file.isNull()) 
  {
    message += QStringLiteral("\tFile is not specified.\n");
    CApplicationInterface::getInstance()
      .warning(message);
    return;
  }

  auto style_file = 
    CApplicationInterface::getInstance().getFileDevice(attr_file);

  if(style_file.isNull()) return; 

  if(style_file->open(QIODevice::OpenModeFlag::ReadOnly))
  {
    _qapp.setStyleSheet(QLatin1String(style_file->readAll()));
    message += QString("\tStyle sheet from file '%1' is installed").arg(attr_file);
    CApplicationInterface::getInstance()
      .message(message);
  }
  else
  {
    message += QString("\tCan't open style sheet file '%1'").arg(attr_file);
    CApplicationInterface::getInstance()
      .warning(message);
  }
}

}
