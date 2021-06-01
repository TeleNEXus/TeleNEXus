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
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include <QTabWidget>
#include <QDomElement>
#include <QDebug>
#include <qdom.h>
#include <qtablewidget.h>


//------------------------------------------------------------------------------
static int __slObjectCounter = 0;
//------------------------------------------------------------------------------
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
    int _iconSize,
    const LIApplication& _app);

QWidget* LCXmlTabWidgetBuilder::buildLocal(QSharedPointer<SBuildData> _buildData) 
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QTabWidget* tabwidget = new QTabWidget;

  QString object_name = QString("XmlTabWidgetBar_%1").arg(__slObjectCounter);
  __slObjectCounter++;
  tabwidget->tabBar()->setObjectName(object_name);

  int index = 0;
  int icon_size = -1;

  QString attr = element.attribute(LCBuildersCommon::mAttributes.iconsize);

  if(!attr.isNull())
  {
    bool flag = 0;
    icon_size = attr.toInt(&flag);
    if(flag)
    {
      tabwidget->setIconSize(QSize(icon_size, icon_size));
    }
    else
    {
      icon_size = -1;
    }
  }

  for( QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    buildTab(node.toElement(), tabwidget, index, icon_size, app); 
    index++;
  }

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);

  tabwidget->setStyleSheet(
      QString("QTabBar#%1 { %2 }").arg(object_name).arg(style));

  LCBuildersCommon::initPosition(element, *tabwidget);

  return tabwidget;
}

//------------------------------------------------------------------------------
static void buildTab(
    const QDomElement& _element, 
    QTabWidget* _tabwidget, 
    int _tabindex, 
    int _iconSize,
    const LIApplication& _app)
{
  QString attr_label = 
    _element.attribute(LCBuildersCommon::mAttributes.label);

  QIcon icon;

  QString attr_icon = _element.attribute(LCBuildersCommon::mAttributes.icon);
  if(!attr_icon.isNull()) 
  {
    //Зогрузка изображения.
    QPixmap pixmap = LCBuildersCommon::getPixmap(attr_icon, _app);
    //Масштабирование изобразения под заданные размеры.
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
    if(!pixmap.isNull()) icon.addPixmap(pixmap);
  }


  if(attr_label.isNull() && icon.isNull())
  {
    attr_label = QString("Tab %1").arg(_tabindex);
  }

  QDomNodeList childs = _element.childNodes(); 
  for (int i = 0; i < childs.count(); i++)
  {
    QDomElement element = childs.at(i).toElement();
    if(!element.isNull())
    {
      QSharedPointer<LIXmlWidgetBuilder> builder = 
        _app.getWidgetBuilder(element.tagName());
      if(!builder.isNull())
      {
        QWidget* widget = (*builder).build(
            childs.at(i).toElement(), _app);
        if (widget)
        {
          _tabwidget->addTab( widget, icon, attr_label);
          return;
        }
      }
    }
  }

  _tabwidget->addTab( new QWidget, icon, attr_label);
}
