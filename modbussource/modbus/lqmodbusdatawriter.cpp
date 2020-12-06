#include "lqmodbusdatawriter.h"
#include "lqmodbusdatasource.h"
#include "LIRemoteDataWriteListener.h"

namespace modbus
{

//==============================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataWriter::CQEventDataIsWrite);

LQModbusDataWriter::
        CQEventDataIsWrite::
            CQEventDataIsWrite(LERemoteDataStatus _status) : 
                                QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                mStatus(_status)
{
}

//==============================================================================
LQModbusDataWriter::LQModbusDataWriter(
        QWeakPointer<LQModbusDataSource> _dataSource,
        QObject *_parent) : 
    QObject(_parent),
    mDataSource(_dataSource)
{

}

//------------------------------------------------------------------------------
void LQModbusDataWriter::setDataName(const QString& _dataName)
{
    mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::
        setDataWriteListener(QWeakPointer<LIRemoteDataWriteListener> _listener)
{
    QSharedPointer<LIRemoteDataWriteListener> sp = _listener.lock();
    if(!sp.isNull()) mwpWriteListener = sp;
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::writeRequest(const QByteArray& _data)
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull())
    {
        QSharedPointer<LIRemoteDataWriteListener> listener = 
                                                        mwpWriteListener.lock();
        if(!listener.isNull())
        {
            listener->dataIsWrite(LERemoteDataStatus::DS_WRONG);
        }
    }
    else
    {
        ((LQModbusDataSource*)sp.data())->write(mDataName, _data, this);
    }
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::customEvent(QEvent *_event)
{
    if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
    {
        QSharedPointer<LIRemoteDataWriteListener> listener = 
                                                        mwpWriteListener.lock();
        if(!listener.isNull())
        {
            CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
            if(e == nullptr)
            {
                listener->dataIsWrite(LERemoteDataStatus::DS_WRONG);
            }
            else
            {
                listener->dataIsWrite(e->mStatus);
            }
        }
    }
}

} //namespace
