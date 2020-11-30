#include "lcjscript.h"
#include "lcqjsapplicationinterface.h"
#include <QJSEngine>
#include <QTimer>

//==============================================================================
struct SLocalData
{
  QJSEngine jsEngin;
  QString jsScript;
  QTimer *timer = nullptr;
  int timeIntervalMs = 0;

  explicit SLocalData() = delete;

  SLocalData(const QString& _script, int _interval) : jsScript(_script)
  {
    QJSValue  js_value_app;
    js_value_app = jsEngin.newQObject(&LCQJSApplicationInterface::instance());
    jsEngin.globalObject().setProperty("Application", js_value_app);

    timer = new QTimer;
    if(_interval <=0)
    {
      timer->setSingleShot(true);
    }
    else
    {
      timer->setInterval(_interval);
    }

    QObject::connect(timer, &QTimer::timeout,
        [this]()
        {
          evaluate();
        });
  }

  ~SLocalData()
  {
    if(timer) timer->deleteLater();
  }

  QJSValue evaluate()
  {
    return jsEngin.evaluate(jsScript);
  }

  void start()
  {
    if(timer->isActive()) return;
    timer->start();
  }

  void stop()
  {
    timer->stop();
  }

};

//==============================================================================
#define mData (*static_cast<SLocalData*>(mpData))

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




