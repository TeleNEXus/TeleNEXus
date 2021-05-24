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
#include "cqjsbinaryfile.h"
#include "cqjsprocess.h"
#include "tnex.h"
#include "LIApplication.h"

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QSharedPointer>
#include <QMap>
#include <QJSValueIterator>
#include <QMutexLocker>
#include <QApplication>
#include <QMetaObject>

static QMap<int, QJSEngine*> __slJSEngines;
static int __slJSEngineCounter = 0;

//==============================================================================
static QString createScriptGlobal(QMap<QString, QString> _attrMap, 
    int _engineId);
static void addMetaObjects(QJSEngine& _engine, QJSValue& _globalExport);


//==============================================================================
static const struct
{
  QString applicationGlobalExport = "APPLICATIONGLOBALEXPORT";
  QString attributes =      "Attributes";
  QString callScriptMain =  "Main";
  QString textFile =        "TextFile";
  QString binaryFile =      "BinaryFile";
  QString process =         "Process";
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
  qDebug() << "script execute";
  QEventLoop loop;
  QJSValue call = _sender->mJSEngine.globalObject().property(__slPropNames.callScriptMain);

  auto ret = [_sender]()
  {
    _sender->mpThread->quit();
    qDebug()<< "return CEventExecute::handle ";
  };

  if(call.isNull()) return ret();
  if(!call.isCallable()) return ret();

  QJSValue result = call.call();
  loop.exec();
  if(result.isError()) 
  { 
    _sender->emitError(result);
  }
  return ret();
}

