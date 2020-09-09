#include "lcqremcombobox.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include <QCoreApplication>
#include <QDebug>
#include <qcombobox.h>
#include <qglobal.h>

//=========================================================================================================CReadListener
LCQRemComboBox::CReadListener::CReadListener(LCQRemComboBox& _label) : mComboBox(_label)
{

}

//---------------------------------------------------------------------------------------------------------------------
void LCQRemComboBox::CReadListener::dataIsRead(QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
{
    Q_UNUSED(_data);
    Q_UNUSED(_status);
    qDebug() << "LCQRemComboBox " << mComboBox.mDataName << ": data is read";
}


//========================================================================================================LCQRemComboBox
LCQRemComboBox::LCQRemComboBox(QWidget* _parent) : QComboBox(_parent)
{
}

LCQRemComboBox::LCQRemComboBox(  const QString&                               _dataName,
                                 QSharedPointer<LIRemoteDataSource>           _dataSource,
                                 QSharedPointer<LCStringDataFormatterBase>    _formatter,
                                 QWidget* _parent) :    QComboBox(_parent),
                                                        mFormatter(_formatter)
{
    this->insertItem(0, mFormatter.data()->undefStateString());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));
    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataName);
    mDataReader->setDataSource(_dataSource);
    mDataReader->setDataReadListener(mDataListener);
    mDataWriter->setDataName(_dataName);
    mDataWriter->setDataSource(_dataSource);
}

LCQRemComboBox::LCQRemComboBox( const QString&                              _dataNameRead,
                                const QString&                              _dataNameWrite,
                                QSharedPointer<LIRemoteDataSource>          _dataSource,
                                QSharedPointer<LCStringDataFormatterBase>   _formatter,
                                QWidget* _parent)
{
    this->insertItem(0, mFormatter.data()->undefStateString());
    mDataListener = QSharedPointer<CReadListener>(new CReadListener(*this));

    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataNameRead);
    mDataReader->setDataSource(_dataSource);

    mDataReader->setDataReadListener(mDataListener);
    mDataWriter->setDataName(_dataNameWrite);
    mDataWriter->setDataSource(_dataSource);
}

//---------------------------------------------------------------------------------------------------------------------
LCQRemComboBox::~LCQRemComboBox()
{
    qDebug() << "LCQRemComboBox::~LCQRemComboBox()";
}

//---------------------------------------------------------------------------------------------------------------------
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
