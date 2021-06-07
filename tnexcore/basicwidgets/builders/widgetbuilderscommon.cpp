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
#include "widgetbuilderscommon.h"
#include "xmlcommon.h"
#include <QDomElement>
#include <QWidget>
#include <QDebug>
#include <QMovie>
#include <QMap>
#include "LIApplication.h"
#include <QFileInfo>
#include <qnamespace.h>
#include "LIMovieAccess.h"
#include <QRegularExpression>

//==============================================================================
class CMovieAccess : public LIMovieAccess
{
private:
    QMovie* mpMovie;
    int mStartCounter;
public:
    CMovieAccess() = delete;
    explicit CMovieAccess(QMovie* _movie);
    virtual ~CMovieAccess();
    virtual QMovie* getMovie() override;
    virtual void start() override;
    virtual void stop() override;
};

//------------------------------------------------------------------------------
CMovieAccess::CMovieAccess(QMovie* _movie) : 
    mpMovie(_movie),
    mStartCounter(0)
{
    mpMovie->jumpToFrame(0);
}

//------------------------------------------------------------------------------
CMovieAccess::~CMovieAccess()
{
    mpMovie->deleteLater();
}

//------------------------------------------------------------------------------
QMovie* CMovieAccess::getMovie()
{
    return mpMovie;
}

//------------------------------------------------------------------------------
void CMovieAccess::start()
{
    if(mStartCounter == 0) 
    {
        mpMovie->start();
    }
    mStartCounter++;
}

//------------------------------------------------------------------------------
void CMovieAccess::stop()
{
    mStartCounter--;
    if(mStartCounter <= 0)
    {
        mStartCounter = 0;
        mpMovie->stop();
    }
}

//==============================================================================
QMap<QString, Qt::GlobalColor> __slStdColors;
//==============================================================================
const LCBuildersCommon::SAttributes LCBuildersCommon::mAttributes;
//------------------------------------------------------------------------------
LCBuildersCommon LCBuildersCommon::instance; 

//------------------------------------------------------------------------------
LCBuildersCommon::LCBuildersCommon()
{
    __slStdColors.insert( "white"      , Qt::GlobalColor::white);
    __slStdColors.insert( "black"      , Qt::GlobalColor::black);
    __slStdColors.insert( "red"        , Qt::GlobalColor::red);
    __slStdColors.insert( "darkred"    , Qt::GlobalColor::darkRed);
    __slStdColors.insert( "green"      , Qt::GlobalColor::green);
    __slStdColors.insert( "darkgreen"  , Qt::GlobalColor::darkGreen);
    __slStdColors.insert( "blue"       , Qt::GlobalColor::blue);
    __slStdColors.insert( "darkblue"   , Qt::GlobalColor::darkBlue);
    __slStdColors.insert( "cyan"       , Qt::GlobalColor::cyan);
    __slStdColors.insert( "darkcyan"   , Qt::GlobalColor::darkCyan);
    __slStdColors.insert( "magenta"    , Qt::GlobalColor::magenta);
    __slStdColors.insert( "darkmagenta", Qt::GlobalColor::darkMagenta);
    __slStdColors.insert( "yellow"     , Qt::GlobalColor::yellow);
    __slStdColors.insert( "darkyellow" , Qt::GlobalColor::darkYellow);
    __slStdColors.insert( "gray"       , Qt::GlobalColor::gray);
    __slStdColors.insert( "darkgray"   , Qt::GlobalColor::darkGray);
    __slStdColors.insert( "lightgray"  , Qt::GlobalColor::lightGray);
    __slStdColors.insert( "transparent", Qt::GlobalColor::transparent);
    __slStdColors.insert( "color0"     , Qt::GlobalColor::color0);
    __slStdColors.insert( "color1"     , Qt::GlobalColor::color1);
}

//------------------------------------------------------------------------------
void LCBuildersCommon::initPosition(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_pos = _element.attribute(mAttributes.posx);
    int posx = _widget.pos().x(); 
    int posy = _widget.pos().y();

    bool flag = false;
    posx = attr_pos.toInt(&flag);

    attr_pos = _element.attribute(mAttributes.posy);
    flag = false;
    posy = attr_pos.toInt(&flag);
    
    _widget.move(posx, posy);
}

//------------------------------------------------------------------------------
void LCBuildersCommon::initSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.width);
    QString attr_height = _element.attribute(mAttributes.height);
    
    int width = _widget.sizeHint().width();
    int height = _widget.sizeHint().height();

    int buff;

    bool flag_transform = false;
    bool flag_set = false;
    buff = attr_width.toInt(&flag_transform);
    if(flag_transform)
    {
      flag_set = true;
      width = buff;
    }

    buff = attr_height.toInt(&flag_transform);
    if(flag_transform)
    {
      flag_set = true;
      height = buff;
    }
    if(flag_set) _widget.resize(width, height);
}

