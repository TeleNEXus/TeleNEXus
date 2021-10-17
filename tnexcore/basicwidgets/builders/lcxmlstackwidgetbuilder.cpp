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
#include "lcxmlstdactionbuilder.h"
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


  QString pushDelay = "pushDelay";
  QString data = "data";
  QString matching = "matching";


}__slAttributes;

static const struct
{
  QString actions = "actions";
  QString press   = "press";
  QString release = "release";
  QString item = "item";
  QString itemUndef = "itemUndef";
  QString itemWrong = "itemWrong";
} __slTags;

//==============================================================================
using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================
void performActions(const TActions& _actions)
{
  for(auto it = _actions.begin(); it != _actions.end(); it++)
  {
    (*it)();
  }
}

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
  /* if(source.isNull()) return ret_wrong(); */

  auto format = _app.getDataFormatter(data_spec.formatterId);
  /* if(format.isNull()) return ret_wrong(); */


  LCQStackWidget* stacked_widget = nullptr;

  if(source.isNull() || format.isNull())
  {
    stacked_widget = new LCQStackWidget(); 
  }
  else
  {
    stacked_widget = new LCQStackWidget(source, data_spec.dataId); 
  }

  
  TActions actions_press;
  TActions actions_release;

  [&_element, &actions_press, &actions_release, &_app, &stacked_widget]()
  {
    auto actions_element = _element.firstChildElement(__slTags.actions);
    if(actions_element.isNull()) return;

    actions_press = LCXmlStdActionBuilder::instance().
      build( actions_element.firstChildElement(__slTags.press), _app);

    actions_release = LCXmlStdActionBuilder::instance().build(
        actions_element.firstChildElement(__slTags.release), _app);

    QString attr_push_delay = 
      actions_element.attribute(__slAttributes.pushDelay);

    if(!attr_push_delay.isNull())
    {
      bool flag = false;
      int delay = attr_push_delay.toInt(&flag);
      if(flag)
      {
        stacked_widget->setPushDelay(delay);
      }
    }
  }();

  if(actions_press.size() != 0)
  {
    QObject::connect(stacked_widget, &LCQStackWidget::press,
        [actions_press]()
        {
          performActions(actions_press);
        });
  }

  if(actions_release.size() != 0)
  {
    QObject::connect(stacked_widget, &LCQStackWidget::release,
        [actions_release]()
        {
          performActions(actions_release);
        });
  }

  auto add_widget = 
    [&_app](
        const QDomElement _element,
        const QByteArray& _matching,
        std::function<void(QWidget* _widget, 
          const QByteArray& _matching)> _adder)
    {

      for(auto node = _element.firstChild(); 
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
          _adder(widget, _matching);
          break;
        }
      }
    };

  auto add_item = 
    [&stacked_widget](QWidget* _widget, const QByteArray& _matching)
    {
      stacked_widget->addWidget(_widget, _matching);
    };


  //add item undef
  [&_element, &add_widget, &stacked_widget]()
  {
    auto el = _element.firstChildElement(__slTags.itemUndef);
    if(el.isNull()) return;
    add_widget(el, QByteArray(), 
        [&stacked_widget](QWidget* _widget, const QByteArray)
        {
          stacked_widget->addWidgetUndef(_widget);
        });
  }();

  //add item wrong 
  [&_element, &add_widget, &stacked_widget]()
  {
    auto el = _element.firstChildElement(__slTags.itemWrong);
    if(el.isNull()) return;
    add_widget(el, QByteArray(), 
        [&stacked_widget](QWidget* _widget, const QByteArray)
        {
          stacked_widget->addWidgetWrong(_widget);
        });
  }();


  //add items
  for(QDomNode node = _element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    auto el = node.toElement();
    if(el.isNull()) continue;
    QString attr_matching = el.attribute(__slAttributes.matching);
    if(attr_matching.isNull()) continue;
    auto matching = format->toBytes(attr_matching);
    if(matching.isNull()) continue;
    add_widget(el, matching, add_item);
  }

  setWidgetName(      _element, stacked_widget);
  setWidgetStyle(     _element, stacked_widget);
  setWidgetSize(      _element, stacked_widget);
  setWidgetPosition(  _element, stacked_widget);
  setWidgetFixedSize( _element, stacked_widget);

  return stacked_widget;
}
