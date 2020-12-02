
#include "lcqjsappservice.h"

#include <QCoreApplication>
#include <QMutex>
#include <QThread>
#include <QDebug>

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQJSAppService::CEventBase);

LCQJSAppService::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventReadData
LCQJSAppService::CEventReadData::CEventReadData(
    const QString& _dataId,
    QMutex& _mutex) :
  edDataId(_dataId),
  edMutex(_mutex)
{
}

//------------------------------------------------------------------------------
void LCQJSAppService::CEventReadData::handle(LCQJSAppService* _sender)
{
  Q_UNUSED(_sender);
  qDebug() << "LCQJSAppService::CEventReadData::handle data ID = " << edDataId;
  _sender->mWaitCond.wakeOne();
}

//==============================================================================LCQJSAppService
LCQJSAppService::LCQJSAppService() : 
  QObject(nullptr),
  mpThread(new QThread)
{
  moveToThread(mpThread);
  mpThread->start();
}

//------------------------------------------------------------------------------
LCQJSAppService::~LCQJSAppService()
{
  mpThread->quit();
  mpThread->wait(1000);
  mpThread->deleteLater();
}

//------------------------------------------------------------------------------
void LCQJSAppService::serviceDeleter(LCQJSAppService* _inst)
{
  _inst->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LCQJSAppService> LCQJSAppService::getService()
{
  static QWeakPointer<LCQJSAppService> wp_inst;
  static QMutex mutex;
  QMutexLocker mlocker(&mutex);

  auto sp_inst = wp_inst.toStrongRef();
  if(!sp_inst.isNull())
  {
    sp_inst = 
      QSharedPointer<LCQJSAppService>(new LCQJSAppService, serviceDeleter);
    wp_inst = sp_inst;
  }
  return sp_inst;
}

//------------------------------------------------------------------------------
void LCQJSAppService::readSourceData(const QString& _dataId)
{
  QMutex mutex;
  QWaitCondition cond;
  mutex.lock();
  mMutexEvent.lock();
  QCoreApplication::postEvent(this, new CEventReadData(_dataId, mutex));
  
  
  mWaitCond.wait(&mMutexEvent);
}

//------------------------------------------------------------------------------
void LCQJSAppService::customEvent(QEvent* _event)
{

  //Внимание!!!
  //Необходимо для синхронизации с началом ожидания вызывающего потока.
  mMutexEvent.lock();
  mMutexEvent.unlock();

  if(_event->type() == CEventBase::msExtendedEventType)
  {
    CEventBase* e = dynamic_cast<CEventBase*>(_event);
    if(e == nullptr)
    {
      qDebug() << "LCQJScriptHiden::customEvent dynamic cast err";
      return;
    }
    e->handle(this);
  }
}



