#include "lcqremwritebutton.h"
#include "LIRemoteDataWriter.h"
#include <QDebug>

//==============================================================================CWriteListener
LCQRemWriteButton::CWriteListener::CWriteListener(
        const QString& _dataName):
    mDataName(_dataName)
{
}

//------------------------------------------------------------------------------dataIsWrite
void LCQRemWriteButton::CWriteListener::dataIsWrite(LERemoteDataStatus _status)
{
    Q_UNUSED(_status); 
}

//==============================================================================SWriteSet
void LCQRemWriteButton::SWriteSet::write()
{
    mWriter->writeRequest(mData);
}

//==============================================================================LCQRemWriteButton
LCQRemWriteButton::LCQRemWriteButton(QPushButton* _parent)
{
    LCQRemWriteButton("RemWriteButton", _parent);
}

LCQRemWriteButton::LCQRemWriteButton(
                const QString&      _text, 
                QPushButton*        _parent) : 
    QPushButton(_text, _parent)
{
    QString text = _text;
    if(text.isNull()) text = "RemWriteButton";
    //Подключение обработки сигнала нажания.
    connect(this, &LCQRemWriteButton::pressed, 
            [&]()
            {
            for(    auto it = mListWriteSets.begin(); 
                    it != mListWriteSets.end(); 
                    it++)
            {
            it->write();
            }
            });
}

//------------------------------------------------------------------------------addDataWrite
void LCQRemWriteButton::addDataWrite(
        QSharedPointer<LIRemoteDataSource>  _source, 
        const QString&                      _dataName, 
        const QByteArray&                   _data)
{
    SWriteSet write_set;

    if(_source.isNull()) return;

    write_set.mListener = 
        QSharedPointer<LIRemoteDataWriteListener>(new CWriteListener(_dataName));

    write_set.mData     = _data;
    write_set.mDataName = _dataName;
    write_set.mWriter   = 
      _source->createWriter(write_set.mDataName, write_set.mListener);
    mListWriteSets << write_set;
}
