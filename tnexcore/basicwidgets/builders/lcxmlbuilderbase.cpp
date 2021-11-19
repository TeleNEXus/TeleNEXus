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
#include "lqeventsfilter.h"
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


//==============================================================================CMovieCtrl
class CMovieCtrl
{
private:
  QMovie* mpMovie;
  int mStartCounter;
public:
  CMovieCtrl() = delete;
  CMovieCtrl(QMovie* _movie) : 
    mpMovie(_movie),
    mStartCounter(0)
  {
    mpMovie->jumpToFrame(0);
  }
  ~CMovieCtrl(){ mpMovie->deleteLater(); }
  QMovie* getMovie() { return mpMovie; }
  void start() 
  {
    if(mStartCounter == 0) 
    {
      mpMovie->start();
    }
    mStartCounter++;
  }

  void stop()
  {
    mStartCounter--;
    if(mStartCounter <= 0)
    {
      mStartCounter = 0;
      mpMovie->stop();
    }
  }
};

//==============================================================================CMovieAccess
class CMovieAccess final : public LIMovieAccess
{
private:
  QSharedPointer<CMovieCtrl> mspMovieCtrl;
  bool mStartFlag;

public:
  CMovieAccess() = delete;

  CMovieAccess(QSharedPointer<CMovieCtrl> _movieCtrl) : 
    mspMovieCtrl(_movieCtrl)
    ,mStartFlag(false)
  {
  }

  virtual ~CMovieAccess(){}

  virtual QMovie* getMovie() override { return mspMovieCtrl->getMovie(); }

  virtual void start() override
  {
    if(!mStartFlag) 
    {
      mspMovieCtrl->start();
      mStartFlag = true;
    }
  }

  virtual void stop() override
  {
    if(mStartFlag) 
    {
      mspMovieCtrl->stop();
      mStartFlag = false;
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
    LQEventsFilter::install(_w, _element, _app);
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
        if(w > 0) _widget->setFixedWidth(w);
        if(h > 0) _widget->setFixedHeight(h);
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
    const QString& _objectName)
{
  QString outstyle = _style; 

  const QRegularExpression rexpr_style_id_desc(
      QStringLiteral("\\$\\([^\\(\\)\\$]+\\)"));

  const QRegularExpression rexpr_style_id_extract(
      QStringLiteral("(?<=\\$\\()(.*)(?=\\))"));

  auto mit= rexpr_style_id_desc.globalMatch(_style);

  while(mit.hasNext())
  {
    QString style_desc = mit.next().captured();
    QString style_id = 
      rexpr_style_id_extract.match(style_desc).captured();

    QString load_style = CApplicationInterface::getInstance().getWidgetStyle(style_id);
    if(load_style.isNull())
    {
      outstyle.remove(style_desc);
    }
    else
    {
      outstyle.replace(style_desc, load_style);
    }
  }


  auto set_style = 
    [_widget, &outstyle](const QString& _objectName)
    {
      outstyle.replace(QStringLiteral("(...)"), _objectName);
      _widget->setStyleSheet(outstyle);
    };

  if(_objectName.isNull())
  {
    set_style(QString("%1#%2")
        .arg(_widget->metaObject()->className())
        .arg(_widget->objectName()));
  }
  else
  {
    set_style(_objectName);
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

  /* QString style = */ 
  /*   CApplicationInterface::getInstance().getWidgetStyle(attr_style); */
  /* if(style.isNull()) {style = attr_style;} */

  /* setWidgetStyle(style, _widget, _name); */
  setWidgetStyle(attr_style, _widget, _name);
}


//==============================================================================
static QMap<QString, QSharedPointer<CMovieCtrl>> __slMovies;
//------------------------------------------------------------------------------
QSharedPointer<LIMovieAccess> 
LCXmlBuilderBase::getMovie(const QString& _movieFile)
{
  auto it = __slMovies.find(_movieFile);

  if(it != __slMovies.end())
  {
    return QSharedPointer<LIMovieAccess>(new CMovieAccess(it.value()));
  }

  QMovie* movie = new QMovie(_movieFile);

  if(!movie->isValid())
  {
    delete movie;
    return QSharedPointer<LIMovieAccess>();
  }

  auto movie_ctrl = QSharedPointer<CMovieCtrl>(new CMovieCtrl(movie));
  __slMovies.insert(_movieFile, movie_ctrl);
  return QSharedPointer<LIMovieAccess>(new CMovieAccess(movie_ctrl));
}

//==============================================================================
QPixmap LCXmlBuilderBase::getPixmap(const QString& _pixmapFile)
{
  QPixmap pixmap(_pixmapFile);
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
