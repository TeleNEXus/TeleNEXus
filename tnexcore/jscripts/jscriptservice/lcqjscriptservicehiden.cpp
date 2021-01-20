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
#include "lcqjsappservice.h"
#include "cqjstextfile.h"
#include "tnex.h"
#include "LIApplication.h"

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

static const struct
{
  QString textFile = "TextFile";
}__slObjectsNames;

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

LCQJScriptHiden::LCQJScriptHiden(
    const QString& _script, 
    const QMap<QString, QString>& _attributesMap,
    const QString& _fileName) : 
  QObject(nullptr),
  mpThread(new QThread),
  mTimerId(0)
{
  mspAppService = LCQJSAppService::getService();

  moveToThread(mpThread);

  mJSEngine.installExtensions(QJSEngine::Extension::AllExtensions);

  QJSValue jsvalue = mJSEngine.newQMetaObject(&CQJSTextFile::staticMetaObject);
  mJSEngine.globalObject().setProperty(__slObjectsNames.textFile, jsvalue);


  jsvalue = mJSEngine.newQObject(this);

  mJSEngine.globalObject().setProperty(
      __slPropNames.applicationGlobalExport, jsvalue);

  jsvalue = mJSEngine.evaluate( createScriptGlobal(_attributesMap) );

  if(jsvalue.isError()) { emitError(jsvalue);}

  mJSEngine.evaluate(_script, _fileName);

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
  if(result.isError()) { 
    emitError(result);
    /* timerStop(); */
  }
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

//------------------------------------------------------------------------------
void LCQJScriptHiden::emitError(const QJSValue& _value)
{
  debugOut(QString("Uncaught exception in file %1 at line %2: %3")
      .arg(_value.property("fileName").toString())
      .arg(_value.property(QStringLiteral("lineNumber")).toInt())
      .arg(_value.toString()));
}



//------------------------------------------------------------------------------
void LCQJScriptHiden::debugOut(const QString& _out)
{
  qDebug("%s", qPrintable(_out));
}

//------------------------------------------------------------------------------
QString LCQJScriptHiden::getProjectPath()
{
  return tnex::getApplicationInterface().getProjectPath();
}

//------------------------------------------------------------------------------
QVariantList LCQJScriptHiden::readData(
    const QString& _sourceId, const QString& _dataId)
{
  QByteArray data_array = 
    mspAppService->readData(_sourceId, _dataId); 
  QVariantList ret;
  for(int i = 0; i < data_array.size(); i++)
  {
    ret << QVariant(static_cast<unsigned char>(data_array.at(i)));
  }
  return ret; 
}

//------------------------------------------------------------------------------
int LCQJScriptHiden::writeData(
    const QString& _sourceId, 
    const QString& _dataId, 
    const QVariantList& _data)
{

  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    bool flag = false;
    int d = _data.at(i).toUInt(&flag);
    if(!flag) 
    {
      return 0;
    }
    wd[i] =  (unsigned char)d;
  }

  return mspAppService->writeData(_sourceId, _dataId, wd);
}

//------------------------------------------------------------------------------
bool LCQJScriptHiden::exportModule(const QString& _fileName)
{
  QString full_file_name = 
        tnex::getApplicationInterface().getProjectPath() + _fileName;

  QFile script_file(full_file_name);

  if(!script_file.open(QFile::OpenModeFlag::ReadOnly)) { 
    return false;}

  QTextStream stream(&script_file);
  QString script_str = stream.readAll();
  script_file.close();
  if(script_str.isNull()) { return false; }

  QJSValue jsvalue = mJSEngine.evaluate(script_str, _fileName);

  if(jsvalue.isError()) 
  {
    emitError(jsvalue);
    return false;
  }

  return false;
}

void LCQJScriptHiden::exjs(QJSValue _jsvalue)
{

  if(_jsvalue.isString())
  {
    qDebug() << "JS Value is String = " << _jsvalue.toString();
  }
  if(_jsvalue.isCallable())
  {
    qDebug() << "JS Value is callable = " << _jsvalue.toString();
    _jsvalue.call(QJSValueList() << "String argument 1" << "String argument 2");
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
      "function ExportModule(_fileName) {"
      "return %2.exportModule(_fileName)};"
      "function ExJs(_jsv) {"
      "return %2.exjs(_jsv)};"
      "var ScriptId = \"%3\";"
      "var ScriptFile = \"%4\";"
      )
    .arg(obj_attributes)
    .arg(__slPropNames.applicationGlobalExport)
    .arg(QString())
    .arg(QString());
}
