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
#include "lcxmlgroupboxbuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QGroupBox>
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
  /* QString style = "style"; */
  QString label = "label";
} __slAttributes;

//==============================================================================
namespace
{

class LCWidget : public QGroupBox
  {
  private:
    QSize mSizeHint;
  public:

    LCWidget() = delete;

    explicit LCWidget(const QString& _label, QWidget* _parent=nullptr); 
    virtual QSize sizeHint() const override;
    void addWidget(QWidget* _widget);
  };

//------------------------------------------------------------------------------
LCWidget::LCWidget(const QString& _label, QWidget* _parent) : 
  QGroupBox(_label, _parent)
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
}


//==============================================================================
LCXmlGroupBoxBuilder::LCXmlGroupBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlGroupBoxBuilder::~LCXmlGroupBoxBuilder()
{

}

//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);
//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlGroupBoxBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  LCWidget* widget = new LCWidget(_element.attribute(__slAttributes.label));

  setWidgetName(_element, widget); 
  setWidgetStyle(_element, widget);

  QDomNode  node = _element.firstChildElement(__slTags.layout);

  if(!node.isNull()) 
  {
    buildFromLayout(widget, node.toElement(), _app);
  }
  else
  {
    node = _element.firstChildElement(__slTags.widgets);

    if(!node.isNull()) 
    {
      buildFromWidgets(widget, node.toElement(), _app);
    }
  }

  setWidgetSize(_element, widget);
  setWidgetPosition(_element, widget);
  setWidgetFixedSize(_element, widget);

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

