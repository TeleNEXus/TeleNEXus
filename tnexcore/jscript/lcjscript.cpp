#include "lcjscript.h"
#include "lcqjscripthiden.h"
#include "lcqjsappinterface.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>

//==============================================================================

struct SPrivateData
{
  int mInterval;
  LCQJScriptHiden* mpScriptHiden = nullptr;
  SPrivateData(LCQJScriptHiden* _p_scriptHiden, int _interval):
    mInterval(_interval),
    mpScriptHiden(_p_scriptHiden){}
  ~SPrivateData()
  {
    mpScriptHiden->deleteLater();
  }
};

//==============================================================================
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval)  :
  mpData( new SPrivateData(new LCQJScriptHiden(_script), _interval) )
{
  /* mpData = static_cast<void*>(new SPrivateData()); */
  /* mpPrivateData->mpScriptHiden = new LCQJScriptHiden(_script); */
  /* mpPrivateData->mInterval= _interval; */
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
  /* mpPrivateData->mpScriptHiden->deleteLater(); */
  delete mpPrivateData;
}

//------------------------------------------------------------------------------
void LCJScript::start() 
{
  mpPrivateData->mpScriptHiden->start(mpPrivateData->mInterval);
}

//------------------------------------------------------------------------------
void LCJScript::stop() 
{
  mpPrivateData->mpScriptHiden->stop();
}


