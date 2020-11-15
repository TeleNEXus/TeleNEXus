#include "lcqremcombolabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include "LIMovieAccess.h"
#include <QMovie>
#include <qnamespace.h>

//==============================================================================IItem
class IItem
{
protected:
    QLabel* mpLabel;
public:
    IItem() = delete;
    IItem(QLabel* _label) : mpLabel(_label){}
    virtual void install() = 0;
    virtual void uninstall() = 0;
};

//==============================================================================CItemText
class CItemText : public IItem
{
private:
  QString mText;
  QFont mFont;
  QPalette mPalette;
  Qt::Alignment mAlignment;
  /* QFont mFontPr; */
  /* QPalette mPalettePr; */
  /* Qt::Alignment mAlignmentPr; */
public:
  CItemText() = delete;
  explicit CItemText(
      QLabel* _label, 
      const QString& _text, 
      QFont _font,
      QPalette _palette,
      Qt::Alignment _alignment): 
    IItem(_label), 
    mText(_text),
    mFont(_font),
    mPalette(_palette),
    mAlignment(_alignment)
  {}

  virtual void install() override
  {
    /* mPalettePr = mpLabel->palette(); */
    /* mFontPr = mpLabel->font(); */
    /* mAlignmentPr = mpLabel->alignment(); */
    qDebug() << "install align = " << mAlignment;
    qDebug() << "install palette size= " << mPalette;

    mpLabel->setText(mText);
    mpLabel->setPalette(mPalette);
    mpLabel->setFont(mFont);
    mpLabel->setAlignment(mAlignment);
    mpLabel->adjustSize();
  }

  virtual void uninstall() override
  {
    mpLabel->clear();
    /* mpLabel->setPalette(mPalettePr); */
    /* mpLabel->setFont(mFontPr); */
    /* mpLabel->setAlignment(mAlignmentPr); */
    mpLabel->adjustSize();
  }
};

//==============================================================================CItemMovie
class CItemMovie: public IItem
{
public:
  QSharedPointer<LIMovieAccess> mspMovieAccess;
private:
  Qt::Alignment mAlignment;
  Qt::Alignment mAlignmentPr;
  QPalette mPalette;
  /* QPalette mPalettePr; */
public:
  CItemMovie() = delete;
  explicit CItemMovie(
      QLabel* _label, 
      QSharedPointer<LIMovieAccess> _movieAccess, 
      const QPalette& _palette,
      Qt::Alignment _alignment): 
    IItem(_label), 
    mspMovieAccess(_movieAccess),
    mAlignment(_alignment),
    mPalette(_palette)
  {
  }

  virtual void install() override
  {
    /* QPalette mPalettePr = mpLabel->palette(); */
    mpLabel->setPalette(mPalette);

    mAlignmentPr = mpLabel->alignment();
    mpLabel->setAlignment(mAlignment);
    mpLabel->clear();
    mpLabel->setMovie(mspMovieAccess->getMovie());
    mspMovieAccess->start();
    mpLabel->adjustSize();
  }

  virtual void uninstall() override
  {
    mpLabel->clear();
    /* mpLabel->setPalette(mPalettePr); */
    mpLabel->setAlignment(mAlignmentPr);
    mspMovieAccess->stop();
  }
};

/* //==============================================================================CItemPixmap */
/* #include <QPixmap> */
/* class CItemPixmap: public IItem */
/* { */
/* public: */
/*     QPixmap mPixmap; */
/* public: */
/*     CItemPixmap() = delete; */
/*     CItemPixmap(QLabel* _label, const QPixmap& _pixmap) : IItem(_label), mPixmap(_pixmap){} */

/*     virtual void install() override */
/*     { */
/*         mpLabel->setPixmap(mPixmap); */
/*         mpLabel->adjustSize(); */
/*     } */

/*     virtual void uninstall() override */
/*     { */
/*         mpLabel->clear(); */
/*     } */
/* }; */

//==============================================================================SOwnData
//Собственные данные класса.
struct SOwnData
{
    //Карта соответствий прочитанных данных и выводимых строк.
    QSharedPointer<IItem> undefItem;
    QSharedPointer<IItem> currentItem;
    QMap<QString, QSharedPointer<IItem>> mItemMap;
    SOwnData(){}
};

//------------------------------------------------------------------------------
#define L_OWNDATA(_data) static_cast<SOwnData*>(_data)

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::
    CReadListener::
        CReadListener(LCQRemComboLabel& _label) : mLabel(_label)
{

}

