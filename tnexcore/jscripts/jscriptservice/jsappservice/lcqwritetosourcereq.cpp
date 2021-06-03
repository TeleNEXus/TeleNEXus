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
#include "lcqwritetosourcereq.h"
#include "applicationinterface.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQWriteToSource::CEventBase);

LCQWriteToSource::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventRead
LCQWriteToSource::CEventWrite::CEventWrite()
{
}

//------------------------------------------------------------------------------
void LCQWriteToSource::CEventWrite::handle(LCQWriteToSource* _sender)
{
  auto source = 
    CApplicationInterface::getInstance().getDataSource(_sender->mSourceId);
  if(source.isNull()) 
  {
    _sender->mWaitCond.wakeOne();
    return;
  }

  _sender->mspDataWriter = source->createWriter( 
      _sender->mDataId, 
      [_sender](LERemoteDataStatus _status)
      {
        if(_status == LERemoteDataStatus::DS_OK) 
        {
          _sender->mWriteDataSize = _sender->edWriteData.size();
        }
        _sender->mWaitCond.wakeOne();
      });

  _sender->mspDataWriter->writeRequest(_sender->edWriteData);
}

//==============================================================================requestDeleter
static void requestDeleter(LCQWriteToSource* _req)
{
  qDebug() << "Write Request Deleter";
  _req->deleteLater();
}

//==============================================================================LCQWriteToSource
LCQWriteToSource::LCQWriteToSource(
    const QString&      _sourceId,
    const QString&      _dataId,
    const QByteArray&   _writeData
    ) :
  QObject(nullptr),
  mSourceId(_sourceId),
  mDataId(_dataId),
  edWriteData(_writeData),
  mWriteDataSize(0)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQWriteToSource> LCQWriteToSource::create(
    const QString&      _sourceId,
    const QString&      _dataId,
    const QByteArray&   _writeData,
    QThread* _thread)
{
  LCQWriteToSource* req = new LCQWriteToSource(_sourceId, _dataId, _writeData);
  req->moveToThread(_thread);
  return QSharedPointer<LCQWriteToSource>(req, requestDeleter);
}


//------------------------------------------------------------------------------
LCQWriteToSource::~LCQWriteToSource()
{
}

//------------------------------------------------------------------------------
int LCQWriteToSource::writeData()
{
  mMutexEvent.lock();

  QCoreApplication::postEvent(this, new CEventWrite());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  return mWriteDataSize;
}

//------------------------------------------------------------------------------
void LCQWriteToSource::customEvent(QEvent* _event)
{
  mMutexEvent.lock();
  mMutexEvent.unlock();

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

