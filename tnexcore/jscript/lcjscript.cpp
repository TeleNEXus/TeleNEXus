#include "lcjscript.h"
#include "lcqjsapplicationinterface.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>

//==============================================================================
static const QString __slApplicationProp = "Application";

//==============================================================================
class CLocalData : public QObject
{
private:
  QJSEngine mJSEngin;
  QJSValue  mJSValue;
  QString jsScript;
  QTimer *timer = nullptr;
  QThread *thr;

public:
  explicit CLocalData() = delete;
  CLocalData(const QString& _script, int _interval) : 
    QObject(nullptr), 
    jsScript(_script)
  {
    mJSValue = mJSEngin.newQObject(new LCQJSApplicationInterface);
    mJSEngin.globalObject().setProperty(__slApplicationProp, mJSValue);
    thr = new QThread;

    timer = new QTimer;
    timer->moveToThread(thr);
    QObject::connect(thr, &QThread::started, [&]{timer->start();});
    QObject::connect(thr, &QThread::finished, [&]{timer->stop();});
    if(_interval <= 0)
    {
      timer->setSingleShot(true);
      _interval = 0;
    }

    timer->setInterval(_interval);

    QObject::connect(timer, &QTimer::timeout,
        [&]()
        {
          mJSEngin.evaluate(jsScript);
        });
    thr->start();
  }

  ~CLocalData()
  {
    if(timer) timer->deleteLater();
  }

  void start()
  {
    /* if(timer->isActive()) return; */
    /* timer->start(); */
    thr->start();
  }

  void stop()
  {
    thr->quit();
    /* timer->stop(); */
  }
};

//==============================================================================
#define mData (*(static_cast<CLocalData*>(mpData)))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval) 
{
  mpData = new CLocalData(_script, _interval);
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
  mData.deleteLater();
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