//------------------------------------------------------------------------------dataIsRead
void LCQRemComboLabel::
        CReadListener::
            dataIsRead( QSharedPointer<QByteArray> _data, 
                        LERemoteDataStatus _status)
{
    QSharedPointer<IItem> item;

    if(_status != LERemoteDataStatus::DS_OK)
    {
        item = L_OWNDATA(mLabel.mpOwnData)->undefItem;
        /* mLabel.setEnabled(false); */
    }
    else
    {
        /* mLabel.setEnabled(true); */
        auto it = L_OWNDATA(mLabel.mpOwnData)->
            mItemMap.find(mLabel.mspFormatter->toString(*_data));

        if(it == L_OWNDATA(mLabel.mpOwnData)->
                mItemMap.end())
        {
            item = L_OWNDATA(mLabel.mpOwnData)->undefItem;
        }
        else
        {
            item = it.value();
        }
    }

    if((L_OWNDATA(mLabel.mpOwnData)->currentItem != item))
    {
        if(!L_OWNDATA(mLabel.mpOwnData)->currentItem.isNull())
            L_OWNDATA(mLabel.mpOwnData)->currentItem->uninstall();
        L_OWNDATA(mLabel.mpOwnData)->currentItem = item;
        item->install();
    }
}

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::LCQRemComboLabel(const QString& _dataName,
                         QSharedPointer<LIRemoteDataSource> _dataSource,
                         QSharedPointer<LCStringDataFormatterBase> _formatter,
                         QWidget* _parent) :    QLabel(_parent),
                                                mDataName(_dataName),
                                                mspFormatter(_formatter)
{
    mpOwnData = new SOwnData();
    setText(mspFormatter->undefStateString());
    addItem(mspFormatter->undefStateString(), QString(), font(), palette(), alignment());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataName);
    mDataReader->setDataReadListener(mDataListener);
    mSizeHint = QLabel::sizeHint();
}

//------------------------------------------------------------------------------~LCQRemComboLabel
LCQRemComboLabel::~LCQRemComboLabel()
{
    delete L_OWNDATA(mpOwnData);
}

//------------------------------------------------------------------------------setActive
void LCQRemComboLabel::setActive(bool _flag)
{
    if(_flag)
    {
        mDataReader->connectToSource();
    }
    else
    {
        mDataReader->disconnectFromSource();
    }
}

//------------------------------------------------------------------------------addItem
void LCQRemComboLabel::addItem(
    const QString&  _text, 
    const QString&  _val, 
    const QFont&    _font,
    const QPalette& _palette,
    Qt::Alignment _alignment)
{
    auto pl = QSharedPointer<QLabel>(new QLabel(_text));
    pl->setFont(_font);
    auto item = new CItemText(this, _text, _font, _palette, _alignment);

    if(_val.isNull())
    {
        L_OWNDATA(mpOwnData)->undefItem =  
            QSharedPointer<IItem>(item);
    }
    else
    {
        L_OWNDATA(mpOwnData)->mItemMap.insert(_val, 
                QSharedPointer<IItem>(item));
    }
    setSizeHint(pl->sizeHint());
    resize(mSizeHint);
}

//------------------------------------------------------------------------------addItem
void LCQRemComboLabel::addItem(QSharedPointer<LIMovieAccess> _movieAccess, 
    const QString& _val,
    const QPalette& _palette,
    Qt::Alignment _alignment)
{
    auto item = new CItemMovie(this, _movieAccess, _palette, _alignment);
    if(_val.isNull())
    {
        L_OWNDATA(mpOwnData)->undefItem =  
            QSharedPointer<IItem>(item);
    }
    else
    {
        L_OWNDATA(mpOwnData)->mItemMap.insert(_val, 
                QSharedPointer<IItem>(item));
    }
    setSizeHint(item->mspMovieAccess->getSize());
    resize(mSizeHint);
}

/* //------------------------------------------------------------------------------addItem */
/* void LCQRemComboLabel::addItem(const QPixmap& _pixmap, const QString& _val) */
/* { */
/*     auto item = new CItemPixmap(this, _pixmap); */
/*     if(_val.isNull()) */
/*     { */
/*         L_OWNDATA(mpOwnData)->undefItem = */  
/*             QSharedPointer<IItem>(item); */
/*     } */
/*     else */
/*     { */
/*         L_OWNDATA(mpOwnData)->mItemMap.insert(_val, */ 
/*                 QSharedPointer<IItem>(item)); */
/*     } */
/*     setSizeHint(_pixmap.size()); */
/*     resize(mSizeHint); */
/* } */

//------------------------------------------------------------------------------event
bool LCQRemComboLabel::event(QEvent *_event)
{
    bool ret = false;
    switch(_event->type())
    {
    case QEvent::Type::Show:
        mDataReader->connectToSource();
        ret = true;
        break;
    case QEvent::Type::Hide:
        mDataReader->disconnectFromSource();
        ret = true;
        break;
    default:
        break;
    }
    QLabel::event(_event);
    return ret;
}

//------------------------------------------------------------------------------
void LCQRemComboLabel::setSizeHint(const QSize& _size)
{
    int width = mSizeHint.width();
    int height = mSizeHint.height();

    if(_size.width() > width)
    {
        width = _size.width();
    }

    if(_size.height() > height)
    {
        height = _size.height();
    }

    mSizeHint = QSize(width, height);
}

