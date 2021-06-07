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
#include "lcxmlframebuilder.h"
#include "widgetbuilderscommon.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QFrame>
#include <QDomElement>
#include <QFile>
#include <QDebug>

//==============================================================================
static const struct
{
  QString layout = "layout";
  QString widgets = "widgets";
} __slTags;

//------------------------------------------------------------------------------
static const struct
{
  QString style = "style";
  QString name = "name";
} __slAttributes;

//==============================================================================
class LCWidget : public QFrame
{
private:
  QSize mSizeHint;
public:
  explicit LCWidget(QWidget* _parent=nullptr); 
  virtual QSize sizeHint() const override;
  void addWidget(QWidget* _widget);
};

//------------------------------------------------------------------------------
LCWidget::LCWidget(QWidget* _parent) : 
  QFrame(_parent)
{
}

//------------------------------------------------------------------------------
QSize LCWidget::sizeHint() const 
{
  return QSize(mSizeHint.width()+5, mSizeHint.height()+5);
}

//------------------------------------------------------------------------------
void LCWidget::addWidget(QWidget* _widget)
{
  int width   = mSizeHint.width();
  int height  = mSizeHint.height();
  _widget->setParent(this);
  _widget->show();

  if( _widget->geometry().bottomRight().x() > width)
  {
    width = _widget->geometry().bottomRight().x();
  }

  if( _widget->geometry().bottomRight().y() > height)
  {
    height = _widget->geometry().bottomRight().y();
  }
  mSizeHint = QSize(width, height);
}


//==============================================================================
LCXmlFrameBuilder::LCXmlFrameBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlFrameBuilder::~LCXmlFrameBuilder()
{

}

//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);
//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlFrameBuilder::buildLocal(QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QDomNode  node = element.firstChildElement(__slTags.layout);
  LCWidget* widget = new LCWidget;

  QString attr_name = element.attribute(__slAttributes.name);
  qDebug() << "LCXmlFrameBuilder::buildLocal attribute naem = " << attr_name;
  LCBuildersCommon::setWidgetName(
      widget, attr_name);

  qDebug() << "LCXmlFrameBuilder::builLocal widget name = " << widget->objectName();
  QString style = element.attribute(__slAttributes.style);

  if(!style.isNull())
  {
    LCBuildersCommon::setStyle(style, widget);
  }

  if(!node.isNull()) 
  {
    buildFromLayout(widget, node.toElement(), app);
  }
  else
  {
    node = element.firstChildElement(__slTags.widgets);

    if(!node.isNull()) 
    {
      buildFromWidgets(widget, node.toElement(), app);
    }
  }

  LCBuildersCommon::initPosition(element, *widget);



  return widget;
}


//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app)
{
  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(!el.isNull())
    {
      auto builder = _app.getLayoutBuilder(el.tagName());
      if(!builder.isNull())
      {
        QLayout* layout = (*builder).build(el, _app);
        if(layout)
        {
          _widget->setLayout(layout);
          break;
        }
      }                 
    }
  }
}

//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app)
{

  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    auto builder = _app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;
    QWidget* build_widget = builder->build(el, _app);
    if(build_widget)
    {
      _widget->addWidget(build_widget);
    }
  }
}

