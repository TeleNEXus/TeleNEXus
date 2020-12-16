/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lcqjscriptservicehiden.h"
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
  _sender->scriptEvaluate();
  if(mInterval <= 0) { return; }
  _sender->timerStart(mInterval);
}

//==============================================================================CEventStop
LCQJScriptHiden::CEventStop::CEventStop()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventStop::handle(LCQJScriptHiden* _sender)
{
  _sender->timerStop();
}

//==============================================================================CEventEvaluate
LCQJScriptHiden::CEventEvaluate::CEventEvaluate()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventEvaluate::handle(LCQJScriptHiden* _sender)
{
  _sender->scriptEvaluate();
}

//==============================================================================LCQJScriptHiden
LCQJScriptHiden::LCQJScriptHiden(const QString& _script, QObject* _parent) : 
  QObject(_parent),
  mScriptString(_script),
  mpThread(new QThread),
  mTimerId(0)
{
  moveToThread(mpThread);

  mJSValue = mJSEngin.newQObject(new LCQJSAppInterface);
  mJSEngin.globalObject().setProperty(__slApplicationProp, mJSValue);

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
  QJSValue result = mJSEngin.evaluate(mScriptString);
  if (result.isError())
    qDebug()
      << "Script uncaught exception at line"
      << result.property("lineNumber").toInt()
      << ":" << result.toString();
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::customEvent(QEvent* _event)
{
  if(_event->type() == CEventBase::msExtendedEventType)
  {
    CEventBase* e = dynamic_cast<CEventBase*>(_event);
    if(e == nullptr)
    {
      return;
    }
    e->handle(this);
  }
}
