#include "lcjscript.h"
#include "lcqjscripthiden.h"
#include "lcqjsapplicationinterface.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>

//==============================================================================
static const QString __slApplicationProp = "Application";

//==============================================================================
/* class CLocalData : public QObject */
/* { */
/* private: */

/*   QJSEngine   mJSEngin; */
/*   QJSValue    mJSValue; */
/*   QString     jsScript; */
/*   QTimer      *mpTimer = nullptr; */
/*   QThread     *mpThread = nullptr; */

/* public: */
/*   explicit CLocalData() = delete; */
/*   CLocalData(const QString& _script, int _interval) : */ 
/*     QObject(nullptr), */ 
/*     jsScript(_script) */
/*   { */
/*     mJSValue = mJSEngin.newQObject(new LCQJSApplicationInterface); */
/*     mJSEngin.globalObject().setProperty(__slApplicationProp, mJSValue); */
/*     mpThread = new QThread; */

/*     mpTimer = new QTimer; */
/*     mpTimer->moveToThread(mpThread); */
/*     QObject::connect(mpThread, &QThread::started, [&]{mpTimer->start();}); */
/*     QObject::connect(mpThread, &QThread::finished, [&]{mpTimer->stop();}); */
/*     if(_interval <= 0) */
/*     { */
/*       mpTimer->setSingleShot(true); */
/*       _interval = 0; */
/*     } */

/*     mpTimer->setInterval(_interval); */

/*     QObject::connect(mpTimer, &QTimer::timeout, */
/*         [&]() */
/*         { */
/*           mJSEngin.evaluate(jsScript); */
/*         }); */
/*   } */

/*   ~CLocalData() */
/*   { */
/*     if(mpTimer) { mpTimer->deleteLater(); } */
/*     if(mpThread) { mpThread->deleteLater(); } */
/*   } */

/*   void start() */
/*   { */
/*     mpThread->start(); */
/*   } */

/*   void stop() */
/*   { */
/*     mpThread->quit(); */
/*   } */
/* }; */

struct SLocalData
{
  int mInterval = 0;
  LCQJScriptHiden* mpScriptHiden = nullptr;
};
//==============================================================================
#define mpLocalData (static_cast<SLocalData*>(mpData))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval) 
{
  mpData = new SLocalData();
  mpLocalData->mpScriptHiden = new LCQJScriptHiden(_script);
  mpLocalData->mInterval= _interval;
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
  mpLocalData->mpScriptHiden->deleteLater();
  delete mpLocalData;
}

//------------------------------------------------------------------------------
void LCJScript::start() 
{
  mpLocalData->mpScriptHiden->start(mpLocalData->mInterval);
}

//------------------------------------------------------------------------------
void LCJScript::stop() 
{
  mpLocalData->mpScriptHiden->stop();
}


