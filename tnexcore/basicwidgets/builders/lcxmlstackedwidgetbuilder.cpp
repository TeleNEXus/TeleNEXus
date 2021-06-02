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

#include "lcxmlstackedwidgetbuilder.h"
#include "widgets/lcqstackedwidget.h"
#include "LIApplication.h"
#include "lcbuilderscommon.h"
#include <QDomElement>
#include <QDebug>

//------------------------------------------------------------------------------
static const struct
{
  QString id = "id";
}__slAttributes;

static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlStackedWidgetBuilder::LCXmlStackedWidgetBuilder()
{
}

LCXmlStackedWidgetBuilder::~LCXmlStackedWidgetBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlStackedWidgetBuilder::buildLocal( 
    QSharedPointer<SBuildData> _buildData)
{

  auto ret_wrong = [](){return new QStackedWidget;};

  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  auto source = [&element, &app]()
  {
    QString attr = element.attribute(LCBuildersCommon::mAttributes.source);
    if(attr.isNull()) return QSharedPointer<LIRemoteDataSource>();
    return app.getDataSource(attr);
  }();

  if(source.isNull()) return ret_wrong();

  QString data = element.attribute(LCBuildersCommon::mAttributes.data);
  if(data.isNull()) return ret_wrong();

  auto format = _buildData->application.getDataFormatter(element.attribute(
        LCBuildersCommon::mAttributes.dataformatter));

  if(format.isNull()) return ret_wrong();

  auto stacked_widget = new LCQStackedWidget(source, data, format); 

  auto add_item = 
    [&app, &stacked_widget](const QDomNode& _node)
    {

      if(!_node.isElement()) return;
      QDomElement el = _node.toElement();

      QString attr_id = el.attribute(__slAttributes.id);
      if(attr_id.isNull()) return;

      for(QDomNode node = el.firstChild(); 
          !node.isNull(); 
          node = node.nextSibling())
      {
        auto we = node.toElement();
        if(!we.isElement()) continue;
        auto builder = app.getWidgetBuilder(we.tagName());
        if(builder.isNull()) continue;
        QWidget* widget = builder->build(we, app);
        if(widget)
        {
          stacked_widget->addWidget(widget, attr_id);
          break;
        }
      }
    };

  for(QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node);
  }

  return stacked_widget;
}
