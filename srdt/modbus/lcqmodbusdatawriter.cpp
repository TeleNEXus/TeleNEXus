#include "lcqmodbusdatawriter.h"
#include "lcqmodbusdatasource.h"
#include "LCRemoteDataWriteListnerInterface.h"

namespace modbus
{

//======================================================================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusDataWriter::CQEventDataIsWrite);

LCQModbusDataWriter::
        CQEventDataIsWrite::
            CQEventDataIsWrite(ERemoteDataStatus _status) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                            mStatus(_status)
{
}

//======================================================================================================================
LCQModbusDataWriter::LCQModbusDataWriter(QObject *_parent) : QObject(_parent)
{

}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataWriter::setDataName(const QString& _dataName)
{
    mDataName = _dataName;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataWriter::setDataSource(QWeakPointer<LCRemoteDataSourceInterface> _source)
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = _source.lock();
    if(!sp.isNull())
    {
        if(dynamic_cast<LCQModbusDataSource*>(sp.data())) mDataSource = sp;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataWriter::setDataWriteListener(QWeakPointer<LCRemoteDataWriteListnerInterface> _listener)
{
    QSharedPointer<LCRemoteDataWriteListnerInterface> sp = _listener.lock();
    if(!sp.isNull()) mwpWriteListener = sp;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataWriter::writeRequest(const QByteArray& _data)
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(sp.isNull())
    {
        QSharedPointer<LCRemoteDataWriteListnerInterface> listener = mwpWriteListener.lock();
        listener->dataIsWrite(ERemoteDataStatus::DS_WRONG);
    }
    else
    {
        ((LCQModbusDataSource*)sp.data())->write(mDataName, _data, this);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataWriter::customEvent(QEvent *_event)
{
    if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
    {
        QSharedPointer<LCRemoteDataWriteListnerInterface> listener = mwpWriteListener.lock();
        CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
        if(e != nullptr)
        {
            listener->dataIsWrite(ERemoteDataStatus::DS_WRONG);
        }
        else
        {
            listener->dataIsWrite(e->mStatus);
        }
    }
}

} //namespace
