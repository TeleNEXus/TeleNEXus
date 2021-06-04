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
#include "lcxmlscrollareabuilder.h"
#include "widgetbuilderscommon.h"

#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"

#include <QScrollArea>
#include <QDomElement>
#include <QDebug>

//==============================================================================
LCXmlScrollAreaBuilder::LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlScrollAreaBuilder::~LCXmlScrollAreaBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlScrollAreaBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QWidget* widget = nullptr;

  for(QDomNode node = element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    auto builder = app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;
    QWidget* w = builder->build(el, app);

    if(w)
    {
      widget = w;
      break;
    }
  }

  QScrollArea *scrollarea = new QScrollArea;
  if(widget)
  {
    scrollarea->setWidget(widget);
  }
  else
  {
    scrollarea->setWidget(new QWidget);
  }

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);
  style = QString(".QScrollArea { %1 }").arg(style);
  scrollarea->setStyleSheet(style);

  return scrollarea;
}


