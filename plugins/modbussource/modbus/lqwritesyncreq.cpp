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
#include "lqwritesyncreq.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CEventWrite
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQWriteSyncReq::CEventWrite);

LQWriteSyncReq::CEventWrite::CEventWrite() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================requestDeleter
static void requestDeleter(LQWriteSyncReq* _req)
{
  _req->deleteLater();
}

//==============================================================================LQWriteSyncReq
LQWriteSyncReq::LQWriteSyncReq(
    QSharedPointer<LIRemoteDataSource> _source,
    const QString&      _dataId) :
  QObject(nullptr),
  mwpDataSource(_source),
  mDataId(_dataId),
  mpWriteData(nullptr),
  mWriteStatus(EWriteStatus::Failure)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LQWriteSyncReq> LQWriteSyncReq::create(
    QSharedPointer<LIRemoteDataSource> _source,
    const QString&      _dataId,
    QThread* _thread)
{
  LQWriteSyncReq* req = new LQWriteSyncReq(_source, _dataId);
  req->moveToThread(_thread);
  return QSharedPointer<LQWriteSyncReq>(req, requestDeleter);
}


//------------------------------------------------------------------------------
LQWriteSyncReq::~LQWriteSyncReq()
{
}

//------------------------------------------------------------------------------
LQWriteSyncReq::EWriteStatus LQWriteSyncReq::writeSync(const QByteArray& _data)
{
  mMutexEvent.lock();

  mpWriteData = &_data;

  QCoreApplication::postEvent(this, new CEventWrite());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  return mWriteStatus;
}

//------------------------------------------------------------------------------
void LQWriteSyncReq::customEvent(QEvent* _event)
{
  mMutexEvent.lock();
  mMutexEvent.unlock();


  auto write_handler = 
    [this](EWriteStatus _status)
    {
      mWriteStatus = _status;
      mWaitCond.wakeOne();
    };

  auto handle_normal = 
    [this, &write_handler]()
    {
      auto source = mwpDataSource.lock();

      if(source.isNull()) {return write_handler(EWriteStatus::Failure);}

      mspDataWriter = source->createWriter(mDataId);

      if(mspDataWriter.isNull()) { return write_handler(EWriteStatus::Failure); }

      mspDataWriter->setHandler(write_handler);
      mspDataWriter->writeRequest(*(mpWriteData));
    };

  if(_event->type() == CEventWrite::msExtendedEventType)
  {
    CEventWrite* e = dynamic_cast<CEventWrite*>(_event);
    if(e == nullptr) { return write_handler(EWriteStatus::Failure);}
    handle_normal();
  }
}

