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
  LCQJScriptHiden* mpScriptHiden;

  SPrivateData(const QString& _script, int _interval):
    mInterval(_interval),
    mpScriptHiden(new LCQJScriptHiden(_script)){}
  ~SPrivateData()
  {
    mpScriptHiden->deleteLater();
  }
};

//==============================================================================
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval)  :
  mpData( new SPrivateData(_script, _interval) )
{
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
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


