#include "lcjscript.h"
#include "lcqjsapplicationinterface.h"
#include <QJSEngine>
#include <QTimer>

//==============================================================================
static const QString __slApplicationProp = "Application";

//==============================================================================
struct SLocalData
{
    QJSValue  js_value_app;
  QJSEngine jsEngin;
  QString jsScript;
  QTimer *timer = nullptr;
  int timeIntervalMs = 0;

  explicit SLocalData() = delete;

  SLocalData(const QString& _script, int _interval) : jsScript(_script)
  {
    /* QJSValue  js_value_app; */

    js_value_app = jsEngin.newQObject(&LCQJSApplicationInterface::instance());
    jsEngin.globalObject().setProperty(__slApplicationProp, js_value_app);

    /* timer = new QTimer; */
    /* if(_interval <= 0) */
    /* { */
    /*   timer->setSingleShot(true); */
    /*   _interval = 0; */
    /* } */

    /* timer->setInterval(_interval); */

    /* QObject::connect(timer, &QTimer::timeout, */
    /*     [&]() */
    /*     { */
    /*       this->evaluate(); */
    /*     }); */
  }

  ~SLocalData()
  {
    /* delete timer; */
    /* if(timer) timer->deleteLater(); */
  }

  QJSValue evaluate()
  {
    /* return jsEngin.evaluate(jsScript); */
  }

  void start()
  {
    /* if(timer->isActive()) return; */
    /* timer->start(); */
  }

  void stop()
  {
    /* timer->stop(); */
  }
};

//==============================================================================
#define mData (*(static_cast<SLocalData*>(mpData)))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval) 
{
  mpData = new SLocalData(_script, _interval);
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
  delete static_cast<SLocalData*>(mpData);
}

//------------------------------------------------------------------------------
void LCJScript::start() 
{
  mData.start();
}

//------------------------------------------------------------------------------
void LCJScript::stop() 
{
  mData.stop();
}


