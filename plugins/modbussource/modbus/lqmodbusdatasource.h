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
#ifndef LQMODBUSDATASOURCE_H
#define LQMODBUSDATASOURCE_H

#include <QThread>
#include <QList>
#include <QLinkedList>
#include <QMap>
#include <QTimerEvent>

#include "lqextendevent.h"
#include "lqmodbusmasterbase.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lcmodbusdatamap.h"

class LQModbusDataWriter;
class LQModbusDataReader;

//==============================================================================LCQModbusDataController
class LQModbusDataSource : public QObject, public LIRemoteDataSource
{
  Q_OBJECT;
private:

  using EReadStatus = LIRemoteDataReader::EReadStatus;
  using EWriteStatus= LIRemoteDataWriter::EWriteStatus;
  //----------------------------------------------------------------------------CQEventBase
  class CQEventBase : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CQEventBase();
      virtual void handle(LQModbusDataSource* _sender) = 0;
  };

  //----------------------------------------------------------------------------CQEventStart
  class CQEventStart : public CQEventBase
  {
  private:
    int mUpdateInterval;
  public:
    CQEventStart(int _updateInterval);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //----------------------------------------------------------------------------CQEventStop
  class CQEventStop : public CQEventBase
  {
  public:
    CQEventStop();
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //----------------------------------------------------------------------------CQEventReqRead
  class CQEventReqRead : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqRead(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //----------------------------------------------------------------------------CQEventReqWrite
  class CQEventReqWrite : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataWriter> mspWriter;
    QByteArray  mData;
  public:
    CQEventReqWrite(
        QSharedPointer<LQModbusDataWriter> _sp_writer,
        const QByteArray& _data);

    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //----------------------------------------------------------------------------CQEventReqConnectReader
  class CQEventReqConnectReader : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqConnectReader(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //----------------------------------------------------------------------------CQEventReqDisconnectReader
  class CQEventReqDisconnectReader : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqDisconnectReader(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  quint8 mDevId;
  QSharedPointer<LQModbusMasterBase> mModbusMaster;
  int mUpdateInterval;
  QTimer mTimer;

  LCModbusDataMap mDataMap;
  QSharedPointer<QThread> mspThread;
  QWeakPointer<LQModbusDataSource> mwpThis;

private:
  explicit LQModbusDataSource(
      quint8 _devId,
      QSharedPointer<LQModbusMasterBase> _modbusMaster,
      quint16 _maxBytesPerReq);

  LQModbusDataSource(const LQModbusDataSource&) = delete;
  LQModbusDataSource& operator=(const LQModbusDataSource&) = delete;
public:
  virtual ~LQModbusDataSource();
  static QSharedPointer<LQModbusDataSource> create(
      quint8 _devId,
      QSharedPointer<LQModbusMasterBase> _modbusMaster,
      quint16 _maxBytesPerReq);
  void addDataItemHoldingRegs(    const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemInputRegs(      const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemDiscreteInputs( const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemCoils(          const QString& _name, quint16 _addr, quint16 _size);
  void start(int _updateIntervalMs = 500);
  void start(QSharedPointer<QThread> _thread, int _updateIntervalMs = 500);
  void stop();


private:
  void connectReader(QSharedPointer<LQModbusDataReader> _reader);
  void disconnectReader(QSharedPointer<LQModbusDataReader> _reader);
  void read(QSharedPointer<LQModbusDataReader> _reader);
  void write(QSharedPointer<LQModbusDataWriter> _writer,
      const QByteArray& _data);
  virtual void customEvent(QEvent* _event) override;

public:

  virtual QSharedPointer<LIRemoteDataReader> createReader(
      const QString& _dataName) override;


  virtual QSharedPointer<LIRemoteDataWriter> createWriter(
      const QString& _dataName) override;

  friend class LQModbusDataReader;
  friend class LQModbusDataWriter;
};

#endif // LQMODBUSDATASOURCE_H
