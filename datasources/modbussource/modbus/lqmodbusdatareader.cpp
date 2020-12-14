#include "lqmodbusdatareader.h"
#include "LIRemoteDataReadListener.h"
#include "lqmodbusdatasource.h"

#include <QCoreApplication>
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
    const QString& _dataName,
    QWeakPointer<LIRemoteDataReadListener> _readListener,
    QWeakPointer<LQModbusDataSource> _dataSource) :   
  QObject(nullptr),
  mDataName(_dataName),
  mwpReadListener(_readListener),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LQModbusDataReader::~LQModbusDataReader()
{
}

//==============================================================================create
static void pointerDeleter(LQModbusDataReader* _reader)
{
  qDebug() << "LQModbusDataReader  pointerDeleter";
  _reader->deleteLater();
}

QSharedPointer<LQModbusDataReader> LQModbusDataReader::create(
    const QString& _dataName, 
    QWeakPointer<LIRemoteDataReadListener> _readListener,
    QWeakPointer<LQModbusDataSource> _dataSource)
{
  auto sp = QSharedPointer<LQModbusDataReader>(
      new LQModbusDataReader( _dataName, _readListener, _dataSource), 
      pointerDeleter);
  sp->mwpThis = sp;
  return sp;
}

//------------------------------------------------------------------------------
void LQModbusDataReader::readRequest()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->read(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::connectToSource()
{
    qDebug() << "LCQModbusDataReader::connectToSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->connectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::notifyListener(
    const QByteArray& _data, LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, 
      new CQEventDataIsRead(_data, _status));
}

//------------------------------------------------------------------------------
void LQModbusDataReader::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsRead(_status));
}

//------------------------------------------------------------------------------
void LQModbusDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    auto listener = mwpReadListener.lock();
    if(!listener.isNull())
    {
        listener->dataIsRead(e->mData, e->mStatus);
    }
}

}//namespace
