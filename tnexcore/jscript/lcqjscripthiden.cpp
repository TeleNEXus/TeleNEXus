#include "lcqjscripthiden.h"
#include "lcqjsappinterface.h"
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QSharedPointer>

static const QString __slApplicationProp = "Application";

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
  qDebug() << "LCQJScriptHiden event start handler";

  if(mInterval <= 0)
  {
    _sender->scriptEvaluate();
    return;
  }

  _sender->timerStart(mInterval);
}

//==============================================================================CEventStop
LCQJScriptHiden::CEventStop::CEventStop()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventStop::handle(LCQJScriptHiden* _sender)
{
  qDebug() << "LCQJScriptHiden event stop handler";
  _sender->timerStop();
}

//==============================================================================CEventEvaluate
LCQJScriptHiden::CEventEvaluate::CEventEvaluate()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventEvaluate::handle(LCQJScriptHiden* _sender)
{
  qDebug() << "LCQJScriptHiden event evaluate handler";
  _sender->scriptEvaluate();
}

int LCQJScriptHiden::mObjectCounter = 0;


//==============================================================================LCQJScriptHiden
LCQJScriptHiden::LCQJScriptHiden(const QString& _script, QObject* _parent) : 
  QObject(_parent),
  mScriptString(_script),
  mpThread(new QThread),
  mObjectNumber(0),
  mTimerId(0)
{
  moveToThread(mpThread);

  /* QJSEngine mJSEngin; */
  /* QJSValue mJSValue = mJSEngin.newQObject(new LCQJSAppInterface); */

  mJSValue = mJSEngin.newQObject(new LCQJSAppInterface);
  mJSEngin.globalObject().setProperty(__slApplicationProp, mJSValue);

  mObjectNumber = mObjectCounter;
  mObjectCounter++;
  mpThread->start();
}

//------------------------------------------------------------------------------
LCQJScriptHiden::~LCQJScriptHiden()
{
    mpThread->quit();
    mpThread->wait();
    mpThread->deleteLater();
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::timerEvent(QTimerEvent* _event)
{
  Q_UNUSED(_event);
  qDebug() << "Script timer timeout object" << mObjectNumber;
  scriptEvaluate();
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
void LCQJScriptHiden::evaluate()
{
    QCoreApplication::postEvent(this, new CEventEvaluate());
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::timerStart(int _interval)
{
  if(mTimerId == 0)
  {
    mTimerId = startTimer(_interval);
  }
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::timerStop()
{
  if(mTimerId != 0)
  {
    killTimer(mTimerId);
    mTimerId = 0;
  }
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::scriptEvaluate()
{
  /* QJSEngine jsengine; */
  /* QJSValue jsvalue = jsengine.newQObject(new LCQJSAppInterface); */
  /* jsengine.globalObject().setProperty(__slApplicationProp, jsvalue); */
  /* jsengine.evaluate(mScriptString); */
  mJSEngin.evaluate(mScriptString);
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
