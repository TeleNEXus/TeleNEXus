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
#include <QEvent>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QKeyEvent>
#include <qnamespace.h>
#include <QLabel>

//------------------------------------------------------------------------------
static const struct
{
  QString data = "data";
  QString matching = "matching";
}__slAttributes;

static const struct
{
  QString item = "item";
  QString itemUndef = "itemUndef";
  QString itemWrong = "itemWrong";
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
  auto ret_wrong = [](){
    auto widget = new QStackedWidget();
    auto label = new QLabel(QStringLiteral("StatckedWidget"), widget);
    widget->addWidget(label);
    return widget;
  };

  bool err_flag = false;

  auto data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data),
      [&err_flag](const QString)
      {
        err_flag = true;
      });

  if(err_flag) return ret_wrong();

  auto source = _app.getDataSource(data_spec.sourceId);
  auto format = _app.getDataFormatter(data_spec.formatterId);

  LCQStackWidget* stacked_widget = nullptr;

  if(source.isNull() || format.isNull())
  {
    return nullptr;
  }
  else
  {
    stacked_widget = new LCQStackWidget(source, data_spec.dataId, format); 
  }


  auto add_widget = 
    [&_app](
        const QDomElement _element,
        const QString& _matching,
        std::function<void(QWidget* _widget, 
          const QString& _matching)> _adder)
    {

      QWidget* widget = nullptr;

      for(auto node = _element.firstChild(); 
          !node.isNull(); 
          node = node.nextSibling())
      {
        auto we = node.toElement();
        if(!we.isElement()) continue;
        auto builder = _app.getWidgetBuilder(we.tagName());
        if(builder.isNull()) continue;
        widget = builder->build(we, _app);
        if(widget)
        {
          _adder(widget, _matching);
          break;
        }
      }

      if(widget == nullptr)
      {
        _adder(new QLabel(QString("No widget")), _matching);
      }
    };

  auto add_item = 
    [&stacked_widget](QWidget* _widget, const QString& _matching)
    {
      stacked_widget->addWidget(_widget, _matching);
    };

  //add items
  for(QDomNode node = _element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    auto el = node.toElement();
    if(el.isNull()) continue;
    QString attr_matching = el.attribute(__slAttributes.matching);
    if(attr_matching.isNull()) continue;
    auto matching_bytes = format->toBytes(attr_matching);
    if(matching_bytes.isNull()) continue;
    auto matching_string = format->toString(matching_bytes);
    if(matching_string.isNull()) continue;
    add_widget(el, matching_string, add_item);
  }

  //add item undef
  [&_element, &add_widget, &stacked_widget]()
  {
    auto el = _element.firstChildElement(__slTags.itemUndef);
    if(el.isNull()) return;
    add_widget(el, QString(), 
        [&stacked_widget](QWidget* _widget, const QString)
        {
          stacked_widget->addWidgetUndef(_widget);
        });
  }();

  //add item wrong 
  [&_element, &add_widget, &stacked_widget]()
  {
    auto el = _element.firstChildElement(__slTags.itemWrong);
    if(el.isNull()) return;
    add_widget(el, QString(), 
        [&stacked_widget](QWidget* _widget, const QString)
        {
          stacked_widget->addWidgetWrong(_widget);
        });
  }();

  setWidgetName(      _element, stacked_widget);
  setWidgetStyle(     _element, stacked_widget);
  setWidgetSize(      _element, stacked_widget);
  setWidgetPosition(  _element, stacked_widget);
  setWidgetFixedSize( _element, stacked_widget);

  return stacked_widget;
}
