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
#include "lcxmllabelbuilder.h"
#include "LIApplication.h"
#include <QLabel>
#include <QDomElement>

//==============================================================================
static const struct
{
  QString text = "text";
}__slAttributes;
//==============================================================================
LCXmlLabelBuilder::LCXmlLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlLabelBuilder::~LCXmlLabelBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlLabelBuilder::buildLocal(
      const QDomElement& _element, const LIApplication& _app) 
{
  Q_UNUSED(_app);

  QLabel* label = new QLabel();

  QString text = _element.attribute(__slAttributes.text);

  if(text.isNull())
  {
    label->setText(QStringLiteral("Label"));
  }
  else
  {
    label->setText(text);
  }


  setWidgetName(_element, label);
  setWidgetStyle(_element, label);
  setWidgetSize(_element, label);
  setWidgetFixedSize(_element, label);
  setWidgetPosition(_element, label);

  return label;
}
