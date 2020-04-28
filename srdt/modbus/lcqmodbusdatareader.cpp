#include "lcqmodbusdatareader.h"
#include "LCRemoteDataReadListnerInterface.h"
#include "lcqmodbusdatasource.h"

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


LCQModbusDataReader::~LCQModbusDataReader()
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQModbusDataSource*)sp.data())->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------setDataName
void LCQModbusDataReader::setDataName(const QString& _dataName)
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        ((LCQModbusDataSource*)sp.data())->disconnectReader(this);
    }
    mDataName = _dataName;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataReader::setDataReadListener(QWeakPointer<LCRemoteDataReadListnerInterface> _listener)
{
    QSharedPointer<LCRemoteDataReadListnerInterface> sp = _listener.lock();
    if(!sp.isNull()) mpReadListener = sp;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataReader::setDataSource(QWeakPointer<LCRemoteDataSourceInterface> _source)
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = _source.lock();
    if(!sp.isNull())
    {
        if(dynamic_cast<LCQModbusDataSource*>(sp.data()))
        {
            mDataSource = sp;
        }
    }
}

//-----------------------------------------------------------------------------------------------------------readRequest
void LCQModbusDataReader::readRequest()
{
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQModbusDataSource*)sp.data())->read(mDataName, this);
}

//-------------------------------------------------------------------------------------------------------connectToSource
void LCQModbusDataReader::connectToSource()
{
    qDebug() << "LCQModbusDataReader::connectToSource:" << mDataName;
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQModbusDataSource*)sp.data())->connectReader(mDataName, this);
}

//--------------------------------------------------------------------------------------------------disconnectFromSource
void LCQModbusDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    QSharedPointer<LCRemoteDataSourceInterface> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQModbusDataSource*)sp.data())->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQModbusDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    QSharedPointer<LCRemoteDataReadListnerInterface> listener = mpReadListener.lock();
    if(!listener.isNull())
    {
        listener->dataIsRead(e->mData, e->mStatus);
    }
}

}//namespace
