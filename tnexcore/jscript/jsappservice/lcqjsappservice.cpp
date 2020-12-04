
#include "lcqjsappservice.h"
#include "lcqreadfromsourcereq.h"
#include "LIApplication.h"
#include "lcxmlmain.h"

#include <QMutex>
#include <QThread>
#include <QDebug>

/* //==============================================================================CEventBase */
/* __LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQJSAppService::CEventBase); */

/* LCQJSAppService::CEventBase::CEventBase() : */ 
/*   QEvent(__LQ_EXTENDED_QEVENT_REGISTERED) */
/* { */
/* } */

/* //==============================================================================CEventReadData */
/* LCQJSAppService::CEventReadData::CEventReadData( */
/*     const QString& _dataId, */
/*     QMutex& _mutex) : */
/*   edDataId(_dataId), */
/*   edMutex(_mutex) */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* void LCQJSAppService::CEventReadData::handle(LCQJSAppService* _sender) */
/* { */
/*   Q_UNUSED(_sender); */
/* } */

//==============================================================================LCQJSAppService
LCQJSAppService::LCQJSAppService() : 
  mpThread(new QThread)
{
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
QSharedPointer<LCQJSAppService> LCQJSAppService::getService()
{
  static QWeakPointer<LCQJSAppService> wp_inst;
  static QMutex mutex;
  QMutexLocker locker(&mutex);

  auto sp_inst = wp_inst.toStrongRef();
  if(sp_inst.isNull())
  {
    sp_inst = 
      QSharedPointer<LCQJSAppService>(new LCQJSAppService);
    wp_inst = sp_inst;
  }
  return sp_inst;
}

//------------------------------------------------------------------------------
QByteArray LCQJSAppService::readSourceData(
    const QString& _sourceId, const QString& _dataId)
{
  auto req = LCQReadFromSourceReq::create(_sourceId, _dataId, mpThread);
  return req->getData();
}

//------------------------------------------------------------------------------
QString LCQJSAppService::getProjectPath()
{
  return LCXmlMain::getApplicationInterface().getProjectPath();
}

/* //------------------------------------------------------------------------------ */
/* void LCQJSAppService::customEvent(QEvent* _event) */
/* { */

/*   //Внимание!!! */
/*   //Необходимо для синхронизации с началом ожидания вызывающего потока. */
/*   mMutexEvent.lock(); */
/*   mMutexEvent.unlock(); */

/*   if(_event->type() == CEventBase::msExtendedEventType) */
/*   { */
/*     CEventBase* e = dynamic_cast<CEventBase*>(_event); */
/*     if(e == nullptr) */
/*     { */
/*       qDebug() << "LCQJScriptHiden::customEvent dynamic cast err"; */
/*       return; */
/*     } */
/*     e->handle(this); */
/*   } */
/* } */



