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
#include "lqmodbusmastertcp.h"
#include <QDebug>
#include <QThread>


//==============================================================================deleterLater(
static void deleterLater(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LQModbusMasterTcp
LQModbusMasterTcp::LQModbusMasterTcp(QObject *_parent) :
  LQModbusMasterBase(_parent),
  mPort(502),
  mpMaster(nullptr),
  mpThread(nullptr)
{
}

//------------------------------------------------------------------------------
LQModbusMasterTcp::~LQModbusMasterTcp()
{
  mpThread->quit();
  mpThread->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LQModbusMasterTcp> LQModbusMasterTcp::create()
{
  LQModbusMasterTcp* master = new LQModbusMasterTcp(nullptr);
  master->mpThread = new QThread;
  master->moveToThread(master->mpThread);
  master->mpThread->start();
  return QSharedPointer<LQModbusMasterTcp>(master, deleterLater);
}

//------------------------------------------------------------------------------
void LQModbusMasterTcp::connectToHost(const QUrl& _url, int _timeout, int _retries)
{
  mPort = _url.port();
  mHostName = _url.host();
  mTimeout = _timeout;
  mRetries = _retries;
  connectRequest();
}

//------------------------------------------------------------------------------
void LQModbusMasterTcp::disconnectFromHost()
{
  //TODO: Add Code
}

QModbusClient* LQModbusMasterTcp::createMaster()
{
  if(mpMaster != nullptr)
  {
    //        mpMaster->disconnectDevice();
    mpMaster->deleteLater();
  }

  mpMaster = new QModbusTcpClient();

  mpMaster->setConnectionParameter(QModbusDevice::NetworkPortParameter, mPort);

  mpMaster->setConnectionParameter(
      QModbusDevice::NetworkAddressParameter, mHostName);

  mpMaster->setTimeout(mTimeout);
  mpMaster->setNumberOfRetries(mRetries);

  return mpMaster;
}

QModbusClient* LQModbusMasterTcp::getMaster()
{
  return mpMaster;
}

