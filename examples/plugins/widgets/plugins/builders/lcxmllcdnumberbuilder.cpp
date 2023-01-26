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

  QString digitCount = "digitCount";
  QString data = "data";
  QString posx = "posx";
  QString posy = "posy";
  QString width = "width";
  QString height = "height";
  QString fixedWidth = "fixedWidth";
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

#include <QLabel>

//------------------------------------------------------------------------------
QWidget* LCXmlLcdNumberBuilder::build(
    const QDomElement& _element,
    const LIApplication& _app)
{
  Q_UNUSED(_element);
  Q_UNUSED(_app);

   LCQDataLcdNumber* lcd_number = new LCQDataLcdNumber();

  //Get data specification.
  {
    QString source_id;
    QString data_id;
    QString formatter_id;

    QString attr = _element.attribute(__slAttributes.data);

    if(attr.isNull())
    {
      return lcd_number;
    }
    bool flag = 
      _app.parseDataSpecification(attr,source_id, data_id, formatter_id);
    if(!flag) return lcd_number;
    auto source = _app.getDataSource(source_id);
    if(source.isNull()) return lcd_number;
    auto formatter = _app.getDataFormatter(formatter_id);
    if(formatter.isNull()) return lcd_number;
    lcd_number->init(data_id, source, formatter);
  }

  //Set segment style
  {
    QString segment_style = 
      _element.attribute(__slAttributes.segmentStyle.attribute);
    if(!segment_style.isNull())
    {
      if(segment_style == __slAttributes.segmentStyle.values.filled)
      {
        lcd_number->setSegmentStyle(QLCDNumber::Filled);
      } 
      else if(segment_style == __slAttributes.segmentStyle.values.flat)
      {
        lcd_number->setSegmentStyle(QLCDNumber::Flat);
      }
      else if(segment_style == __slAttributes.segmentStyle.values.outline)
      {
        lcd_number->setSegmentStyle(QLCDNumber::Outline);
      }
      else
      {
        _app.warning(
            QString("LCXmlLcdNumberBuilder: Wrong segment style '%1'.")
            .arg(segment_style));
      }
    }
  }


  auto attr_to_int = 
    [](const QString& _attr, int& _val)
    {
      bool flag = false;
      if(_attr.isNull()) return flag;
      int val = _attr.toInt(&flag);
      if(flag)
      {
        _val = val;
      }
      return flag;
    };

  //Set digit count
  {
    int dc;
    if(attr_to_int(_element.attribute(__slAttributes.digitCount), dc))
    {
      lcd_number->setDigitCount(dc);
    }
  }

  //Set position
  {
    int posx = lcd_number->pos().x();
    int posy = lcd_number->pos().y();
    attr_to_int(_element.attribute(__slAttributes.posx), posx);
    attr_to_int(_element.attribute(__slAttributes.posy), posy);
    lcd_number->move(posx, posy);
  }

  //Set size
  {
    int width;
    int height;
    if(
        attr_to_int(_element.attribute(__slAttributes.width), width) &&
        attr_to_int(_element.attribute(__slAttributes.height), height)
      )
    {
      lcd_number->resize(width, height);
    }
  }

  //Set fixed size
  {
    int width;
    int height;
    if( attr_to_int(_element.attribute(__slAttributes.fixedWidth), width))
    {
      lcd_number->setFixedWidth(width);
    }
    if( attr_to_int(_element.attribute(__slAttributes.fixedHeight), height))
    {
      lcd_number->setFixedHeight(height);
    }
  }

  //Set fixed size
  {
    int width;
    int height;
    if( attr_to_int(_element.attribute(__slAttributes.fixedWidth), width))
    {
      lcd_number->setFixedWidth(width);
    }
    if( attr_to_int(_element.attribute(__slAttributes.fixedHeight), height))
    {
      lcd_number->setFixedHeight(height);
    }
  }

  //Set style
  {
    QString style = _element.attribute(__slAttributes.style);
    if(!style.isNull())
    {
      _app.setWidgetUniqName(lcd_number);
      _app.setWidgetStyle(style, lcd_number);
    }
  }

  return lcd_number;
}

