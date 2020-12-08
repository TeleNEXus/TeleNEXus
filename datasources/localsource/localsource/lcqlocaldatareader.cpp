#include "lcqlocaldatareader.h"
#include "LIRemoteDataReadListener.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataReader::CQEventDataIsRead);

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead(
        const QByteArray& _data,
        LERemoteDataStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray(_data)),
    mStatus(_status)
{
}

//------------------------------------------------------------------------------
LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead( 
        LERemoteDataStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================LCQRemoteDataListener
LCQLocalDataReader::LCQLocalDataReader() :   
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
LCQLocalDataReader::~LCQLocalDataReader()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->disconnectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setDataName(const QString& _dataName)
{
    auto sp = mwpDataSource.lock();
    if(!sp.isNull()) sp->disconnectReader(mwpThis);
    mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setDataReadListener(
        QWeakPointer<LIRemoteDataReadListener> _listener)
{
    auto sp = _listener.lock();
    if(!sp.isNull()) mwpReadListener = sp;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::readRequest()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->read(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::connectToSource()
{
    qDebug() << "LCQLocalDataReader::connectToSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->connectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::disconnectFromSource()
{
    qDebug() << "LCQLocalDataReader::disconnectFromSource():" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->disconnectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setReadData(const QByteArray& _data)
{
  QCoreApplication::postEvent(this, 
      new CQEventDataIsRead(_data, LERemoteDataStatus::DS_OK));
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setReadData(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsRead(_status));
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataReader> LCQLocalDataReader::create(
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp = QSharedPointer<LCQLocalDataReader>(new LCQLocalDataReader());
  sp->mwpDataSource = _dataSource;
  sp->mwpThis = sp;
  return sp;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    auto listener = mwpReadListener.lock();

    if(!listener.isNull())
    {
        listener->dataIsRead(e->mspData, e->mStatus);
    }
}
