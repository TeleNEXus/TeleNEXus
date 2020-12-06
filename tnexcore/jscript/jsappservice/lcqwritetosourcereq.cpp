#include "lcqwritetosourcereq.h"
#include "lcxmlmain.h"
#include "LIApplication.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CReadListener
LCQWriteToSource::CWriteListener::CWriteListener(LCQWriteToSource* _req) :
  mpRequest(_req)
{
}

//------------------------------------------------------------------------------
void LCQWriteToSource::CWriteListener::dataIsWrite(LERemoteDataStatus _status)
{
  Q_UNUSED(_status);
  if(_status == LERemoteDataStatus::DS_OK) 
  {
    mpRequest->mWriteDataSize = mpRequest->edWriteData.size();
  }
  mpRequest->mWaitCond.wakeOne();
}

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQWriteToSource::CEventBase);

LCQWriteToSource::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventRead
LCQWriteToSource::CEventWrite::CEventWrite()
{
}

//------------------------------------------------------------------------------
void LCQWriteToSource::CEventWrite::handle(LCQWriteToSource* _sender)
{
  auto source = 
    LCXmlMain::getApplicationInterface().getDataSource(_sender->mSourceId);
  if(source.isNull()) _sender->mWaitCond.wakeOne();
  _sender->mspDataWriter = source->createWriter();
  _sender->mspDataWriter->setDataName(_sender->mDataId);
  _sender->mspDataWriter->setDataWriteListener(_sender->mspDataListener);
  _sender->mspDataWriter->writeRequest(_sender->edWriteData);
}

//==============================================================================requestDeleter
static void requestDeleter(LCQWriteToSource* _req)
{
  qDebug() << "Write Request Deleter";
  _req->deleteLater();
}

//==============================================================================LCQWriteToSource
LCQWriteToSource::LCQWriteToSource(
    const QString&      _sourceId,
    const QString&      _dataId,
    const QByteArray&   _writeData
    ) :
  QObject(nullptr),
  mSourceId(_sourceId),
  mDataId(_dataId),
  edWriteData(_writeData),
  mWriteDataSize(0),
  mspDataListener(new CWriteListener(this))
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQWriteToSource> LCQWriteToSource::create(
    const QString&      _sourceId,
    const QString&      _dataId,
    const QByteArray&   _writeData,
    QThread* _thread)
{
  LCQWriteToSource* req = new LCQWriteToSource(_sourceId, _dataId, _writeData);
  req->moveToThread(_thread);
  return QSharedPointer<LCQWriteToSource>(req, requestDeleter);
}


//------------------------------------------------------------------------------
LCQWriteToSource::~LCQWriteToSource()
{
}

//------------------------------------------------------------------------------
int LCQWriteToSource::writeData()
{
  mMutexEvent.lock();

  QCoreApplication::postEvent(this, new CEventWrite());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  return mWriteDataSize;
}

//------------------------------------------------------------------------------
void LCQWriteToSource::customEvent(QEvent* _event)
{
  mMutexEvent.lock();
  mMutexEvent.unlock();

  if(_event->type() == CEventBase::msExtendedEventType)
  {
    CEventBase* e = dynamic_cast<CEventBase*>(_event);
    if(e == nullptr)
    {
      return;
    }
    e->handle(this);
  }
}

