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
#ifndef LQMODBUSMASTERTCP_H
#define LQMODBUSMASTERTCP_H

#include "lqmodbusmasterbase.h"

#include <QModbusTcpClient>
#include <QUrl>

class QThread;


class LQModbusMasterTcp : public LQModbusMasterBase
{
private:
  quint16             mPort;
  QString             mHostName;
  int                 mTimeout;
  int                 mRetries;
  QModbusTcpClient*   mpMaster;
  QThread*            mpThread;

private:
  LQModbusMasterTcp() = delete;
  LQModbusMasterTcp(const LQModbusMasterTcp&) = delete;
  LQModbusMasterTcp& operator=(const LQModbusMasterTcp&) = delete;
  explicit LQModbusMasterTcp(QObject *_parent = nullptr);
  virtual ~LQModbusMasterTcp();

public:
  static QSharedPointer<LQModbusMasterTcp> create();

public slots:
  void connectToHost(const QUrl& _url, int _timeout, int _retries);
  void disconnectFromHost();

private:
  virtual QModbusClient* createMaster() override;
  virtual QModbusClient* getMaster() override;
};

#endif // LQMODBUSMASTERTCP_H