//------------------------------------------------------------------------------
void LCBuildersCommon::initFixedSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.fixwidth);
    QString attr_height = _element.attribute(mAttributes.fixheight);

    int width = _widget.sizeHint().width();
    int height = _widget.sizeHint().height();

    int buff;

    bool flag_transform = false;
    bool flag_set = false;
    buff = attr_width.toInt(&flag_transform);
    if(flag_transform)
    {
      flag_set = true;
      width = buff;
    }

    buff = attr_height.toInt(&flag_transform);
    if(flag_transform)
    {
      flag_set = true;
      height = buff;
    }

    if(flag_set) _widget.setFixedSize(width, height);
}

//==============================================================================
static QMap<QString, QSharedPointer<CMovieAccess>> __slMovies;
//------------------------------------------------------------------------------
QSharedPointer<LIMovieAccess> LCBuildersCommon::getMovie(
        const QString& _movieFile, const LIApplication& _app)
{
    auto it = __slMovies.find(_movieFile);

    if(it != __slMovies.end())
    {
        return it.value();
    }

    QMovie* movie = new QMovie(_app.getProjectPath() + _movieFile);

    auto ret = QSharedPointer<CMovieAccess>(new CMovieAccess(movie));

    __slMovies.insert(_movieFile, ret);
    return ret;
}

//==============================================================================
QMap<QString, QPixmap> __slPicture;
//------------------------------------------------------------------------------
QPixmap LCBuildersCommon::getPixmap(
        const QString& _pixmapFile, const LIApplication& _app)
{
    auto it = __slPicture.find(_pixmapFile);

    if(it != __slPicture.end())
    {
        return it.value();
    }

    QPixmap pixmap(_app.getProjectPath() + _pixmapFile);
    __slPicture.insert(_pixmapFile, pixmap);

    return pixmap;
}

//------------------------------------------------------------------------------
bool
LCBuildersCommon::toAlignFlags(const QString& _attributes, 
    Qt::Alignment& _flags)
{

  bool ret = false;

  if(_attributes.isNull())
  {
    return ret;
  }

  QString attr = _attributes.toLower();

  if(attr.contains(mAttributes.aligns.vals.Left))
  {
    ret = true;
    _flags |= Qt::AlignLeft;
  }

  if(_attributes.contains(mAttributes.aligns.vals.Right))
  {
    ret = true;
    _flags |= Qt::AlignRight;
  }

  if((attr.contains(mAttributes.aligns.vals.Center)) && 
      (!attr.contains(mAttributes.aligns.vals.HCenter)) &&
      (!attr.contains(mAttributes.aligns.vals.VCenter)))
  {
    ret = true;
    _flags |= Qt::AlignCenter;
  }

  if(attr.contains(mAttributes.aligns.vals.HCenter))
  {
    ret = true;
    _flags |= Qt::AlignHCenter;
  }

  if(attr.contains(mAttributes.aligns.vals.Top))
  {
    ret = true;
    _flags |= Qt::AlignTop;
  }

  if(attr.contains(mAttributes.aligns.vals.Bottom))
  {
    ret = true;
    _flags |= Qt::AlignBottom;
  }

  if(attr.contains(mAttributes.aligns.vals.VCenter))
  {
    ret = true;
    _flags |= Qt::AlignVCenter;
  }

  return ret;
}

//------------------------------------------------------------------------------
bool
LCBuildersCommon::toAlignFlags(const QDomElement& _element, 
    Qt::Alignment& _flags)
{
    return toAlignFlags(_element.attribute(mAttributes.aligns.attrName), _flags);
}

