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
#include "lcxmltextlabelbuilder.h"
#include "widgetbuilderscommon.h"
#include "LIApplication.h"
#include <QLabel>
#include <QDomElement>
#include <QDebug>
//==============================================================================
LCXmlTextLabelBuilder::LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlTextLabelBuilder::~LCXmlTextLabelBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlTextLabelBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData) 
{

  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QLabel* label = new QLabel();

  //Задание текста в виде атрибута.
  if(!element.attribute("text").isNull())
  {
    label->setText(element.attribute("text"));
  }

  QString style;
  style = LCBuildersCommon::getBaseStyleSheet(element, app);
  label->setStyleSheet(style);

  LCBuildersCommon::initPosition(element, *label);

  return label;
}
