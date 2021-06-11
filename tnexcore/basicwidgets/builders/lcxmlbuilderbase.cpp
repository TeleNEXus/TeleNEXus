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
#include "QRegularExpression"

#include "LIMovieAccess.h"
#include "applicationinterface.h"
#include "lcqwidgetvisiblecontrol.h"
#include "xmlcommon.h"

#include <QWidget>
#include <QMovie>
#include <QDomElement>
#include <QDebug>

//==============================================================================__slAttributes
static const struct
{
  QString file      = "file";
  QString position  = "position";
  QString fixedSize = "fixedSize";
  QString size = "size";
  QString name = "name";
  QString style = "style";
}__slAttributes;

//==============================================================================CMovieAccess
class CMovieAccess final : public LIMovieAccess
{
private:
  QMovie* mpMovie;
  int mStartCounter;
public:
  CMovieAccess() = delete;
  CMovieAccess(QMovie* _movie) : 
    mpMovie(_movie),
    mStartCounter(0)
  {
    mpMovie->jumpToFrame(0);
  }
  virtual ~CMovieAccess(){ mpMovie->deleteLater(); }
  virtual QMovie* getMovie() override { return mpMovie; }
  virtual void start() override
  {
    if(mStartCounter == 0) 
    {
      mpMovie->start();
    }
    mStartCounter++;
  }

  virtual void stop() override
  {
    mStartCounter--;
    if(mStartCounter <= 0)
    {
      mStartCounter = 0;
      mpMovie->stop();
    }
  }
};

//==============================================================================
static bool stringToNumber(const QString& _str, int& _out)
{
  bool flag = false;
  int number = _str.toInt(&flag);
  if(!flag) return false;
  _out = number;
  return true;
}

/* static bool stringToNumber(const QString& _str, float& _out) */
/* { */
/*   bool flag = false; */
/*   int number = _str.toFloat(&flag); */
/*   if(!flag) return false; */
/*   _out = number; */
/*   return true; */
/* } */

//==============================================================================
template<typename T> 
void attributeToValues(
    const QDomElement& _element, 
    const QString& _attrName, 
    std::function<void(T,T)> _setter)
{
  QString values_string = _element.attribute(_attrName);

  if(values_string.isNull()) return;

  auto values = xmlcommon::parseValues(values_string);

  if(values.size() != 2) return;

  T val_a;
  T val_b;

  if(!stringToNumber(values.first(), val_a)) return;

  values.pop_front();
  if(!stringToNumber(values.first(), val_b)) return;

  _setter(val_a, val_b);
}

//==============================================================================LCXmlBuilderBase
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
  return ret(buildLocal( _element, _app));
}

//------------------------------------------------------------------------------
bool LCXmlBuilderBase::setWidgetPosition(const QDomElement& _element, QWidget* _widget)
{
  bool ret = false;
  attributeToValues<int>(
      _element, 
      __slAttributes.position,
      [_widget, &ret](int w, int h)
      {
        _widget->move(w, h);
        ret = true;
      });
  return ret;
}

//------------------------------------------------------------------------------
bool LCXmlBuilderBase::setWidgetSize(const QDomElement& _element, QWidget* _widget)
{
  bool ret = false;
  attributeToValues<int>(
      _element, 
      __slAttributes.size,
      [_widget, &ret](int w, int h)
      {
        _widget->resize(w, h);
        ret = true;
      });
  return ret;
}

