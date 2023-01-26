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

#include "lcxmllcdnumberbuilder.h"
#include "lcqdatalcdnumber.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QDebug>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>

static const struct
{
  struct
  {
    QString attribute = "segmentStyle";
    struct
    {
      QString outline = "outline";
      QString filled  = "filled";
      QString flat    = "flat";
    }values;
  }segmentStyle;

  QString data = "data";
  QString posx = "posx";
  QString posy = "posy";
  QString width = "width";
  QString height = "height";
  QString fixedWidgh = "fixedWidth";
  QString fixedHeight = "fixedHeight";
  QString style = "style";
}__slAttributes;

//==============================================================================
LCXmlLcdNumberBuilder::LCXmlLcdNumberBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlLcdNumberBuilder::~LCXmlLcdNumberBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlLcdNumberBuilder::build(
    const QDomElement& _element,
    const LIApplication& _app)
{
   LCQDataLcdNumber* lcd_number = nullptr;
  //Get data specification.
  QString attr = _element.attribute(__slAttributes.data);
  if(attr.isNull())
  {
    lcd_number = new LCQDataLcdNumber();
  }


  return lcd_number;
}

