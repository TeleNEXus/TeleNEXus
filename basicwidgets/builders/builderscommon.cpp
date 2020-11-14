#include "builderscommon.h"
#include <QDomElement>
#include <QWidget>
#include <QDebug>
#include <QMovie>
#include <QMap>
#include "LIApplication.h"
#include <QFileInfo>
#include <qnamespace.h>
#include "LIMovieAccess.h"

//==============================================================================
class CMovieAccess : public LIMovieAccess
{
private:
    QMovie* mpMovie;
    QSize mSize;
    int mStartCounter;
public:
    CMovieAccess() = delete;
    explicit CMovieAccess(QMovie* _movie);
    virtual ~CMovieAccess();
    virtual QMovie* getMovie() override;
    virtual void start() override;
    virtual void stop() override;
    virtual QSize getSize() override;
};

//------------------------------------------------------------------------------
CMovieAccess::CMovieAccess(QMovie* _movie) : 
    mpMovie(_movie),
    mStartCounter(0)
{
    int width  = 0;
    int height = 0;

    mpMovie->jumpToFrame(0);

    QSize s = mpMovie->currentPixmap().size();

    width = (s.width() > width) ? (s.width()) : (width);
    height = (s.height() > height) ? (s.height()) : (height);

    mSize = QSize(width, height);
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

//------------------------------------------------------------------------------
QSize CMovieAccess::getSize()
{
    return mSize;
}
//==============================================================================
QMap<QString, Qt::GlobalColor> __slStdColors;
//==============================================================================
const LCWidgetBuildersCommon::SAttributes LCWidgetBuildersCommon::mAttributes;
//------------------------------------------------------------------------------
LCWidgetBuildersCommon LCWidgetBuildersCommon::instance; 

//------------------------------------------------------------------------------
LCWidgetBuildersCommon::LCWidgetBuildersCommon()
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
void LCWidgetBuildersCommon::initPosition(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_pos = _element.attribute(mAttributes.posx);

    bool flag = false;

    int posx;
    int posy;

    posx = attr_pos.toInt(&flag);
    
    if(!flag)
    {
        return;
    }

    flag = false;

    attr_pos = _element.attribute(mAttributes.posy);
    posy = attr_pos.toInt(&flag);

    if(!flag)
    {
        return;
    }
    
    _widget.move(posx, posy);
}

//------------------------------------------------------------------------------
void LCWidgetBuildersCommon::initSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.width);
    QString attr_height = _element.attribute(mAttributes.height);
    
    int width;
    int height;

    bool flag;
    width = attr_width.toInt(&flag);
    if(!flag)
    {
        return;
    }

    height = attr_height.toInt(&flag);
    if(!flag)
    {
        return;
    }
    _widget.resize(width, height);
}

//------------------------------------------------------------------------------
void LCWidgetBuildersCommon::initFixedSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.fixwidth);
    QString attr_height = _element.attribute(mAttributes.fixheght);

    int width;
    int height;

    bool flag;
    width = attr_width.toInt(&flag);

    if(!flag)
    {
        return;
    }

    height = attr_height.toInt(&flag);

    if(!flag)
    {
        return;
    }
    _widget.setFixedSize(width, height);
}

//==============================================================================
static QMap<QString, QSharedPointer<CMovieAccess>> __slMovies;
//------------------------------------------------------------------------------
QSharedPointer<LIMovieAccess> LCWidgetBuildersCommon::getMovie(
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
QPixmap LCWidgetBuildersCommon::getPixmap(
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
  Qt::Alignment 
LCWidgetBuildersCommon::toAlignFlags(const QString& _attributes, bool* _flag)
{
  Qt::Alignment flags = 0;

  qDebug() << "Align flags 0 flags = "<<flags;
  if(_attributes.isNull())
  {
    if(_flag) *_flag = false;
    return Qt::AlignmentFlag::AlignLeft;
  }

  if(_attributes.contains(mAttributes.aligns.vals.Left))
  {
  qDebug() << "Align flags 1";
  qDebug() << "Align Left = " << Qt::AlignLeft;
    flags |= Qt::AlignLeft;
  }

  if(_attributes.contains(mAttributes.aligns.vals.Right))
  {
    flags |= Qt::AlignRight;
  }

  if((_attributes.contains(mAttributes.aligns.vals.Center)) && 
      (!_attributes.contains(mAttributes.aligns.vals.HCenter)) &&
      (!_attributes.contains(mAttributes.aligns.vals.VCenter)))
  {
  qDebug() << "Align flags 2";
    flags |= Qt::AlignCenter;
  }

  if(_attributes.contains(mAttributes.aligns.vals.HCenter))
  {
  qDebug() << "Align flags 3";
    flags |= Qt::AlignHCenter;
  }

  if(_attributes.contains(mAttributes.aligns.vals.Top))
  {
    flags |= Qt::AlignTop;
  }

  if(_attributes.contains(mAttributes.aligns.vals.Bottom))
  {
    flags |= Qt::AlignBottom;
  }

  if(_attributes.contains(mAttributes.aligns.vals.VCenter))
  {
    flags |= Qt::AlignVCenter;
  }

  if(flags == 0) 
  {
    if(_flag) *_flag = false;
    return Qt::AlignmentFlag::AlignLeft;
  }  
  if(_flag) *_flag = true;
  qDebug() << "Align flags = " << flags;

  return flags;
}

//------------------------------------------------------------------------------
Qt::Alignment
LCWidgetBuildersCommon::toAlignFlags(const QDomElement& _element, bool* _flag)
{
    return toAlignFlags(_element.attribute(mAttributes.aligns.attrName), _flag);
}

//------------------------------------------------------------------------------
QColor LCWidgetBuildersCommon::attributeToColor(const QString& _color)
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

