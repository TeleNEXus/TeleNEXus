#include "lcqremwritebutton.h"
#include "LIRemoteDataWriter.h"
#include <QDebug>


//==============================================================================
LCQRemWriteButton::CWriteListener::CWriteListener(
        const QString& _dataName):
    mDataName(_dataName)
{
}

//------------------------------------------------------------------------------
void LCQRemWriteButton::CWriteListener::dataIsWrite(LERemoteDataStatus _status)
{
    Q_UNUSED(_status); 
    qDebug() << "LCQRemWriteDataButton::dataIsWrite data name = " << mDataName;
}

//==============================================================================
void LCQRemWriteButton::SWriteSet::write()
{
    mWriter->writeRequest(mData);
}

//==============================================================================
LCQRemWriteButton::LCQRemWriteButton(
                const QString&      _text, 
                QPushButton*        _parent) : 
    QPushButton(_text, _parent)
{
    //Подключение обработки сигнала нажания.
    connect(this, &LCQRemWriteButton::pressed, 
            [&]()
            {
            qDebug() << "LCQRemWriteButton pressed";
            for(    auto it = mListWriteSets.begin(); 
                    it != mListWriteSets.end(); 
                    it++)
            {
            qDebug() <<  "LCQRemWriteButton write request data name = " 
            << it->mDataName;
            it->write();
            }
            });
}

//------------------------------------------------------------------------------
void LCQRemWriteButton::addDataWrite(
        QSharedPointer<LIRemoteDataSource> _source, 
        const QString _dataName, 
        const QByteArray _data)
{
    SWriteSet write_set;
    if(_source.isNull()) return;
    write_set.mListener = 
        QSharedPointer<LIRemoteDataWriteListner>(new CWriteListener(_dataName));
    write_set.mData = _data;
    write_set.mWriter = _source->createWriter();
    write_set.mWriter->setDataName(_dataName);
    write_set.mWriter->setDataSource(_source);
    write_set.mWriter->setDataWriteListener(write_set.mListener);
    write_set.mDataName = _dataName;
    mListWriteSets << write_set;
}
