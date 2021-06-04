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
#ifndef LQMODBUSMASTERRTU_H
#define LQMODBUSMASTERRTU_H

#include "lqmodbusmasterbase.h"
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

class QThread;


class LQModbusMasterRtu : public LQModbusMasterBase
{
  Q_OBJECT;

private:
  QString mPortName;
  QSerialPort::BaudRate mBaudRate;
  QSerialPort::Parity mParity;
  QSerialPort::DataBits mDataBits;
  QSerialPort::StopBits mStopBits;
  int mTimeout;
  QModbusRtuSerialMaster* mpMaster;
  QThread* mpThread;

private:
  LQModbusMasterRtu() = delete;
  LQModbusMasterRtu(const LQModbusMasterRtu&) = delete;
  LQModbusMasterRtu& operator=(const LQModbusMasterRtu&) = delete;
  explicit LQModbusMasterRtu(QObject* _parent = nullptr);
  virtual ~LQModbusMasterRtu();

public:
  static QSharedPointer<LQModbusMasterRtu> create();

  public slots:
    void connectToPort( 
        QString                 _portName,
        QSerialPort::BaudRate   _baudRate,
        QSerialPort::Parity     _parity,
        QSerialPort::DataBits   _dataBits,
        QSerialPort::StopBits   _stopBits,
        int                     _timeout = 500);
  void disconnectFromPort();

protected:
  virtual QModbusClient* createMaster() override;
  virtual QModbusClient* getMaster() override;
};

#endif // LQMODBUSMASTERRTU_H
