#include "lcqremlabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <qnamespace.h>

#define __WRONG_DATA_STRING "####"


//==============================================================================LCQRemLabel
LCQRemLabel::CReadListener::CReadListener(LCQRemLabel& _label) : mLabel(_label)
{

}

void LCQRemLabel::CReadListener::dataIsRead(QSharedPointer<QByteArray>  _data, 
                                            LERemoteDataStatus          _status)
{
    if(_status != LERemoteDataStatus::DS_OK)
    {
        mLabel.setText(mLabel.mFormatter.data()->undefStateString());
        return;
    }
    QString str = mLabel.mFormatter.data()->toString(*_data);
    mLabel.setText(str);
}

//==============================================================================LCQRemLabel
LCQRemLabel::LCQRemLabel(QWidget* _parent) : QLabel(_parent)
{
    setText("LCQRemLabel");
}

LCQRemLabel::LCQRemLabel(QString _text, QWidget* _parent) : QLabel(_text, _parent)
{
}

LCQRemLabel::LCQRemLabel(const QString& _dataName,
                         QSharedPointer<LIRemoteDataSource> _dataSource,
                         QSharedPointer<LCStringDataFormatterBase> _formatter,
                         QWidget* _parent) :    QLabel(_parent),
                                                mDataName(_dataName),
                                                mFormatter(_formatter)
{
    setText(mFormatter.data()->undefStateString());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataName);
    mDataReader->setDataReadListener(mDataListener);

}

LCQRemLabel::~LCQRemLabel()
{
}

//------------------------------------------------------------------------------setActive
void LCQRemLabel::setActive(bool _flag)
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

//------------------------------------------------------------------------------event
bool LCQRemLabel::event(QEvent *_event)
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

/* //------------------------------------------------------------------------------event */
/* void LCQRemLabel::paintEvent(QPaintEvent* _event) */ 
/* { */
/*     /1* QFontMetrics fm(font()); *1/ */
/*    /1* setMinimumWidth(fm.size(Qt::TextFlag::TextSingleLine, text()).width()); *1/ */
/*    /1* resize( *1/ */
/*   /1* setUpdatesEnabled(false); *1/ */
/*   /1* adjustSize(); *1/ */
/*   /1* setUpdatesEnabled(true); *1/ */
/*   QLabel::paintEvent(_event); */
/*   /1* this->adjustSize(); *1/ */
/*   this->updateGeometry(); */
/* } */

/* QSize LCQRemLabel::sizeHint() const */
/* { */
/*   QFontMetrics fm(font()); */
/*   QSize s =fm.size(Qt::TextFlag::TextSingleLine, text()); */ 
/*   qDebug() << "LCQRemLabel size hint = " << s; */

/*   return s; */
/* } */
