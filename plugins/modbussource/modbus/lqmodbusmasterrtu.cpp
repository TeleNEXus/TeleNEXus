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
#include "lqmodbusmasterrtu.h"
#include <QDebug>
#include <QThread>

//==============================================================================deleterLater
static void deleterLater(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LQModbusMasterRtu
LQModbusMasterRtu::LQModbusMasterRtu(QObject* _parent) :
  LQModbusMasterBase(_parent),
  mBaudRate (QSerialPort::BaudRate::UnknownBaud),
  mParity (QSerialPort::Parity::UnknownParity),
  mDataBits (QSerialPort::DataBits::UnknownDataBits),
  mStopBits (QSerialPort::StopBits::UnknownStopBits),
  mTimeout(500),
  mRetries(3),
  mpMaster(nullptr),
  mpThread(nullptr)
{
}

//------------------------------------------------------------------------------
LQModbusMasterRtu::~LQModbusMasterRtu()
{
  mpThread->quit();
  mpThread->deleteLater();
}

//------------------------------------------------------------------------------
void LQModbusMasterRtu::connectToPort(
    QString                 _portName,
    QSerialPort::BaudRate   _baudRate,
    QSerialPort::Parity     _parity,
    QSerialPort::DataBits   _dataBits,
    QSerialPort::StopBits   _stopBits,
    int                     _timeout,
    int                     _retries)
{
  mPortName   = _portName;
  mParity     = _parity;
  mBaudRate   = _baudRate;
  mDataBits   = _dataBits;
  mStopBits   = _stopBits;
  mTimeout    = _timeout;
  mRetries    = _retries;
  connectRequest();
}

//------------------------------------------------------------------------------
void LQModbusMasterRtu::disconnectFromPort()
{
  //TODO: add code.
}


//------------------------------------------------------------------------------
QSharedPointer<LQModbusMasterRtu> LQModbusMasterRtu::create()
{
  LQModbusMasterRtu* master = new LQModbusMasterRtu(nullptr);
  master->mpThread = new QThread;
  master->moveToThread(master->mpThread);
  master->mpThread->start();
  return QSharedPointer<LQModbusMasterRtu>(master, deleterLater);
}

//------------------------------------------------------------------------------
QModbusClient* LQModbusMasterRtu::createMaster()
{
  if(mpMaster != nullptr)
  {
    mpMaster->disconnectDevice();
    mpMaster->deleteLater();
  }

  mpMaster = new QModbusRtuSerialMaster;

  mpMaster->setConnectionParameter(
      QModbusDevice::SerialPortNameParameter, mPortName);

  mpMaster->setConnectionParameter(
      QModbusDevice::SerialBaudRateParameter, mBaudRate);

  mpMaster->setConnectionParameter(
      QModbusDevice::SerialParityParameter,   mParity);

  mpMaster->setConnectionParameter(
      QModbusDevice::SerialDataBitsParameter, mDataBits);

  mpMaster->setConnectionParameter(
      QModbusDevice::SerialStopBitsParameter, mStopBits);

  mpMaster->setTimeout(mTimeout);

  mpMaster->setNumberOfRetries(mRetries);

  return mpMaster;
}

//------------------------------------------------------------------------------
QModbusClient* LQModbusMasterRtu::getMaster()
{
  return mpMaster;
}