//------------------------------------------------------------------------------
bool LCXmlBuilderBase::setWidgetFixedSize(const QDomElement& _element, QWidget* _widget)
{
  bool ret = false;
  attributeToValues<int>(
      _element, 
      __slAttributes.fixedSize,
      [_widget, &ret](int w, int h)
      {
        _widget->setFixedSize(w, h);
        ret = true;
      });
  return ret;
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setWidgetName(
    const QDomElement& _element, QWidget* _widget)
{
  static quint64 counter = 0;
  QString attr_name = _element.attribute(__slAttributes.name);
  if(!attr_name.isNull()) 
  {
    _widget->setObjectName(attr_name);
  }
  else
  {
    _widget->setObjectName(
        QString("%1_%2")
        .arg(_widget->metaObject()->className())
        .arg(counter));
    counter++;
  }
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setWidgetStyle(const QString& _style, QWidget* _widget, 
    const QString& _name)
{

  QString outstyle = _style;

  auto set_style = 
    [_widget, &outstyle](const QString& _name)
    {
      outstyle.replace(QStringLiteral("(...)"), _name);
      _widget->setStyleSheet(outstyle);
    };

  if(_name.isNull())
  {
    set_style(QString("%1#%2")
        .arg(_widget->metaObject()->className())
        .arg(_widget->objectName()));
  }
  else
  {
    set_style(_name);
  }
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setWidgetStyle(
    const QDomElement& _element, 
    QWidget* _widget,
    const QString& _name)
{
  QString attr_style = _element.attribute(__slAttributes.style);
  if(attr_style.isNull()) return;

  QString style = 
    CApplicationInterface::getInstance().getWidgetStyle(attr_style);
  if(style.isNull()) {style = attr_style;}

  setWidgetStyle(style, _widget, _name);
}


//==============================================================================
static QMap<QString, QSharedPointer<CMovieAccess>> __slMovies;
//------------------------------------------------------------------------------
QSharedPointer<LIMovieAccess> 
LCXmlBuilderBase::getMovie(const QString& _movieFile)
{
    auto it = __slMovies.find(_movieFile);
    if(it != __slMovies.end())
    {
        return it.value();
    }
    QMovie* movie = new QMovie(_movieFile);
    auto ret = QSharedPointer<CMovieAccess>(new CMovieAccess(movie));
    __slMovies.insert(_movieFile, ret);
    return ret;
}

//==============================================================================
QMap<QString, QPixmap> __slPicture;

//------------------------------------------------------------------------------
QPixmap LCXmlBuilderBase::getPixmap(const QString& _pixmapFile)
{
    auto it = __slPicture.find(_pixmapFile);

    if(it != __slPicture.end())
    {
        return it.value();
    }

    QPixmap pixmap(_pixmapFile);
    __slPicture.insert(_pixmapFile, pixmap);

    return pixmap;
}

//------------------------------------------------------------------------------
QPixmap LCXmlBuilderBase::parsePixmap(const QString& _expr)
{
  auto set_size = 
    [](const QString& _attr_size, 
        std::function<void(int)> _setWidth,
        std::function<void(int)> _setHeight)
    {
      auto set_value = 
        [](const QString& _value, std::function<void(int)> _setter)
        {
          bool flag = false;
          int ivalue = _value.toInt(&flag);
          if(!flag) return;
          _setter(ivalue);
        };

      auto s = xmlcommon::parseAction(_attr_size);
      auto itsize = s.parameters.begin();
      if(itsize == s.parameters.end()) return;
      if(!((*itsize).isNull()))
      {
        set_value((*itsize), _setWidth);
      }
      itsize++;
      if(!((*itsize).isNull()))
      {
        set_value((*itsize), _setHeight);
      }
    };

  if(_expr.isNull()) return QPixmap();
  auto icon_attrs = xmlcommon::parseAttributes(_expr);
  auto attr_it = icon_attrs.find(__slAttributes.file);
  if(attr_it == icon_attrs.end()) return QPixmap();

  QPixmap pixmap(LCXmlBuilderBase::getPixmap(attr_it.value()));

  if(pixmap.isNull()) return pixmap;
  attr_it = icon_attrs.find(__slAttributes.size);

  QSize size = pixmap.size();

  if(attr_it != icon_attrs.end())
  {
    set_size(attr_it.value(), 
        [&size](int _width)
        {
          size.setWidth(_width);
        }, 
        [&size](int _height)
        {
          size.setHeight(_height);
        });
    pixmap = pixmap.scaled(size.width(), size.height());
  }
  return(pixmap);
}
