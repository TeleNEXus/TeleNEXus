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
#include "lcxmlmain.h"
#include "LIApplication.h"

#include <QCoreApplication>
#include <QDebug>

/* //==============================================================================CReadListener */
/* LCQReadFromSourceReq::CReadListener::CReadListener(LCQReadFromSourceReq* _req) : */
/*   mpRequest(_req) */
/* { */
/* } */

/* void LCQReadFromSourceReq::CReadListener::dataIsRead( */
/*     QSharedPointer<QByteArray>  _data, */ 
/*     LERemoteDataStatus          _status) */
/* { */
/*   qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 0"; */
/*   if(_status == LERemoteDataStatus::DS_OK) mpRequest->mRetData = *_data.data(); */
/*   qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 1"; */
/*   mpRequest->mWaitCond.wakeOne(); */
/*   qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 2"; */
/* } */

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
    LCXmlMain::getApplicationInterface().getDataSource(_sender->mSourceId);
  if(source.isNull()) 
  {
    _sender->mWaitCond.wakeOne();
    return;
  }

  _sender->mspDataReader = source->createReader( _sender->mDataId, 
      [_sender](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
      {
        qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 0";
        if(_status == LERemoteDataStatus::DS_OK) _sender->mRetData = *_data.data();
        qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 1";
        _sender->mWaitCond.wakeOne();
        qDebug() << "LCQReadFromSourceReq::CReadListener::dataIsRead 2";
      });

  _sender->mspDataReader->readRequest();
}

static void requestDeleter(LCQReadFromSourceReq* _req)
{
  qDebug() << "Read Request Deleter";
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
  qDebug() << "LCQReadFromSourceReq::customEvent 0";

  if(_event->type() == CEventBase::msExtendedEventType)
  {
  qDebug() << "LCQReadFromSourceReq::customEvent 1";
    CEventBase* e = dynamic_cast<CEventBase*>(_event);
  qDebug() << "LCQReadFromSourceReq::customEvent 2";
    if(e == nullptr)
    {
  qDebug() << "LCQReadFromSourceReq::customEvent 3";
      return;
    }
  qDebug() << "LCQReadFromSourceReq::customEvent 4";
    e->handle(this);
  }
  qDebug() << "LCQReadFromSourceReq::customEvent 5";
}

