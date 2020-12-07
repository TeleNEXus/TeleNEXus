#include "lqmodbusdatareader.h"
#include "LIRemoteDataReadListener.h"
#include "lqmodbusdatasource.h"

#include <QDebug>
namespace modbus
{
//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataReader::CQEventDataIsRead);

LQModbusDataReader:: CQEventDataIsRead:: CQEventDataIsRead(
        const QByteArray& _data,
        LERemoteDataStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray(_data)),
    mStatus(_status)
{
}

LQModbusDataReader:: CQEventDataIsRead:: CQEventDataIsRead( 
        LERemoteDataStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================LCQRemoteDataListener
LQModbusDataReader::LQModbusDataReader(
        QWeakPointer<LQModbusDataSource> _dataSource, 
        QObject* _parent) :   
    QObject(_parent),
    mDataSource(_dataSource)
{
}

LQModbusDataReader::~LQModbusDataReader()
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LQModbusDataSource*)sp.data())->disconnectReader(this);
}

//------------------------------------------------------------------------------setDataName
void LQModbusDataReader::setDataName(const QString& _dataName)
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        ((LQModbusDataSource*)sp.data())->disconnectReader(this);
    }
    mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LQModbusDataReader::setDataReadListener(
        QWeakPointer<LIRemoteDataReadListener> _listener)
{
    QSharedPointer<LIRemoteDataReadListener> sp = _listener.lock();
    if(!sp.isNull()) mpReadListener = sp;
}

//------------------------------------------------------------------------------readRequest
void LQModbusDataReader::readRequest()
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LQModbusDataSource*)sp.data())->read(mDataName, this);
}

//------------------------------------------------------------------------------connectToSource
void LQModbusDataReader::connectToSource()
{
    qDebug() << "LCQModbusDataReader::connectToSource:" << mDataName;
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LQModbusDataSource*)sp.data())->connectReader(mDataName, this);
}

//------------------------------------------------------------------------------disconnectFromSource
void LQModbusDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LQModbusDataSource*)sp.data())->disconnectReader(this);
}

//------------------------------------------------------------------------------customEvent
void LQModbusDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    QSharedPointer<LIRemoteDataReadListener> listener = mpReadListener.lock();
    if(!listener.isNull())
    {
        listener->dataIsRead(e->mData, e->mStatus);
    }
}

}//namespace
