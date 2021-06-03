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
#include "lcqjsappservice.h"
#include "lcqreadfromsourcereq.h"
#include "lcqwritetosourcereq.h"
#include "LIApplication.h"

#include <QMutex>
#include <QThread>
#include <QDebug>

//==============================================================================LCQJSAppService
LCQJSAppService::LCQJSAppService() : 
  mpThread(new QThread)
{
  mpThread->start();
}

//------------------------------------------------------------------------------
LCQJSAppService::~LCQJSAppService()
{
  mpThread->quit();
  mpThread->wait(1000);
  mpThread->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LCQJSAppService> LCQJSAppService::getService()
{
  static QWeakPointer<LCQJSAppService> wp_inst;
  static QMutex mutex;
  QMutexLocker locker(&mutex);

  auto sp_inst = wp_inst.toStrongRef();
  if(sp_inst.isNull())
  {
    sp_inst = 
      QSharedPointer<LCQJSAppService>(new LCQJSAppService);
    wp_inst = sp_inst;
  }
  return sp_inst;
}

//------------------------------------------------------------------------------
QByteArray LCQJSAppService::readData(
    const QString& _sourceId, const QString& _dataId)
{
  auto req = LCQReadFromSourceReq::create(_sourceId, _dataId, mpThread);
  return req->readData();
}

//------------------------------------------------------------------------------
int LCQJSAppService::writeData(
    const QString& _sourceId, 
    const QString& _dataId, 
    const QByteArray& _data)
{
  auto req = LCQWriteToSource::create(_sourceId, _dataId, _data, mpThread);
  return req->writeData();
}


