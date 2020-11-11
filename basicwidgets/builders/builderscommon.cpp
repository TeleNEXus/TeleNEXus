#include "builderscommon.h"
#include <QDomElement>
#include <QWidget>
#include <QDebug>
#include <QMovie>
#include <QMap>
#include "LIApplication.h"
#include <QFileInfo>
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
    qDebug() << "CMovieAccess constructor current frame size = " << s;

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
const LCWidgetBuildersCommon::SAttributes LCWidgetBuildersCommon::mAttributes;

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

