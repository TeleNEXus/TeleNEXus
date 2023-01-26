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
#include "lcxmltabwidgetbuilder.h"
#include "LIApplication.h"
#include "xmlcommon.h"
#include <QTabWidget>
#include <QDomElement>
#include <QDebug>
#include <qdom.h>
#include <qtablewidget.h>

//------------------------------------------------------------------------------
static const struct
{
  QString side = "side";
  QString text = "text";
  QString icon = "icon";
  QString styleBar = "styleBar";
}__slAttributes;
static const struct
{
  QString top     = "Top";
  QString bottom  = "Bottom";
  QString left    = "Left";
  QString right   = "Right";
}__slSideVals;
static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlTabWidgetBuilder::LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlTabWidgetBuilder::~LCXmlTabWidgetBuilder()
{
}

//------------------------------------------------------------------------------
static void buildTab(
    const QDomElement& _element, 
    QTabWidget* tabwidget, 
    int _tabindex, 
    const LIApplication& _app);

QWidget* LCXmlTabWidgetBuilder::buildLocal(      
    const QDomElement& _element, const LIApplication& _app) 
{
  QTabWidget* tabwidget = new QTabWidget;

  auto get_side =
    [&_element]()
    {
      auto side = QTabWidget::TabPosition::North;
      QString attr_side = _element.attribute(__slAttributes.side);
      if(attr_side == __slSideVals.bottom) side = QTabWidget::TabPosition::South;
      else if(attr_side == __slSideVals.left) side = QTabWidget::TabPosition::West;
      else if(attr_side == __slSideVals.right) side = QTabWidget::TabPosition::East;
      return side;
    };

  tabwidget->setTabPosition(get_side());


  for( auto el = _element.firstChildElement(__slTags.item);
      !el.isNull();
      el = el.nextSiblingElement(__slTags.item))
  {
    int index = 0;
    buildTab(el, tabwidget, index, _app); 
    index++;
  }

  setWidgetName(tabwidget);
  setWidgetStyle(     _element, tabwidget);

  setWidgetStyle(
      QString("%1 %2")
      .arg(tabwidget->styleSheet())
      .arg(_element.attribute(__slAttributes.styleBar)),
      tabwidget,
      QString("QTabWidget#%1 > %2").arg(tabwidget->objectName()).arg("QTabBar"));

  setWidgetSize(      _element, tabwidget);
  setWidgetPosition(  _element, tabwidget);
  setWidgetFixedSize( _element, tabwidget);

  return tabwidget;
}

//------------------------------------------------------------------------------
static void buildTab(
    const QDomElement& _element, 
    QTabWidget* _tabwidget, 
    int _tabindex, 
    const LIApplication& _app)
{
  QString attr_text = _element.attribute(__slAttributes.text);
  QIcon icon;
  QPixmap pixmap = LCXmlBuilderBase::parsePixmap(_element.attribute(__slAttributes.icon));
  if(!pixmap.isNull())
  {
    icon.addPixmap(pixmap);
    if((pixmap.size().width() > _tabwidget->iconSize().width()) ||
        (pixmap.size().height() > _tabwidget->iconSize().height()))
    {
      _tabwidget->setIconSize(pixmap.size());
    }
  }

  if(attr_text.isNull() && icon.isNull())
  {
    attr_text = QString("Tab %1").arg(_tabindex);
  }

  QDomNodeList childs = _element.childNodes(); 

  for (auto node = _element.firstChild();
      !node.isNull(); node = node.nextSibling())
  {
    if(!node.isElement()) continue;
    QDomElement el = node.toElement(); 
    QSharedPointer<LIXmlWidgetBuilder> builder = _app.getWidgetBuilder(el.tagName());
    if(!builder.isNull())
    {
      QWidget* widget = (*builder).build( el, _app);
      if (widget)
      {
        _tabwidget->addTab( widget, icon, attr_text);
        return;
      }
    }
  }

  _tabwidget->addTab( new QWidget, icon, attr_text);
}
