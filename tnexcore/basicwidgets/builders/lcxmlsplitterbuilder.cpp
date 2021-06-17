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
#include "lcxmlsplitterbuilder.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QSplitter>
#include <QDebug>

//==============================================================================
LCXmlSplitterBuilder::LCXmlSplitterBuilder(Qt::Orientation _orientation) :
  mOrientation(_orientation)
{
}

//------------------------------------------------------------------------------
LCXmlSplitterBuilder::~LCXmlSplitterBuilder()
{

}

//------------------------------------------------------------------------------
QWidget* LCXmlSplitterBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{
  QSplitter* splitter = new QSplitter(mOrientation);

  for(QDomNode node = _element.firstChild(); !node.isNull();
      node = node.nextSibling())
  {
    auto el =  node.toElement();
    if(el.isNull()) continue;
    auto builder = _app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;
    QWidget* widget = builder->build(el, _app);
    if(widget)
    {
      splitter->addWidget(widget);
    }
  }

  setWidgetName(      _element, splitter);
  setWidgetStyle(     _element, splitter);
  setWidgetSize(      _element, splitter);
  setWidgetPosition(  _element, splitter);
  setWidgetFixedSize( _element, splitter);

  return splitter;
}

