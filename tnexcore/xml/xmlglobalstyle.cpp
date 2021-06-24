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
#include <QFile>
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
    CApplicationInterface::getInstance()
      .messageDeploy("Upload global style: main document has no global style element.");
    return;
  }
  
  QString attr_file = element.attribute( __slAttributes.file);
  if(attr_file.isNull()) 
  {
    CApplicationInterface::getInstance()
      .messageDeploy(QString("Upload global style: the file is not specified."));
    return;
  }
  QFile style_file(attr_file); 
  if(style_file.open(QFile::OpenModeFlag::ReadOnly))
  {
    _qapp.setStyleSheet(QLatin1String(style_file.readAll()));
  }
  else
  {
    CApplicationInterface::getInstance()
      .messageDeploy(QString("Upload global style: file '%1' not find.")
          .arg(attr_file));
  }
}

}
