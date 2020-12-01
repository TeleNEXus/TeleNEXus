#include "lcqjscripthiden.h"
#include "lcqjsapplicationinterface.h"
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QJSEngine>
#include <QSharedPointer>

static const QString __slApplicationProp = "Application";
//==============================================================================SLocalData
class SLocalData : public QObject
{
public:
        QJSEngine   jsengin;
        QJSValue    jsvalue;
  QString     mScriptString;
  QSharedPointer<QThread>  mpThread;
  QSharedPointer<QTimer>   mpTimer;
  SLocalData(): QObject(nullptr){}
};

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQJScriptHiden::CEventBase);

LCQJScriptHiden::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventStart
LCQJScriptHiden::CEventStart::CEventStart(int _interval) : mInterval(_interval)
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventStart::handle(LCQJScriptHiden* _sender)
{
  /* Q_UNUSED(_sender); */
  qDebug() << "LCQJScriptHiden event start handler";
  QTimer& timer = *(static_cast<SLocalData*>(_sender->mpData)->mpTimer);

  if(mInterval <= 0)
  {
    timer.setSingleShot(true);
    timer.start(0);
    return;
  }

  timer.setSingleShot(false);
  timer.start(mInterval);
}

//==============================================================================CEventStop
LCQJScriptHiden::CEventStop::CEventStop()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventStop::handle(LCQJScriptHiden* _sender)
{
  Q_UNUSED(_sender);
  qDebug() << "LCQJScriptHiden event stop handler";
  static_cast<SLocalData*>(_sender->mpData)->mpTimer->stop();
}

int LCQJScriptHiden::mObjectCounter = 0;

#define mpLocalData ((static_cast<SLocalData*>(mpData)))

static void doDeleteThread(QThread*obj)
  {
    obj->exit(0);
    obj->terminate();
    obj->wait();
      obj->deleteLater();
  }

static void doDeleteTimer(QTimer *obj)
  {
    obj->stop();
      obj->deleteLater();
  }



//==============================================================================LCQJScriptHiden
LCQJScriptHiden::LCQJScriptHiden(const QString& _script, QObject* _parent) : 
  QObject(_parent),
  mpData(nullptr)
{

  mpData = static_cast<void*>(new SLocalData);
  mpLocalData->mScriptString = _script;
  mpLocalData->mpTimer = QSharedPointer<QTimer>(new QTimer, doDeleteTimer);
  mpLocalData->mpThread = QSharedPointer<QThread>(new QThread, doDeleteThread);

  moveToThread(mpLocalData->mpThread.data());
  mpLocalData->mpTimer->moveToThread(mpLocalData->mpThread.data());
  mpLocalData->jsengin.moveToThread(mpLocalData->mpThread.data());
  /* mpLocalData->jsvalue.moveToThread(mpLocalData->mpThread); */

  int nobj = mObjectCounter;

        mpLocalData->jsvalue = mpLocalData->jsengin.newQObject(new LCQJSApplicationInterface);
        mpLocalData->jsengin.globalObject().setProperty(__slApplicationProp, mpLocalData->jsvalue);
  
  connect(mpLocalData->mpTimer.data(), &QTimer::timeout, 
      [this, nobj]
      {
      for(int i = 0; i < 10000; i++)
        qDebug() << "Script timer timeout " << nobj<< " i = " << i;
        

        /* mpLocalData->jsengin.evaluate(mpLocalData->mScriptString); */

        /* for(int i = 0; i < 100; i++) */
        /* { */
        /*   qDebug() << "Script timer timeout " << mpLocalData->mpTimer->interval() << " i = " << i; */
        /* } */
      });
  connect(mpLocalData->mpThread.data(), &QThread::finished, [this]{mpLocalData->mpTimer->stop();});

  
  /* connect(mpLocalData->mpTimer, &QTimer::timeout, */ 
  /*     [this, nobj] */
  /*     { */

  /*       qDebug() << "************************************Script timer timeout " << nobj; */

  /*       QJSEngine   jsengin; */
  /*       QJSValue    jsvalue; */

  /*       jsvalue = jsengin.newQObject(new LCQJSApplicationInterface); */
  /*       jsengin.globalObject().setProperty(__slApplicationProp, jsvalue); */
  /*       jsengin.evaluate(mpLocalData->mScriptString); */

  /*       /1* for(int i = 0; i < 100; i++) *1/ */
  /*       /1* { *1/ */
  /*       /1*   qDebug() << "Script timer timeout " << mpLocalData->mpTimer->interval() << " i = " << i; *1/ */
  /*       /1* } *1/ */
  /*     }); */


  mpLocalData->mpThread->start();
  mObjectCounter++;
}

//------------------------------------------------------------------------------
LCQJScriptHiden::~LCQJScriptHiden()
{
  if(mpLocalData)
  {
    /* mpLocalData->mpThread->terminate(); */
    /* mpLocalData->mpThread->quit(); */
    /* mpLocalData->mpThread->wait(); */

    /* mpLocalData->mpThread->deleteLater(); */
    /* mpLocalData->mpTimer->deleteLater(); */
    mpLocalData->deleteLater();
  }
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::start(int _interval)
{
    QCoreApplication::postEvent(this, new CEventStart(_interval));
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::stop()
{
    QCoreApplication::postEvent(this, new CEventStop());
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::customEvent(QEvent* _event)
{
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
