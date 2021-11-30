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
#include "lcqreadfromsourcereq.h"
#include "applicationinterface.h"

#include <QCoreApplication>
#include <QDebug>

using EReadStatus = LIRemoteDataReader::EReadStatus;
//==============================================================================CReaderStub
class CReaderStub : public LIRemoteDataReader
{
public:
  CReaderStub(){}
  virtual ~CReaderStub(){}
  virtual void readRequest()override {}
  virtual void connectToSource()override {}
  virtual void disconnectFromSource()override {}
  virtual void setHandler(THandler)override {}
};

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQReadFromSourceReq::CEventBase);

LCQReadFromSourceReq::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventRead
LCQReadFromSourceReq::CEventRead::CEventRead()
{
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::CEventRead::handle(LCQReadFromSourceReq* _sender)
{

  auto source = 
    CApplicationInterface::getInstance().getDataSource(_sender->mSourceId);

  if(source.isNull()) 
  {
    _sender->mWaitCond.wakeOne();
    return;
  }


  auto read_handler = 
      [_sender](QSharedPointer<QByteArray> _data, EReadStatus _status)
      {
        if(_status == EReadStatus::Valid) 
        {
          _sender->mRetData = *_data.data();
        }
        _sender->mWaitCond.wakeOne();
      };

  _sender->mspDataReader = source->createReader(_sender->mDataId);

  if(!_sender->mspDataReader.isNull())
  {
    _sender->mspDataReader->setHandler(read_handler);
    _sender->mspDataReader->readRequest();
  }
  else
  {
    _sender->mspDataReader = QSharedPointer<LIRemoteDataReader>(new CReaderStub);
  }
}

static void requestDeleter(LCQReadFromSourceReq* _req)
{
  _req->deleteLater();
}

//==============================================================================LCQReadFromSourceReq
LCQReadFromSourceReq::LCQReadFromSourceReq(
    const QString& _sourceId,
    const QString& _dataId) :
  QObject(nullptr),
  mSourceId(_sourceId),
  mDataId(_dataId)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQReadFromSourceReq> LCQReadFromSourceReq::create(
    const QString& _sourceId,
    const QString& _dataId,
    QThread* _thread)
{
  LCQReadFromSourceReq* req = new LCQReadFromSourceReq(_sourceId, _dataId);
  req->moveToThread(_thread);
  return QSharedPointer<LCQReadFromSourceReq>(req, requestDeleter);
}


//------------------------------------------------------------------------------
LCQReadFromSourceReq::~LCQReadFromSourceReq()
{
}

//------------------------------------------------------------------------------
QByteArray LCQReadFromSourceReq::readData()
{
  mMutexEvent.lock();

  QCoreApplication::postEvent(this, new CEventRead());
  mWaitCond.wait(&mMutexEvent);

  mMutexEvent.unlock();

  return mRetData;
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::customEvent(QEvent* _event)
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

