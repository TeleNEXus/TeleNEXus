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
#include "widgets/lcqlistwidget.h"
#include "widgetbuilderscommon.h"
#include "LIApplication.h"
#include <QDomElement>

//------------------------------------------------------------------------------
static const struct
{
  QString text = "text";
  QString id = "id";
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
    QSharedPointer<SBuildData> _buildData)
{

  auto ret_wrong = [](){return new QListWidget;};

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

  auto format = app.getDataFormatter(element.attribute(
        LCBuildersCommon::mAttributes.dataformatter));

  if(format.isNull()) return ret_wrong();

  auto list_widget = new LCQListWidget(source, data, format);

  int icon_size = -1;

  QString attr = element.attribute(LCBuildersCommon::mAttributes.iconsize);
  if(!attr.isNull())
  {
    bool flag = 0;
    icon_size = attr.toInt(&flag);
    if(flag)
    {
      list_widget->setIconSize(QSize(icon_size, icon_size));
    }
    else
    {
      icon_size = -1;
    }
  }

  QString style_sheet = 
    LCBuildersCommon::getBaseStyleSheet(element, app);

  list_widget->setStyleSheet(style_sheet);

  auto add_item = [&app, &list_widget](const QDomNode& _node, int _iconSize)
  {

    if(!_node.isElement()) return;
    QDomElement el = _node.toElement();

    QString attr_text = el.attribute(__slAttributes.text);
    if(attr_text.isNull()) return;

    QString attr_val = el.attribute(__slAttributes.id);
    if(attr_val.isNull()) return;


    QListWidgetItem *item = new QListWidgetItem;
    item->setText(attr_text);

    QString attr = el.attribute(LCBuildersCommon::mAttributes.icon);
    if(!attr.isNull()) 
    {
      QPixmap pixmap = LCBuildersCommon::getPixmap(attr, app);
      if(_iconSize >= 0)
      {
        int maxsize = (pixmap.width() > pixmap.height()) ? 
          (pixmap.width()) : (pixmap.height());

        float scale = 1;

        if(maxsize > 0)
        {
          scale = (float) _iconSize /(float)  maxsize;
        }

        pixmap = pixmap.scaled(
            pixmap.width() * scale, pixmap.height() * scale);
      }
      item->setIcon(pixmap);
    }
    list_widget->addItem(item, attr_val);
  };

  for(QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    add_item(node, icon_size);
  }

  return list_widget;
}
