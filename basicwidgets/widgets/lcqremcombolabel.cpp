#include "lcqremcombolabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMap>

#define __WRONG_DATA_STRING "####"

//==============================================================================SOwnData
struct SOwnData
{
    QMap<QString, QString> mDataMap;
    SOwnData(){}
};
#define __L_OWNDATA static_cast<SOwnData*>(mpOwnData)

//==============================================================================LCQRemComboLabel
LCQRemComboLabel::CReadListener::CReadListener(LCQRemComboLabel& _label) : mLabel(_label)
{

}

void LCQRemComboLabel::
        CReadListener::
            dataIsRead( QSharedPointer<QByteArray> _data, 
                        LERemoteDataStatus _status)
{

    //TODO: Добавить логику отображения.
    if(_status != LERemoteDataStatus::DS_OK)
    {
        mLabel.setText(mLabel.mFormatter.data()->undefStateString());
        return;
    }
    mLabel.setText(mLabel.mFormatter.data()->toString(*_data));
}


//==============================================================================LCQRemComboLabel
LCQRemComboLabel::LCQRemComboLabel(QWidget* _parent) : QLabel(_parent)
{
    mpOwnData = new SOwnData();
    setText("LCQRemComboLabel");
}

LCQRemComboLabel::LCQRemComboLabel(QString _text, QWidget* _parent) : QLabel(_text, _parent)
{
}

LCQRemComboLabel::LCQRemComboLabel(const QString& _dataName,
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
    mDataReader->setDataSource(_dataSource);
    mDataReader->setDataReadListener(mDataListener);

}

LCQRemComboLabel::~LCQRemComboLabel()
{
    delete __L_OWNDATA;
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

//------------------------------------------------------------------------------
void LCQRemComboLabel::addItem(const QString& _name, const QString& _val)
{
    __L_OWNDATA->mDataMap.insert(_name, _val);
}

//------------------------------------------------------------------------------
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
