#include "lcqremcombobox.h"
#include <QCoreApplication>
#include <QDebug>
#include <qcombobox.h>

//=========================================================================================================CReadListener
LCQRemComboBox::CReadListener::CReadListener(LCQRemComboBox& _label) : mComboBox(_label)
{

}
//------------------------------------------------------------------------------------------------------------dataIsRead
void LCQRemComboBox::CReadListener::dataIsRead(QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
{
    qDebug() << "LCQRemComboBox " << mComboBox.mDataName << ": data is read";
}


//========================================================================================================LCQRemComboBox
LCQRemComboBox::LCQRemComboBox(QWidget* _parent) : QComboBox(_parent)
{
}

LCQRemComboBox::LCQRemComboBox(const QString& _dataName,
                         QSharedPointer<LIRemoteDataSource> _dataSource,
                         QSharedPointer<LCStringDataFormatterBase> _formatter,
                         QWidget* _parent) :    QComboBox(_parent),
                                                mDataName(_dataName),
                                                mFormatter(_formatter)
{
    this->insertItem(0, mFormatter.data()->undefStateString());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataName);
    mDataReader->setDataSource(_dataSource);
    mDataReader->setDataReadListener(mDataListener);

}

LCQRemComboBox::~LCQRemComboBox()
{
    qDebug() << "LCQRemComboBox::~LCQRemComboBox()";
}

//-------------------------------------------------------------------------------------------------------------setActive
void LCQRemComboBox::setActive(bool _flag)
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

//-----------------------------------------------------------------------------------------------------------------event
bool LCQRemComboBox::event(QEvent *_event)
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
    QComboBox::event(_event);
    return ret;
}
