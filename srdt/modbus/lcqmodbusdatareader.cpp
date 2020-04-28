#include "lcqmodbusdatareader.h"
#include "LCRemoteDataReadListnerInterface.h"

#include <QDebug>
namespace modbus
{
//=======================================================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusDataReader::CQEventDataIsRead);

LCQModbusDataReader::
        CQEventDataIsRead::
            CQEventDataIsRead(const QByteArray& _data,
                              ERemoteDataStatus _status) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                           mData(new QByteArray(_data)),
                                                           mStatus(_status)
{
}

LCQModbusDataReader::
        CQEventDataIsRead::
            CQEventDataIsRead(ERemoteDataStatus _status) :  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                            mData(new QByteArray),
                                                            mStatus(_status)
{
}

//=================================================================================================LCQRemoteDataListener
LCQModbusDataReader::LCQModbusDataReader(QObject* _parent) :   QObject(_parent)
{
}

LCQModbusDataReader::
    LCQModbusDataReader(  const QString& _dataName,
                            QWeakPointer<LCQModbusDataSource> _dataSource,
                            QObject* _parent) : QObject(_parent),
                                                mDataName(_dataName),
                                                mDataSource(_dataSource),
                                                mpReadListener(nullptr)
{

}

LCQModbusDataReader::~LCQModbusDataReader()
{
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------setDataName
void LCQModbusDataReader::setDataName(const QString& _dataName)
{
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        sp->disconnectReader(this);
    }
    mDataName = _dataName;
}

void LCQModbusDataReader::setDataReadListener(QWeakPointer<LCRemoteDataReadListnerInterface> _listener)
{
    mpReadListener = _listener;
}

//-----------------------------------------------------------------------------------------------------------readRequest
void LCQModbusDataReader::readRequest()
{
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp->read(mDataName, this);
}

//-------------------------------------------------------------------------------------------------------connectToSource
void LCQModbusDataReader::connectToSource()
{
    qDebug() << "LCQModbusDataReader::connectToSource:" << mDataName;
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp->connectReader(mDataName, this);
}

//--------------------------------------------------------------------------------------------------disconnectFromSource
void LCQModbusDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQModbusDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != LCQModbusDataSource::CQEventDataIsRead::msExtendedEventType) return;
    LCQModbusDataSource::CQEventDataIsRead *e = dynamic_cast<LCQModbusDataSource::CQEventDataIsRead*>(_event);
    if(e == nullptr) return;
    QSharedPointer<LCRemoteDataReadListnerInterface> listener = mpReadListener.lock();
    if(!listener.isNull())
    {
        listener->dataIsRead(e->mData, e->mStatus);
    }
}

}//namespace
