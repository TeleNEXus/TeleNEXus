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
#ifndef LCQJSCRIPTHIDEN_H_
#define LCQJSCRIPTHIDEN_H_

#include "lqextendevent.h"
#include <QObject>
#include <QJSEngine>
#include <QDebug>
/* #include <QMutex> */
#include <QEventLoop>

class QTimer;
class LCQJSAppService;
class LCQJScriptHiden : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQJScriptHiden* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventLaunch
  class CEventLaunch: public CEventBase
  {
  private:
    int mInterval;
  public:
    CEventLaunch() = delete;
    CEventLaunch(int _interval);
    virtual void handle(LCQJScriptHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventStop
  class CEventStop: public CEventBase
  {
  public:
    CEventStop();
    virtual void handle(LCQJScriptHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventExecute
  class CEventExecute: public CEventBase
  {
  public:
    CEventExecute();
    virtual void handle(LCQJScriptHiden* _sender) override;

  };

  QString     mScriptFileName;
  int         mId;
  QSharedPointer<LCQJSAppService> mspAppService;
  QJSEngine   mJSEngine;
  QThread*    mpThread;
  int         mTimerId;
  QEventLoop  mEventLoop;
  QJSValue    mCallScriptMain;
public:
  static int mObjectCounter;

  explicit LCQJScriptHiden(
      const QString& _script, 
      const QMap<QString, QString>& _attributesMap,
      const QString& _fileName);

  virtual ~LCQJScriptHiden();
  void launch(int interval);
  void stop();
  void execute();
  QMetaObject::Connection mQuitConnection;

private:
  virtual void customEvent(QEvent* _event) override;
  virtual void timerEvent(QTimerEvent*) override;
  void timerStart(int _interval);
  void timerStop();
  void scriptExecute();

//--------------------------------------------------------jsexports
public slots:

  void message(const QString& _out);
  void warning(const QString& _out);
  void error(const QString& _out);

  QString getProjectPath();

  QVariantList readData(
      const QString& _sourceId, 
      const QString& _dataId);

  int writeData(
      const QString& _sourceId, 
      const QString& _dataId, 
      const QVariantList& _data);

  void importModule(const QString& _fileName, 
      const QString& _propertyName = QString());

  QJSValue newTextFile(const QString& _name);
  QJSValue newBinaryFile(const QString& _name);

  void sleepMSec(unsigned long _timems);
    
  void collectGarbage();

private:
  static QJSEngine* getJSEngine(int _engineId);

  friend class CQJSTextFile;
  friend class CQJSBinaryFile;
  friend class CQJSProcess;
};
#endif
