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

//==============================================================================
static void pointerDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LCQRemoteDataListener
LCQLocalDataReader::LCQLocalDataReader(
    const QString& _dataName, 
    QSharedPointer<LIRemoteDataReadListener> _readListener,
    QSharedPointer<LCQLocalSourceHiden> _dataSource) :   
  QObject(nullptr),
  mDataName(_dataName),
  mwpReadListener(_readListener),
  mwpDataSource(_dataSource)
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
QSharedPointer<LCQLocalDataReader> LCQLocalDataReader::create(
    const QString& _dataName, 
    QSharedPointer<LIRemoteDataReadListener> _readListener,
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp = QSharedPointer<LCQLocalDataReader>(
      new LCQLocalDataReader( _dataName, _readListener, _dataSource), 
      pointerDeleter);
  sp->mwpThis = sp;
  return sp;
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
void LCQLocalDataReader::notifyListener(const QByteArray& _data)
{
  QCoreApplication::postEvent(this, 
      new CQEventDataIsRead(_data, LERemoteDataStatus::DS_OK));
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsRead(_status));
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
