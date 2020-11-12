﻿#include "lcqremcombolabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include "LIMovieAccess.h"
#include <QMovie>

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
public:
    QString mText;
public:
    CItemText() = delete;
    CItemText(QLabel* _label, const QString& _text): IItem(_label), mText(_text){}

    virtual void install() override
    {
        mpLabel->setText(mText);
        mpLabel->adjustSize();
    }

    virtual void uninstall() override
    {
        mpLabel->clear();
    }
};

//==============================================================================CItemMovie
class CItemMovie: public IItem
{
public:
    QSharedPointer<LIMovieAccess> mspMovieAccess;
public:
    CItemMovie() = delete;
    CItemMovie(QLabel* _label, QSharedPointer<LIMovieAccess> _movieAccess): 
        IItem(_label), 
        mspMovieAccess(_movieAccess){}

    virtual void install() override
    {
        mpLabel->clear();
        mpLabel->setMovie(mspMovieAccess->getMovie());
        mspMovieAccess->start();
        mpLabel->adjustSize();
    }

    virtual void uninstall() override
    {
        mpLabel->clear();
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



    /* if(_status != LERemoteDataStatus::DS_OK) */
    /* { */
    /*     mLabel.setText(mLabel.mspFormatter->undefStateString()); */
    /*     return; */
    /* } */

    
    /* auto it = L_OWNDATA(mLabel.mpOwnData)-> */
    /*     mItemMap.find(mLabel.mspFormatter->toString(*_data)); */


    /* if (it == L_OWNDATA(mLabel.mpOwnData)->mItemMap.end()) */
    /* { */
    /*     if(L_OWNDATA(mLabel.mpOwnData)->undefItem.isNull()) */
    /*     { */
    /*         mLabel.setText(mLabel.mspFormatter->getWrongStateString()); */
    /*     } */
    /*     else */
    /*     { */
    /*         L_OWNDATA(mLabel.mpOwnData)->undefItem->install(); */
    /*     } */
    /*     return; */
    /* } */

    /* if((L_OWNDATA(mLabel.mpOwnData)->currentItem != it.value())) */
    /* { */
    /*     if(!L_OWNDATA(mLabel.mpOwnData)->currentItem.isNull()) */
    /*         L_OWNDATA(mLabel.mpOwnData)->currentItem->uninstall(); */
    /*     L_OWNDATA(mLabel.mpOwnData)->currentItem = it.value(); */
    /*     it.value()->install(); */
    /* } */
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
    addItem(mspFormatter->undefStateString(), QString());
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
void LCQRemComboLabel::addItem(const QString& _text, const QString& _val)
{
    auto pl = QSharedPointer<QLabel>(new QLabel(_text));
    auto item = new CItemText(this, _text);

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
    const QString& _val)
{
    auto item = new CItemMovie(this, _movieAccess);
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

