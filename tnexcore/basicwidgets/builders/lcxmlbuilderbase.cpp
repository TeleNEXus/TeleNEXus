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

#include "lcxmlbuilderbase.h"
#include "widgetbuilderscommon.h"
#include "QRegularExpression"

#include "LIApplication.h"
#include "lcqwidgetvisiblecontrol.h"
#include "xmlcommon.h"

#include <QWidget>

#include <QDomElement>
#include <QDebug>

//==============================================================================
static const struct
{
  QString file      = "file";
  QString position  = "position";
  QString fixedSize = "fixedSize";
  QString size = "size";
}__slAttributes;

//==============================================================================
LCXmlBuilderBase::LCXmlBuilderBase()
{
}

//------------------------------------------------------------------------------
LCXmlBuilderBase::~LCXmlBuilderBase()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlBuilderBase::build( const QDomElement& _element, 
    const LIApplication&    _app)
{
  auto ret = [&_element, &_app](QWidget* _w)
  {
    LCQWidgetVisibleControl::build(_element, _w, _app);
    return _w;
  };

  QString attr_file = _element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return nullptr;
    if(el.tagName() != _element.tagName()) return nullptr;
    return ret(build(el, _app));
  }

  return ret(buildLocal(
      QSharedPointer<SBuildData>(new SBuildData{QPoint(), _element, _app})));
}


static void twoIntValueSetter(const QString& _valuesString, std::function<void(int,int)> _setter)
{
  auto values = xmlcommon::parseValues(_valuesString);
  if(values.size() != 2) return;

  int val_a;
  int val_b;

  bool flag = false;
  int value = values.first().toInt(&flag);
  if(!flag) return;
  val_a = (value);
  values.pop_front();
  flag = false;
  value = values.first().toInt(&flag);
  if(!flag) return;
  val_b = (value);
  _setter(val_a, val_b);
}


//------------------------------------------------------------------------------
void LCXmlBuilderBase::setPosition(const QDomElement& _element, QWidget* _widget)
{
  QString attr = _element.attribute(__slAttributes.position);
  if(attr.isNull()) return;
  twoIntValueSetter(attr,
      [_widget](int w, int h)
      {
        _widget->move(w, h);
      });
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setSize(const QDomElement& _element, QWidget* _widget)
{
  QString attr = _element.attribute(__slAttributes.size);
  if(attr.isNull()) return;
  twoIntValueSetter(attr,
      [_widget](int w, int h)
      {
        _widget->resize(w, h);
      });
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setFixedSize(const QDomElement& _element, QWidget* _widget)
{
  QString attr = _element.attribute(__slAttributes.fixedSize);
  if(attr.isNull()) return;
  twoIntValueSetter(attr,
      [_widget](int w, int h)
      {
        _widget->setFixedSize(w, h);
      });
}

