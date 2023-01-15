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

#include "lcxmllistwidgetbuilder.h"
#include "LIDataFormatter.h"
#include "widgets/lcqlistwidget.h"
#include "xmlcommon.h"
#include "LIApplication.h"
#include <QDomElement>

//------------------------------------------------------------------------------
static const struct
{
  QString icon      = "icon";
  QString data      = "data";
  QString text      = "text";
  QString matching  = "matching";
}__slAttributes;

static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
LCXmlListWidgetBuilder::LCXmlListWidgetBuilder()
{
}

LCXmlListWidgetBuilder::~LCXmlListWidgetBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlListWidgetBuilder::buildLocal(      
    const QDomElement& _element, const LIApplication& _app)
{

  auto ret_wrong = [](){return new QListWidget;};

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

  auto list_widget = new LCQListWidget(source, data_spec.dataId);

  //----------------------------------------------------------add_item[]
  auto add_item = [&list_widget, &format](const QDomNode& _node)
  {

    if(!_node.isElement()) return;
    QDomElement el = _node.toElement();

    QString attr_text = el.attribute(__slAttributes.text);
    if(attr_text.isNull()) return;

    QString attr_matching = el.attribute(__slAttributes.matching);

    if(attr_matching.isNull()) return;

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(attr_text);

    QString attr = el.attribute(__slAttributes.icon);

    QPixmap pixmap = parsePixmap(el.attribute(__slAttributes.icon));

    if(!pixmap.isNull())
    {
      item->setIcon(pixmap);
      if(
          (pixmap.size().width() > list_widget->iconSize().width()) || 
          (pixmap.size().height() > list_widget->iconSize().height()))
      {
        list_widget->setIconSize(pixmap.size());
      }
    }

    list_widget->addItem(item, format->toBytes(attr_matching)); 
  };

  for(QDomNode node = _element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node);
  }

  setWidgetName(      _element, list_widget);
  setWidgetStyle(     _element, list_widget);
  setWidgetSize(      _element, list_widget);
  setWidgetPosition(  _element, list_widget);
  setWidgetFixedSize( _element, list_widget);

  return list_widget;
}
