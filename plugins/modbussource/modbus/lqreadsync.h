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
#ifndef LCQREADSYNCREQ_H_
#define LCQREADSYNCREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

class QThread;

class LQReadSyncReq  : public QObject
{
  Q_OBJECT;
public:
  using EReadStatus = LIRemoteDataSource::EReadStatus;

private:

  //----------------------------------------------------------------------------CEventRead
  class CEventRead : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventRead();
  };

  QWeakPointer<LIRemoteDataSource>    mwpDataSource;
  QSharedPointer<LIRemoteDataReader>  mspDataReader;

  QString     mSourceId;
  QString     mDataId;
  QByteArray  mRetData;
  EReadStatus mRetStatus;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LQReadSyncReq(QSharedPointer<LIRemoteDataSource> _source, const QString& _dataId);

public:
  virtual ~LQReadSyncReq();

  static QSharedPointer<LQReadSyncReq> create(
      QSharedPointer<LIRemoteDataSource> _source,
      const QString& _dataId,
      QThread* _thread);

  QByteArray readSync(EReadStatus* _status = nullptr);

private:
  virtual void customEvent(QEvent*) override; 
};

#endif
