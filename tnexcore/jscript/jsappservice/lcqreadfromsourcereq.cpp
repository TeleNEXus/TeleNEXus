#include "lcqreadfromsourcereq.h"
#include "lcxmlmain.h"
#include "LIApplication.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CReadListener
LCQReadFromSourceReq::CReadListener::CReadListener(LCQReadFromSourceReq* _req) :
  mpRequest(_req)
{
}

void LCQReadFromSourceReq::CReadListener::dataIsRead(
    QSharedPointer<QByteArray>  _data, 
    LERemoteDataStatus          _status)
{
  if(_status == LERemoteDataStatus::DS_OK) mpRequest->mRetData = *_data;
  mpRequest->mWaitCond.wakeOne();
}

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQReadFromSourceReq::CEventBase);

LCQReadFromSourceReq::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventRead
LCQReadFromSourceReq::CEventRead::CEventRead()
{
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::CEventRead::handle(LCQReadFromSourceReq* _sender)
{

  auto source = LCXmlMain::getApplicationInterface().getDataSource(_sender->mSourceId);
  if(source.isNull()) _sender->mWaitCond.wakeOne();
  _sender->mspDataReader = source->createReader();
  _sender->mspDataReader->setDataName(_sender->mDataId);
  _sender->mspDataReader->setDataReadListener(_sender->mspDataListener);
  _sender->mspDataReader->readRequest();

  /* QString ret_string = QString("Read from Source(%1) data(%2)"). */
  /*   arg(_sender->mSourceId). */
  /*   arg(_sender->mDataId); */

  /* _sender->mRetData = QByteArray( */
  /*     ret_string.toStdString().c_str(), ret_string.length()); */

  /* _sender->mWaitCond.wakeOne(); */
}

static void requestDeleter(LCQReadFromSourceReq* _req)
{
  qDebug() << "Read Request Deleter";
  _req->deleteLater();
}

//==============================================================================LCQReadFromSourceReq
LCQReadFromSourceReq::LCQReadFromSourceReq(
    const QString& _sourceId,
    const QString& _dataId) :
  QObject(nullptr),
  mSourceId(_sourceId),
  mDataId(_dataId),
  mspDataListener(new CReadListener(this))
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQReadFromSourceReq> LCQReadFromSourceReq::create(
    const QString& _sourceId,
    const QString& _dataId,
    QThread* _thread)
{
  LCQReadFromSourceReq* req = new LCQReadFromSourceReq(_sourceId, _dataId);
  req->moveToThread(_thread);
  return QSharedPointer<LCQReadFromSourceReq>(req, requestDeleter);
}


//------------------------------------------------------------------------------
LCQReadFromSourceReq::~LCQReadFromSourceReq()
{
}

//------------------------------------------------------------------------------
QByteArray LCQReadFromSourceReq::getData()
{
  mMutexEvent.lock();

  QCoreApplication::postEvent(this, new CEventRead());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  return mRetData;
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::customEvent(QEvent* _event)
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

