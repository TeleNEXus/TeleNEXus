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

#include "lcxmlswitchwidgetslistbuilder.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QDebug>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>

static const struct
{
    QString file        = "file";
    QString fontid      = "fontId";
    QString font        = "font";
    QString bgcolor     = "bgColor";
    QString bgimage     = "bgImage";
    QString bgpos       = "bgPos";
    QString textcolor   = "textColor";
    QString minwidth    = "minWidth";
    QString minheight   = "minHeight";
    QString maxwidth    = "maxWidth";
    QString maxheight   = "maxHeight";
    QString fixwidth    = "fixWidth";
    QString fixheight   = "fixHeight";
    QString iconsize    = "iconSize";
    QString text        = "text";
    QString icon        = "icon";

}__slBaseStyleAttribute;

//------------------------------------------------------------------------------
static const struct
{
  QString item = "item";
} __slTags;

//==============================================================================
static QString baseStyleSheet(const QDomElement& _element, 
    const LIApplication& _app);

//==============================================================================
LCXmlSwitchWidgetsListBuilder::LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlSwitchWidgetsListBuilder::~LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
    const QDomElement& _element, 
    const LIApplication& _app);

static QListWidgetItem* createItem(
    const QDomElement& _element, 
    const LIApplication& _app,
    int   _iconSize = -1);

//------------------------------------------------------------------------------
QWidget* LCXmlSwitchWidgetsListBuilder::build( const QDomElement& _element, 
    const LIApplication&    _app)
{
  QString attr_file = _element.attribute(__slBaseStyleAttribute.file);

  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return nullptr;
    if(el.tagName() != _element.tagName()) return nullptr;
    return (build(el, _app));
  }

  return (buildLocal(_element, _app));
}

//------------------------------------------------------------------------------
QWidget* LCXmlSwitchWidgetsListBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  const QDomElement& element = _element;
  const LIApplication& app = _app;

  QString attr;
  QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
  QListWidget* listWidget = new QListWidget;
  QStackedWidget* stacked_widget  = new QStackedWidget;

  QString style_sheet = baseStyleSheet(element, app);

  style_sheet = QString(".QListWidget { %1 }").arg(style_sheet);

  int icon_size = -1;
  attr = element.attribute(__slBaseStyleAttribute.iconsize);
  if(!attr.isNull())
  {
    bool flag = 0;
    icon_size = attr.toInt(&flag);
    if(flag)
    {
      listWidget->setIconSize(QSize(icon_size, icon_size));
    }
    else
    {
      icon_size = -1;
    }
  }

  listWidget->setStyleSheet(style_sheet);

  for( QDomNode node = element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    QDomElement el = node.toElement();

    QWidget* widget = createWidget(el, app);

    if(widget)
    { 
      listWidget->addItem(createItem(el, app, icon_size));
      stacked_widget->addWidget(widget);
    }
  }
  splitter->addWidget(listWidget);
  splitter->addWidget(stacked_widget);

  QObject::connect(
      listWidget, 
      &QListWidget::currentRowChanged, 
      [stacked_widget](int i)
      {
      stacked_widget->setCurrentIndex(i);
      });
  return splitter;
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
    const QDomElement& _element, 
    const LIApplication& _app)
{
  QWidget* widget = nullptr;
  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;

    auto builder = _app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;

    widget = builder->build(el, _app);
  }

  return widget;
}

//------------------------------------------------------------------------------
static QListWidgetItem* createItem(
    const QDomElement& _element, 
    const LIApplication& _app,
    int _iconSize)
{
  QListWidgetItem *item = new QListWidgetItem;

  QString attr = _element.attribute(__slBaseStyleAttribute.text);
  if(!attr.isNull()) item->setText(attr);

  attr = _element.attribute(__slBaseStyleAttribute.icon);
  if(!attr.isNull()) 
  {
    QPixmap pixmap(_app.getProjectPath() + attr);
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
  return item;
}



//==============================================================================
static QString baseStyleSheet(const QDomElement& _element, 
    const LIApplication& _app)
{
  QString style;

  QString attr = _element.attribute(QStringLiteral("fontId"));

  if(!attr.isNull())
  {
    QString font = _app.getFontStyle(attr);
    if(!font.isNull())
    {
      style += "font :" + font + "; ";
    }
  }
  else
  {
    attr = _element.attribute(QStringLiteral("font"));
    if(!attr.isNull())
    {
      style += "font : " + attr + "; ";
    }
  }

  attr = _element.attribute(__slBaseStyleAttribute.bgcolor);
  if(!attr.isNull())
  {
    style += QString("background: %1; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.bgimage);
  if(!attr.isNull())
  {
    style += QString("background-image:url(\"%1\"); " ).arg(_app.getProjectPath() + attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.bgpos);
  if(!attr.isNull())
  {
    if(!attr.isNull()) style += QString("background-position: %1 ;").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.textcolor);
  if(!attr.isNull())
  {
    style += QString("color: %1; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.minwidth);
  if(!attr.isNull())
  {
    style += QString("min-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.minheight);
  if(!attr.isNull())
  {
    style += QString("min-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.maxwidth);
  if(!attr.isNull())
  {
    style += QString("max-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.maxheight);
  if(!attr.isNull())
  {
    style += QString("max-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.fixwidth);
  if(!attr.isNull())
  {
    style += QString("max-width: %1px; ").arg(attr);
    style += QString("min-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(__slBaseStyleAttribute.fixheight);
  if(!attr.isNull())
  {
    style += QString("max-height: %1px; ").arg(attr);
    style += QString("min-height: %1px; ").arg(attr);
  }

  return style;
}
