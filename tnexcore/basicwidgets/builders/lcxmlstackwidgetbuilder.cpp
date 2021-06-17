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

#include "lcxmlstackwidgetbuilder.h"
#include "widgets/lcqstackwidget.h"
#include "xmlcommon.h"
#include "LIApplication.h"
#include "LIDataFormatter.h"
#include <QDomElement>
#include <QDebug>

//------------------------------------------------------------------------------
static const struct
{
  QString data = "data";
  QString matching = "matching";
}__slAttributes;

static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlStackWidgetBuilder::LCXmlStackWidgetBuilder()
{
}

LCXmlStackWidgetBuilder::~LCXmlStackWidgetBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlStackWidgetBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  auto ret_wrong = [](){return new QStackedWidget;};

  bool err_flag = false;

  auto data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data),
      [&err_flag](const QString)
      {
        err_flag = true;
      });

  if(err_flag) return ret_wrong();


  auto source = _app.getDataSource(data_spec.sourceId);
  if(source.isNull()) return ret_wrong();

  auto format = _app.getDataFormatter(data_spec.formatterId);
  if(format.isNull()) return ret_wrong();


  auto stacked_widget = new LCQStackWidget(source, data_spec.dataId); 

  auto add_item = 
    [&_app, &stacked_widget, &format](const QDomNode& _node)
    {

      if(!_node.isElement()) return;
      QDomElement el = _node.toElement();

      QString attr_matching = el.attribute(__slAttributes.matching);
      if(attr_matching.isNull()) return;

      for(QDomNode node = el.firstChild(); 
          !node.isNull(); 
          node = node.nextSibling())
      {
        auto we = node.toElement();
        if(!we.isElement()) continue;
        auto builder = _app.getWidgetBuilder(we.tagName());
        if(builder.isNull()) continue;
        QWidget* widget = builder->build(we, _app);
        if(widget)
        {
          stacked_widget->addWidget(widget, format->toBytes(attr_matching));
          break;
        }
      }
    };

  for(QDomNode node = _element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node);
  }

  setWidgetName(      _element, stacked_widget);
  setWidgetStyle(     _element, stacked_widget);
  setWidgetSize(      _element, stacked_widget);
  setWidgetPosition(  _element, stacked_widget);
  setWidgetFixedSize( _element, stacked_widget);

  return stacked_widget;
}