//==============================================================================LCQJScriptHiden
LCQJScriptHiden::LCQJScriptHiden(
    const QString& _script, 
    const QMap<QString, QString>& _attributesMap,
    const QString& _fileName) : 
  QObject(nullptr),
  /* mpThread(new QThread), */
  mpThread(nullptr),
  mTimerId(0)
{

  //Engine registration.
  mId = __slJSEngineCounter;
  __slJSEngineCounter++;
  __slJSEngines.insert(mId, &mJSEngine);

  mspAppService = LCQJSAppService::getService();

  /* moveToThread(mpThread); */
  /* mJSEngine.moveToThread(mpThread); */

  mJSEngine.installExtensions(QJSEngine::Extension::AllExtensions);

  QJSValue jsvalue = mJSEngine.newQObject(this);

  addMetaObjects(mJSEngine, jsvalue);

  mJSEngine.globalObject().setProperty(
      __slPropNames.applicationGlobalExport, jsvalue);

  jsvalue = mJSEngine.evaluate(createScriptGlobal(_attributesMap, mId));

  if(jsvalue.isError()) { emitError(jsvalue);}

  jsvalue = mJSEngine.evaluate(_script, _fileName);

  if(jsvalue.isError())
  {
    emitError(jsvalue);
  }

  /* mJSEngine.collectGarbage(); */

  /* mCallScriptMain = mJSEngine.globalObject().property( */
  /*     __slPropNames.callScriptMain); */

  /* mpThread->start(); */
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
  /* QCoreApplication::postEvent(this, new CEventStart(_interval)); */
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::stop(unsigned long time)
{
  QMutexLocker locker(&mMutex);
  if(!mpThread) return;
  mpThread->quit();
  if(!mpThread->wait(1000))
  {
    /* mpThread->terminate(); */
    /* mpThread->wait(1000); */
    /* disconnect(mQuitConnection); */
    /* moveToThread(QApplication::instance()->thread()); */
    /* mpThread->deleteLater(); */
    /* mpThread = nullptr; */
  }
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::execute()
{
  qDebug() << "     LCQJScriptHiden::execute 0";
  QMutexLocker locker(&mMutex);

  if(mpThread) return;
  qDebug() << "     LCQJScriptHiden::execute 1";
  mpThread = new QThread();
  qDebug() << "     LCQJScriptHiden::execute 2";
  this->moveToThread(mpThread);
  qDebug() << "     LCQJScriptHiden::execute 3";
  mQuitConnection = connect(mpThread, &QThread::finished, 
      [this]()
      {
        qDebug() << "LCQJScriptHiden thread deleter 0";
        QMutexLocker locker(&mMutex);
        moveToThread(QApplication::instance()->thread());
        mpThread->deleteLater();
        mpThread = nullptr;
        qDebug() << "LCQJScriptHiden thread deleter 1";
      });

  qDebug() << "     LCQJScriptHiden::execute 4";
  mpThread->start();
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
  /* QJSValue result = mCallScriptMain.call(); */
  /* if(result.isError()) { */ 
  /*   emitError(result); */
  /* } */
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
void LCQJScriptHiden::importModule(const QString& _fileName, 
    const QString& _propertyName)
{
  QJSValue jsmodule = mJSEngine.importModule(_fileName);

  if(jsmodule.isError())
  {
    mJSEngine.throwError(QString("Can't import from file \"%1\" ").
        arg(_fileName));
    return;
  }

  if(_propertyName != QString())
  {
    QJSValue jsval = jsmodule.property(_propertyName);
    if(jsval.isUndefined())
    {
      mJSEngine.throwError(
          QString("Can't import property \"%1\" from file \"%2\" ").
          arg(_propertyName).
          arg(_fileName));
      return;
    }
    mJSEngine.globalObject().setProperty(_propertyName, jsval);
    return;
  }

  QJSValueIterator it(jsmodule);
  while(it.hasNext())
  {
    it.next();
    mJSEngine.globalObject().setProperty(it.name(), it.value());
  }
}

//------------------------------------------------------------------------------
QJSValue LCQJScriptHiden::newTextFile(const QString& _name)
{
  CQJSTextFile *file = new CQJSTextFile(_name, mId);
  QJSValue val =  mJSEngine.newQObject(file);
  return val;
}

//------------------------------------------------------------------------------
QJSValue LCQJScriptHiden::newBinaryFile(const QString& _name)
{

  CQJSBinaryFile *file = new CQJSBinaryFile(_name, mId);
  QJSValue val =  mJSEngine.newQObject(file);
  return val;
}

//------------------------------------------------------------------------------
void LCQJScriptHiden::collectGarbage()
{
  qDebug() << "Qt: Collect Garbage";
   mJSEngine.collectGarbage();
}

//------------------------------------------------------------------------------
QJSEngine* LCQJScriptHiden::getJSEngine(int _engineId)
{
  auto it = __slJSEngines.find(_engineId);
  if(it == __slJSEngines.end()) return nullptr;
  return it.value();
}

//==============================================================================
static QString createScriptGlobal(QMap<QString, QString> _attrMap, 
    int _engineId)
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
      "function ImportModule(_fileName, _property) {"
      "return %2.importModule(_fileName, _property)};"
      "function NewTextFile(_fileName) {"
      "return new %2.TextFile(_fileName, %3);};"
      "function NewBinaryFile(_fileName) {"
      "return new %2.BinaryFile(_fileName, %3);};"
      "function NewProcess() {"
      "return new %2.Process(%3);};"
      "function CollectGarbage() {"
      "return %2.collectGarbage()};"
      "var ScriptId = \"%4\";"
      "var ScriptFile = \"%5\";"
      )
/*1*/ .arg(obj_attributes)
/*2*/ .arg(__slPropNames.applicationGlobalExport)
/*3*/ .arg(_engineId)
/*4*/ .arg(QString())
/*5*/ .arg(QString());
}


//==============================================================================
static void addMetaObjects(QJSEngine& _engine, QJSValue& _globalExport)
{
  QJSValue jsvalue = _engine.newQMetaObject(&CQJSTextFile::staticMetaObject);
  _globalExport.setProperty(__slPropNames.textFile, jsvalue);

  jsvalue = _engine.newQMetaObject(&CQJSBinaryFile::staticMetaObject);
  _globalExport.setProperty(__slPropNames.binaryFile, jsvalue);

  jsvalue = _engine.newQMetaObject(&CQJSProcess::staticMetaObject);
  _globalExport.setProperty(__slPropNames.process, jsvalue);
}



