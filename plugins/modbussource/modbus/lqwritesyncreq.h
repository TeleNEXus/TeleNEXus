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
#ifndef LCQWRITETOSOURCE_H_
#define LCQWRITETOSOURCE_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class QThread;

class LCQWriteToSource  : public QObject
{
  Q_OBJECT;
private:

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQWriteToSource* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventWrite: public CEventBase 
  {
  public:
    CEventWrite();
    virtual void handle(LCQWriteToSource* _sender);
  };

  QString     mSourceId;
  QString     mDataId;

  const QByteArray& edWriteData;

  int mWriteDataSize;

  QSharedPointer<LIRemoteDataWriter>    mspDataWriter;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LCQWriteToSource(
      const QString&      _sourceId, 
      const QString&      _dataId,
      const QByteArray&   _writeData
      );

public:
  virtual ~LCQWriteToSource();

  static QSharedPointer<LCQWriteToSource> create(
      const QString&      _sourceId,
      const QString&      _dataId,
      const QByteArray&   _writeData,
      QThread* _thread);

  int writeData();

private:
  virtual void customEvent(QEvent*) override; 
};
#endif
