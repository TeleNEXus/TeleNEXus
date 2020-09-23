#include "lcqremcombobox.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include <QCoreApplication>
#include <QDebug>
#include <qcombobox.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <QKeyEvent>
//==============================================================================CReadListener
LCQRemComboBox::
    CReadListener::
        CReadListener(LCQRemComboBox& _combobox) :  mOwner(_combobox)
{
}

//------------------------------------------------------------------------------
void LCQRemComboBox::
        CReadListener::
            dataIsRead( QSharedPointer<QByteArray>  _data, 
                        LERemoteDataStatus          _status)
{
    switch(_status)
    {
    case LERemoteDataStatus::DS_OK:
        mOwner.setCurrentIndex(
                mOwner.findData( 
                    mOwner.mFormatter->
                                toString( *_data.data())) ); 
        mOwner.setEnabled(true);
        break;

    case LERemoteDataStatus::DS_WRONG:
        mOwner.setCurrentIndex(-1);
        mOwner.setEnabled(true);
        break;

    case LERemoteDataStatus::DS_UNDEF:
        mOwner.setCurrentIndex(-1);
        mOwner.setEnabled(false);
    default:
        break;
    }
}


//==============================================================================LCQRemComboBox
//LCQRemComboBox::LCQRemComboBox(QWidget* _parent) : QComboBox(_parent)
//{
//}

//------------------------------------------------------------------------------
LCQRemComboBox::LCQRemComboBox( 
        const QString&                              _dataNameRead,
        const QString&                              _dataNameWrite,
        QSharedPointer<LIRemoteDataSource>          _dataSource,
        QSharedPointer<LCStringDataFormatterBase>   _formatter,
        QWidget* _parent):  QComboBox(_parent),
                            mFormatter(_formatter)
{

    mDataReadListener = QSharedPointer<CReadListener>(new CReadListener(*this));

    mDataReader = _dataSource->createReader();
    mDataReader->setDataName(_dataNameRead);
    mDataReader->setDataSource(_dataSource);
    mDataReader->setDataReadListener(mDataReadListener);

    mDataWriter = _dataSource->createWriter();
    mDataWriter->setDataName(_dataNameWrite);
    mDataWriter->setDataSource(_dataSource);

    this->setEnabled(false);

    connect(this, 
            static_cast <void(LCQRemComboBox::*)(int)> 
                    (&LCQRemComboBox::activated),
            [&](int index)
            {
                Q_UNUSED(index);
                mDataWriter->writeRequest(
                        mFormatter->toBytes(currentData().toString()));
            });
}

//------------------------------------------------------------------------------
LCQRemComboBox::~LCQRemComboBox()
{
}

//------------------------------------------------------------------------------
bool LCQRemComboBox::event(QEvent *_event)
{
    bool ret = true;
    switch(_event->type())
    {
    case QEvent::Type::Show:
        mDataReader->connectToSource();
        setCurrentIndex(-1);
        ret = false;
        break;

    case QEvent::Type::Hide:
        mDataReader->disconnectFromSource();
        ret = false;
        break;

    case QEvent::Type::KeyRelease:
        //Очиска фокуса видета при нажатии клавиши Escape.
        if( static_cast<QKeyEvent*>(_event)->key() == Qt::Key_Escape)
        {
            this->clearFocus();
            ret = false;
        }
        break;
    default:
        break;
    }
    if(ret) QComboBox::event(_event);
    return ret;
}

