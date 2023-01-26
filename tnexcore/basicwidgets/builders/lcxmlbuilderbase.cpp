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

#include <QBuffer>
#include <QWidget>
#include <QMovie>
#include <QDomElement>
#include <QDebug>
#include <QByteArray>
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
  QWidget* w = nullptr;

  auto builder = 
    [this, &_app, &w](const QDomElement& _element)
    {
      if(_element.isNull())
      {
        return;
      }

      w = buildLocal(_element, _app);

      if(w != nullptr)
      {
        LCQWidgetVisibleControl::build(_element, w, _app);
        LQEventsFilter::install(w, _element, _app);
      }
    };

  _app.buildFromFile(_element, builder);

  return w;
}

//------------------------------------------------------------------------------
bool LCXmlBuilderBase::setWidgetPosition(
    const QDomElement& _element, QWidget* _widget)
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
bool LCXmlBuilderBase::setWidgetSize(
    const QDomElement& _element, QWidget* _widget)
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
bool LCXmlBuilderBase::setWidgetFixedSize(
    const QDomElement& _element, QWidget* _widget)
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
void LCXmlBuilderBase::setWidgetName(QWidget* _widget)
{
  CApplicationInterface::getInstance().setWidgetUniqName(_widget);
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setWidgetStyle(const QString& _style, QWidget* _widget,
    const QString& _objectName)
{
  CApplicationInterface::getInstance().setWidgetStyle(
      _style, _widget, _objectName);
}

//------------------------------------------------------------------------------
void LCXmlBuilderBase::setWidgetStyle(
    const QDomElement& _element, QWidget* _widget, const QString& _objectName)
{
  QString attr_style = _element.attribute(__slAttributes.style);
  if(attr_style.isNull()) return;
  setWidgetStyle(attr_style, _widget, _objectName);
}


//==============================================================================
static QMap<QString, QSharedPointer<CMovieCtrl>> __slMovies;
//------------------------------------------------------------------------------

/* static void deleter(QIODevice* _d) */
/* { */
/*   _d->deleteLater(); */
/* } */

QSharedPointer<LIMovieAccess> 
LCXmlBuilderBase::getMovie(const QString& _movieFile)
{
  auto it = __slMovies.find(_movieFile);

  if(it != __slMovies.end())
  {
    return QSharedPointer<LIMovieAccess>(new CMovieAccess(it.value()));
  }

  QSharedPointer<QIODevice> fd =
    CApplicationInterface::getInstance().getFileDevice(_movieFile);


  if(fd.isNull()) return QSharedPointer<LIMovieAccess>();
  

  if(!fd->open(QIODevice::OpenModeFlag::ReadOnly)) 
    return QSharedPointer<LIMovieAccess>();

  QByteArray* data = new QByteArray();

  *data = fd->readAll();

  QBuffer* buffer = new QBuffer(data);
  QObject::connect(buffer, &QBuffer::destroyed,
      [data](QObject*)
      {
        delete data;
      });

  QMovie* movie = new QMovie(buffer);
  buffer->setParent(movie);

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
#include "QImageReader"
QPixmap LCXmlBuilderBase::getPixmap(const QString& _pixmapFile)
{

  auto fd = CApplicationInterface::getInstance().getFileDevice(_pixmapFile);
  if(fd.isNull()) return QPixmap();
  QImageReader ir(fd.data());

  return QPixmap::fromImage(ir.read());
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
