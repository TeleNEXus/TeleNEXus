#include "lcqremcombolabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMap>

//==============================================================================SOwnData
//Собственные данные класса.
struct SOwnData
{
    //Карта соответствий прочитанных данных и выводимых строк.
    QMap<QString, QString> mDataMap;
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
    if(_status != LERemoteDataStatus::DS_OK)
    {
        mLabel.setText(mLabel.mFormatter.data()->undefStateString());
        return;
    }
    QMap<QString, QString>::iterator it = 
        L_OWNDATA(mLabel.mpOwnData)->mDataMap.find(
                                    mLabel.mFormatter.data()->toString(*_data));
    if (it == L_OWNDATA(mLabel.mpOwnData)->mDataMap.end())
    {
        mLabel.setText(mLabel.mFormatter->getWrongStateString());
        return;
    }
    mLabel.setText(*it);
}


//==============================================================================LCQRemComboLabel
LCQRemComboLabel::LCQRemComboLabel(const QString& _dataName,
                         QSharedPointer<LIRemoteDataSource> _dataSource,
                         QSharedPointer<LCStringDataFormatterBase> _formatter,
                         QWidget* _parent) :    QLabel(_parent),
                                                mDataName(_dataName),
                                                mFormatter(_formatter)
{
    mpOwnData = new SOwnData();
    setText(mFormatter.data()->undefStateString());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataName);
    mDataReader->setDataReadListener(mDataListener);

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
void LCQRemComboLabel::addItem(const QString& _name, const QString& _val)
{
    L_OWNDATA(mpOwnData)->mDataMap.insert(_val, _name);
}

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
