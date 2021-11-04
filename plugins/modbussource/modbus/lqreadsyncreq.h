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
#ifndef LCQREADFROMSOURCEREQ_H_
#define LCQREADFROMSOURCEREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

class QThread;

class LCQReadFromSourceReq  : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQReadFromSourceReq* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventRead: public CEventBase 
  {
  public:
      CEventRead();
      virtual void handle(LCQReadFromSourceReq* _sender);
  };

  QString     mSourceId;
  QString     mDataId;
  QByteArray  mRetData;

  QSharedPointer<LIRemoteDataReader>  mspDataReader;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LCQReadFromSourceReq(const QString& _sourceId, const QString& _dataId);

public:
  virtual ~LCQReadFromSourceReq();

  static QSharedPointer<LCQReadFromSourceReq> create(
      const QString& _sourceId,
      const QString& _dataId,
      QThread* _thread);

  QByteArray readData();
private:
  virtual void customEvent(QEvent*) override; 
};
#endif
