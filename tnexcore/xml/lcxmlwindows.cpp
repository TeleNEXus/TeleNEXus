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
#include "lcxmlwindows.h"
#include "lcxmlcommon.h"
#include "LIWindow.h"
#include "LIKeyboard.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>
#include <LIRemoteDataWriter.h>
#include <QEvent>


//==============================================================================LCXmlWindow
class LCXmlWindow : public LIWindow
{
public:
  static QMap<QString, QSharedPointer<LIWindow>> smWindowsMap;
  QWidget* mpWidget;
public:
  LCXmlWindow() = delete;
  LCXmlWindow(QWidget* _widget): mpWidget(_widget){}
  virtual ~LCXmlWindow()
  {
  }
  //--------------------------------------------------------------------------
  virtual void show() override
  {
    mpWidget->show();
  }

  //--------------------------------------------------------------------------
  virtual void hide() override
  {
    mpWidget->close();
  }
};


//------------------------------------------------------------------------------
QMap<QString, QSharedPointer<LIWindow>> LCXmlWindow::smWindowsMap;

//==============================================================================LCXmlWindows
LCXmlWindows::LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows::~LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows& LCXmlWindows::instance()
{
  static LCXmlWindows instance;
  return instance;
}

//------------------------------------------------------------------------------
QSharedPointer<LIWindow> LCXmlWindows::getWindow(const QString& _windowId)
{
  return LCXmlWindow::smWindowsMap.find(_windowId).value();
}

//------------------------------------------------------------------------------
#include "LIXmlWidgetBuilder.h"

static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app);

static LCXmlWindow* buildLocal(const QDomElement& _element,
    const LIApplication& _app);

static void initWindow(LCXmlWindow* _win, const QDomElement& _element);

//------------------------------------------------------------------------------
void LCXmlWindows::buildWindow(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  auto window = buildLocal(_element, _app); 
  initWindow(window, _element);
}

//------------------------------------------------------------------------------
static void initWindow(LCXmlWindow* _win, const QDomElement& _element)
{
  if(_win == nullptr) return;

  QString attr_id = _element.attribute(
      LCXmlCommon::mCommonAttributes.id);
  QString attr_title = _element.attribute(
      LCXmlCommon::mCommonAttributes.title);
  QString attr_show = _element.attribute(
      LCXmlCommon::mCommonAttributes.show.tag);

  if(!attr_id.isNull()) 
  {
    LCXmlWindow::smWindowsMap.insert(attr_id, 
        QSharedPointer<LIWindow>(_win));
    if(attr_title.isNull()) attr_title = attr_id;
  }

  if(!attr_title.isNull())
  {
    _win->mpWidget->setWindowTitle(attr_title);
  }

  if(!attr_show.isNull())
    _win->mpWidget->show();
}

//------------------------------------------------------------------------------
static void widgetAttr(QWidget* _widget, const QDomElement& _element);

static LCXmlWindow* buildLocal(const QDomElement& _element,
    const LIApplication& _app)
{
  QString attr_file =  _element.attribute(
      LCXmlCommon::mCommonAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      return buildLocal(el, _app);
    }
    return nullptr;
  }


  QWidget* widget = buildWidget(_element, _app);
  if(widget == nullptr) 
  {
    return nullptr;
  }

  LCXmlWindow*  window = new LCXmlWindow(widget);

  widgetAttr(widget, _element);

  return window;
}

//------------------------------------------------------------------------------
static void widgetAttr(QWidget* _widget, const QDomElement& _element)
{
  //Получение атрибутов.
  QString attr_width = _element.attribute(
      LCXmlCommon::mCommonAttributes.widht);

  QString attr_height = _element.attribute(
      LCXmlCommon::mCommonAttributes.height);

  //Переопределение размеров окна.
  QSize s = _widget->geometry().size();

  if(!attr_width.isNull())
  {
    bool flag = false;
    int width = attr_width.toInt(&flag);

    if(flag) 
    {
      s.setWidth(width);
    }
  }

  if(!attr_height.isNull())
  {
    bool flag = false;
    int height = attr_height.toInt(&flag);
    if(flag) 
    {
      s.setHeight(height);
    }
  }

  bool flag_posx = false;
  bool flag_posy = false;
  int posx;
  int posy;

  QString attr = _element.attribute(
      LCXmlCommon::mCommonAttributes.posx);
  posx = attr.toInt(&flag_posx);

  attr = _element.attribute(
      LCXmlCommon::mCommonAttributes.posy);
  posy = attr.toInt(&flag_posy);

  if(flag_posx && flag_posy)
  {
    _widget->move(posx, posy);
  }
  _widget->resize(s);
}

//------------------------------------------------------------------------------
static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app)
{
  for(auto node = _element.firstChild(); 
      !node.isNull(); 
      node = node.nextSibling())
  {
    if(node.isElement())
    {
      auto el = node.toElement();
      auto builder = _app.getWidgetBuilder(el.tagName());
      if(!builder.isNull())
      {
        auto widget = builder->build(el, _app);
        if(widget) return widget;
      }
    }
  }
  return nullptr;
}
