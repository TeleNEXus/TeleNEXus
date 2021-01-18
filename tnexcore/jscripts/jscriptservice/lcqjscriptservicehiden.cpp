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

//==============================================================================
static const struct
{
  QString applicationGlobalExport = "APPLICATIONGLOBALEXPORT";
  QString attributes = "Attributes";
  QString callScriptMain = "Main";
}__slPropNames;

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
  _sender->scriptExecute();
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

//==============================================================================CEventExecute
LCQJScriptHiden::CEventExecute::CEventExecute()
{
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::CEventExecute::handle(LCQJScriptHiden* _sender)
{
  _sender->scriptExecute();
}

//==============================================================================LCQJScriptHiden
static QString createScriptGlobal(QMap<QString, QString> _attrMap);
static void emitError(const QJSValue& _value);

LCQJScriptHiden::LCQJScriptHiden(
    const QString& _script, 
    const QMap<QString, QString>& _attributesMap) : 
  QObject(nullptr),
  mpThread(new QThread),
  mTimerId(0)
{
  moveToThread(mpThread);

  QJSValue jsvalue = mJSEngine.newQObject(new LCQJSAppInterface(mJSEngine));

  mJSEngine.globalObject().setProperty(
      __slPropNames.applicationGlobalExport, jsvalue);

  jsvalue = mJSEngine.evaluate( createScriptGlobal(_attributesMap) );

  if(jsvalue.isError()) { emitError(jsvalue);}

  mJSEngine.evaluate(_script);

  mCallScriptMain = mJSEngine.globalObject().property(
      __slPropNames.callScriptMain);

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
  scriptExecute();
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::launch(int _interval)
{
  QCoreApplication::postEvent(this, new CEventStart(_interval));
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::stop()
{
  QCoreApplication::postEvent(this, new CEventStop());
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::execute()
{
  QCoreApplication::postEvent(this, new CEventExecute());
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
void LCQJScriptHiden::scriptExecute()
{
  QJSValue result = mCallScriptMain.call();
  if(result.isError()) { emitError(result);}
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

//==============================================================================createScriptGlobal
static QString createScriptGlobal(QMap<QString, QString> _attrMap)
{
  QString obj_attributes = 
    QString("var %1 = { ").arg(__slPropNames.attributes);

  for(auto it = _attrMap.begin(); it != _attrMap.end(); it++)
  {
    obj_attributes += QString("%1 : '%2', ")
      .arg(it.key())
      .arg(it.value());
  }

  obj_attributes += "};";

  return QString(
      "%1"
      "function DebugOut(str) {%2.debugOut(str)};"
      "function DataSourceRead(_sourceId, _dataId) {"
      "return %2.readData(_sourceId, _dataId)};"
      "function DataSourceWrite(_sourceId, _dataId, _data) {"
      "return %2.writeData(_sourceId, _dataId, _data)};"
      "var ScriptId = \"%3\";"
      "var ScriptFile = \"%4\";"
      )
    .arg(obj_attributes)
    .arg(__slPropNames.applicationGlobalExport)
    .arg(QString())
    .arg(QString());
}

//==============================================================================emitError
static void emitError(const QJSValue& _value)
{
  qDebug() 
    << "Uncaught exception at line"
    << _value.property("lineNumber").toInt()
    << ":" << _value.toString();
}