//------------------------------------------------------------------------------
QString LCBuildersCommon::toAlignString(const QString& _attributes)
{
  
  QString ret;
  if(_attributes.isNull())
  {
    return ret;
  }
  
  QString attr = _attributes.toLower();

  if(attr.contains(mAttributes.aligns.vals.Left))
  {
    ret += "Qt::AlignLeft | ";
  }

  if(attr.contains(mAttributes.aligns.vals.Right))
  {
    ret += "Qt::AlignRight | ";
  }

  if((attr.contains(mAttributes.aligns.vals.Center)) && 
      (!attr.contains(mAttributes.aligns.vals.HCenter)) &&
      (!attr.contains(mAttributes.aligns.vals.VCenter)))
  {
    ret += "Qt::AlignCenter | ";
  }

  if(attr.contains(mAttributes.aligns.vals.HCenter))
  {
    ret += "Qt::AlignHCenter | ";
  }

  if(attr.contains(mAttributes.aligns.vals.Top))
  {
    ret += "Qt::AlignTop | ";
  }

  if(attr.contains(mAttributes.aligns.vals.Bottom))
  {
    ret += "Qt::AlignBottom | ";
  }

  if(attr.contains(mAttributes.aligns.vals.VCenter))
  {
    ret += "Qt::AlignVCenter | ";
  }
  if(!ret.isNull())
  {
    ret.resize(ret.size() - 3);
  }
  return ret;
}

//------------------------------------------------------------------------------
QColor LCBuildersCommon::attributeToColor(const QString& _color)
{

    QString color_name = _color.toLower();

    if(__slStdColors.contains(color_name))
    {
        return QColor(__slStdColors[color_name]);
    }

    color_name.remove(QRegExp( QString("[ ]{1,}|[_]{1,}") ));
    bool flag = false;
    QRgb rgb;

    rgb = color_name.toUInt(&flag, 16);

    if(flag)
    {
        return QColor(rgb);
    }

    return QColor();
}

//------------------------------------------------------------------------------
QString LCBuildersCommon::getBaseStyleSheet(const QDomElement& _element, 
    const LIApplication& _app)
{
  QString style;

  QString attr = _element.attribute(LCBuildersCommon::mAttributes.fontid);

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
    attr = _element.attribute(LCBuildersCommon::mAttributes.font);
    if(!attr.isNull())
    {
      style += "font : " + attr + "; ";
    }
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.bgcolor);
  if(!attr.isNull())
  {
    style += QString("background: %1; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.bgimage);
  if(!attr.isNull())
  {
    style += QString("background-image:url(\"%1\"); " ).arg(_app.getProjectPath() + attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.bgpos);
  if(!attr.isNull())
  {
    if(!attr.isNull()) style += QString("background-position: %1 ;").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.textcolor);
  if(!attr.isNull())
  {
    style += QString("color: %1; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.minwidth);
  if(!attr.isNull())
  {
    style += QString("min-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.minheight);
  if(!attr.isNull())
  {
    style += QString("min-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.maxwidth);
  if(!attr.isNull())
  {
    style += QString("max-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.maxheight);
  if(!attr.isNull())
  {
    style += QString("max-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.fixwidth);
  if(!attr.isNull())
  {
    style += QString("max-width: %1px; ").arg(attr);
    style += QString("min-width: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.fixheight);
  if(!attr.isNull())
  {
    style += QString("max-height: %1px; ").arg(attr);
    style += QString("min-height: %1px; ").arg(attr);
  }

  attr = _element.attribute(LCBuildersCommon::mAttributes.aligns.attrName);
  if(!attr.isNull())
  {
    attr = LCBuildersCommon::toAlignString(attr);
    if(!attr.isNull()) style += 
      QString("qproperty-alignment: '%1' ;").arg(attr);
  }
  return style;
}

//------------------------------------------------------------------------------
QPixmap LCBuildersCommon::parsePixmap(
    const QString& _expr, const LIApplication& _app)
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
  auto attr_it = icon_attrs.find(QStringLiteral("file"));
  if(attr_it == icon_attrs.end()) return QPixmap();

  QPixmap pixmap(LCBuildersCommon::getPixmap(attr_it.value(), _app));

  if(pixmap.isNull()) return pixmap;
  attr_it = icon_attrs.find(QStringLiteral("size"));

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

void LCBuildersCommon::setWidgetName(QWidget* _widget, const QString& _name)
{
  static quint64 counter = 0;
  qDebug() << "LCBuildersCommon::setWidgetName _name = " << _name;

  if(!_name.isNull()) 
  {
    _widget->setObjectName(_name);
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

void LCBuildersCommon::setStyle(const QString& _style, QWidget* _widget)
{
  auto ret = [_widget](const QString& _ss)
  {
    _widget->setStyleSheet(_ss);
  };

  if(_style.contains(
        QRegularExpression(
          QStringLiteral("\\A([^{^}]*\\{[^{^}]*\\})+\\s*\\z"))))
  {
    return ret(_style);
  }

  if(!_widget->objectName().isNull())
  {
    return ret( QString("%1#%2 {%3}")
        .arg(_widget->metaObject()->className())
        .arg(_widget->objectName())
        .arg(_style));
  }
  return ret(_style);
}

