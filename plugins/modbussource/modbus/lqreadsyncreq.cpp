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
#include "lqreadsyncreq.h"

#include <QCoreApplication>
#include <QDebug>

using EReadStatus = LIRemoteDataReader::EReadStatus;

//==============================================================================
static void requestDeleter(LQReadSyncReq* _req)
{
  _req->deleteLater();
}

//==============================================================================CEventRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQReadSyncReq::CEventRead);

LQReadSyncReq::CEventRead::CEventRead() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================LQReadSyncReq
LQReadSyncReq::LQReadSyncReq(
    QSharedPointer<LIRemoteDataSource> _source,
    const QString& _dataId) :
  QObject(nullptr),
  mwpDataSource(_source),
  mDataId(_dataId),
  mRetStatus(EReadStatus::Undef)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LQReadSyncReq> LQReadSyncReq::create(
    QSharedPointer<LIRemoteDataSource> _source,
    const QString& _dataId,
    QThread* _thread)
{
  LQReadSyncReq* req = new LQReadSyncReq(_source, _dataId);
  req->moveToThread(_thread);
  return QSharedPointer<LQReadSyncReq>(req, requestDeleter);
}

//------------------------------------------------------------------------------
LQReadSyncReq::~LQReadSyncReq()
{
}

//------------------------------------------------------------------------------
QByteArray LQReadSyncReq::readSync(EReadStatus* _status)
{
  mMutexEvent.lock();

  QCoreApplication::postEvent(this, new CEventRead());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  if(_status != nullptr) *_status = mRetStatus;

  return mRetData;
}

//------------------------------------------------------------------------------
void LQReadSyncReq::customEvent(QEvent* _event)
{
  mMutexEvent.lock();
  mMutexEvent.unlock();

  auto read_handler = 
    [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
    {
      if(_status == EReadStatus::Valid) 
      {
        if(!_data.isNull()) mRetData = *_data.data();
      }
      mRetStatus = _status;
      mWaitCond.wakeOne();
    };

  auto handler_normal = 
    [this, &read_handler]()
    {
      auto source = mwpDataSource.lock();

      if(source.isNull()) 
      {
        mWaitCond.wakeOne();
        return;
      }

      mspDataReader = source->createReader(mDataId);

      if(mspDataReader.isNull()){ return read_handler(nullptr, EReadStatus::Undef); }

      mspDataReader->setHandler(read_handler);
      mspDataReader->readRequest();

    };

  if(_event->type() == CEventRead::msExtendedEventType)
  {
    CEventRead* e = dynamic_cast<CEventRead*>(_event);
    if(e == nullptr) { return read_handler(nullptr, EReadStatus::Undef); }
    handler_normal();
  }
}

