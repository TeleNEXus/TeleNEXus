#include "lcqjscripthiden.h"
#include "lcqjsapplicationinterface.h"
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QJSEngine>

static const QString __slApplicationProp = "Application";
//==============================================================================SLocalData
struct SLocalData
{
        QJSEngine   jsengin;
        QJSValue    jsvalue;
  QString     mScriptString;
  QThread*    mpThread;
  QTimer*     mpTimer;
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
  reinterpret_cast<SLocalData*>(_sender->mpData)->mpTimer->stop();
}

int LCQJScriptHiden::mObjectCounter = 0;

#define mpLocalData ((reinterpret_cast<SLocalData*>(mpData)))
//==============================================================================LCQJScriptHiden
LCQJScriptHiden::LCQJScriptHiden(const QString& _script, QObject* _parent) : 
  QObject(_parent)
{

  mpData = new SLocalData;
  mpLocalData->mScriptString = _script;
  mpLocalData->mpThread = new QThread;
  mpLocalData->mpTimer = new QTimer;

  moveToThread(mpLocalData->mpThread);
  mpLocalData->mpTimer->moveToThread(mpLocalData->mpThread);

  int nobj = mObjectCounter;

        mpLocalData->jsvalue = mpLocalData->jsengin.newQObject(new LCQJSApplicationInterface);
        mpLocalData->jsengin.globalObject().setProperty(__slApplicationProp, mpLocalData->jsvalue);
  
  connect(mpLocalData->mpTimer, &QTimer::timeout, 
      [this, nobj]
      {
        qDebug() << "************************************Script timer timeout " << nobj;

        mpLocalData->jsengin.evaluate(mpLocalData->mScriptString);

        /* for(int i = 0; i < 100; i++) */
        /* { */
        /*   qDebug() << "Script timer timeout " << mpLocalData->mpTimer->interval() << " i = " << i; */
        /* } */
      });

  
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
  mpLocalData->mpThread->deleteLater();
  mpLocalData->mpTimer->deleteLater();
  delete mpLocalData;
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
        static_cast<CEventBase*>(_event)->handle(this);
    }
}
