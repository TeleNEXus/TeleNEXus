#include "lcqremwritebutton.h"
#include "LIRemoteDataWriter.h"
#include <QDebug>

//==============================================================================
void LCQRemWriteButton::CWriteListener::dataIsWrite(LERemoteDataStatus _status)
{
    Q_UNUSED(_status); 
    qDebug() << "LCQRemWriteDataButton::dataIsWrite--------";
}

//==============================================================================
LCQRemWriteButton::LCQRemWriteButton(
                QSharedPointer<LIRemoteDataSource> _dataSource,
                const QString&      _dataNameWrite, 
                const QByteArray&   _data, 
                const QString&      _text, 
                QPushButton*        _parent) : 
    QPushButton(_text, _parent),
    mDataNameWrite(_dataNameWrite),
    mData(_data)

{
    mDataWriter = _dataSource->createWriter();

    if(mDataWriter.isNull()) return;

    mDataWriteListener = 
        QSharedPointer<LIRemoteDataWriteListner>(new CWriteListener()); 

    mDataWriter->setDataName(_dataNameWrite);
    mDataWriter->setDataSource(_dataSource);
    mDataWriter->setDataWriteListener(mDataWriteListener);

    //Подключение обработки сигнала нажания.
    connect(this, &LCQRemWriteButton::pressed, 
            [&]()
            {
                mDataWriter->writeRequest(mData);             
            });
}

//------------------------------------------------------------------------------
