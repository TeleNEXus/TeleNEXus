#include "lcqmodbusdatawriter.h"
#include "LCRemoteDataWriteListnerInterface.h"

namespace modbus
{

//======================================================================================================CQEventDataWrite
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusDataWriter::CQEventDataIsWrite);

LCQModbusDataWriter::
        CQEventDataIsWrite::
            CQEventDataIsWrite(ERemoteDataStatus _status) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                            mStatus(_status)
{
}

//===================================================================================================LCQModbusDataWriter
LCQModbusDataWriter::LCQModbusDataWriter(QObject *_parent) : QObject(_parent)
{

}

LCQModbusDataWriter::LCQModbusDataWriter(   QWeakPointer<LCQModbusDataSource> _dataSource,
                                            QObject* _parent) : QObject(_parent),
                                                                mDataSource(_dataSource)
{

}

//-----------------------------------------------------------------------------------------------------------setDataName
void LCQModbusDataWriter::setDataName(const QString& _dataName)
{
    mDataName = _dataName;
}

//----------------------------------------------------------------------------------------------------------writeRequest
void LCQModbusDataWriter::writeRequest(const QByteArray& _data)
{
    QSharedPointer<LCQModbusDataSource> sp = mDataSource.lock();
    if(sp.isNull())
    {
        QSharedPointer<LCRemoteDataWriteListnerInterface> listener = mwpWriteListener.lock();
        listener->dataIsWrite(ERemoteDataStatus::DS_WRONG);
    }
    else
    {
        sp->write(mDataName, _data, this);
    }
}

//-----------------------------------------------------------------------------------------------------------customEvent
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
