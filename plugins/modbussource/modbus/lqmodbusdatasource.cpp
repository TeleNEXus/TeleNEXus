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
#include <QThread>
#include <typeinfo>
#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>

#include "lqmodbusdatasource.h"
#include "lmodbusdefs.h"

#include "lqmodbusdatareader.h"
#include "lqmodbusdatawriter.h"
#include "lqreadsync.h"
#include "lqwritesync.h"

using EWriteStatus = LIRemoteDataSource::EWriteStatus;
//==============================================================================CQEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataSource::CQEventBase);

LQModbusDataSource::CQEventBase::CQEventBase() :
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventStart::CQEventStart(int _updateInterval) :
  mUpdateInterval(_updateInterval)
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventStart::handle(LQModbusDataSource* _sender)
{
  _sender->mUpdateInterval = mUpdateInterval;
  _sender->mTimer.start(mUpdateInterval);
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventStop::CQEventStop()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventStop::handle(LQModbusDataSource* _sender)
{
  _sender->mTimer.stop();
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventReqRead::CQEventReqRead(
    QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventReqRead::handle(LQModbusDataSource* _sender)
{
  _sender->mDataMap.read(mspReader);
}

//==============================================================================CQEventReqWrite
LQModbusDataSource::CQEventReqWrite::CQEventReqWrite(
    QSharedPointer<LQModbusDataWriter> _sp_writer,
    const QByteArray& _data) :
  mspWriter(_sp_writer),
  mData(_data)
{

}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventReqWrite::handle(LQModbusDataSource* _sender)
{
  _sender->mDataMap.write(mspWriter, mData);
}
//==============================================================================CQEventReqConnectReader
LQModbusDataSource::CQEventReqConnectReader::CQEventReqConnectReader(
    QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventReqConnectReader::handle(
    LQModbusDataSource* _sender)
{
  _sender->mDataMap.connectReader(mspReader);
}
//==============================================================================CQEventReqDisconnectReader
LQModbusDataSource::CQEventReqDisconnectReader::
CQEventReqDisconnectReader(QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CQEventReqDisconnectReader::handle(
    LQModbusDataSource* _sender)
{
  _sender->mDataMap.disconnectReader(mspReader);
}

//==============================================================================doDeleteLater
static void doDeleteLater(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LCQModbusDataSource
LQModbusDataSource::LQModbusDataSource(
    quint8 _devId,
    QSharedPointer<LQModbusMasterBase> _modbusMaster,
    quint16 _maxBytesPerReq):
  QObject(nullptr),
  mDevId(_devId),
  mModbusMaster(_modbusMaster),
  mUpdateInterval(500),
  mTimer(this),
  mDataMap(mDevId, _modbusMaster, _maxBytesPerReq)
{
  connect(&mTimer, &QTimer::timeout,
      [=]()
      {
        mDataMap.update();
      });
}

//------------------------------------------------------------------------------
LQModbusDataSource::~LQModbusDataSource()
{
}

//------------------------------------------------------------------------------
QSharedPointer<LQModbusDataSource> LQModbusDataSource::create(
    quint8 _devId,
    QSharedPointer<LQModbusMasterBase> _modbusMaster,
    quint16 _maxBytesPerReq)
{
  if(_maxBytesPerReq > 256) _maxBytesPerReq = 256;
  QSharedPointer<LQModbusDataSource> source(
      new LQModbusDataSource( _devId, _modbusMaster, _maxBytesPerReq), doDeleteLater);
  source->mwpThis = source;
  return source;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemHoldingRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemHoldingRegs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemInputRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemInputRegs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemDiscreteInputs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemDiscreteInputs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemCoils(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemCoils(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::start(int _updateIntervalMs)
{
  QMutexLocker locker(&mStartMutex);

  if(!mspThread.isNull()) return;

  mspThread.reset(new QThread, doDeleteLater);
  moveToThread(mspThread.data());
  mspThread->start();

  _updateIntervalMs = (_updateIntervalMs > 50) ? (_updateIntervalMs) : (50);

  QCoreApplication::postEvent(
      this, new CQEventStart(_updateIntervalMs));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::stop()
{
  QCoreApplication::postEvent(this, new CQEventStop);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::connectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqConnectReader(_reader));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::disconnectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqDisconnectReader(_reader));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::readerRead(QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqRead(_reader));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::writerWrite(
    QSharedPointer<LQModbusDataWriter> _writer, const QByteArray& _data)
{
  QCoreApplication::postEvent( this, new CQEventReqWrite(_writer, _data));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::customEvent(QEvent* _event)
{
  if(_event->type() == CQEventBase::msExtendedEventType)
  {
    static_cast<CQEventBase*>(_event)->handle(this);
  }
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataReader> LQModbusDataSource::createReader(
    const QString& _dataName)
{
  return LQModbusDataReader::create(_dataName, mwpThis);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataWriter> LQModbusDataSource::createWriter(
    const QString& _dataName)
{
  return LQModbusDataWriter::create(_dataName, mwpThis);
}

//------------------------------------------------------------------------------
QByteArray LQModbusDataSource::read(
    const QString& _dataId, EReadStatus* _status)
{
  auto req = LQReadSyncReq::create(mwpThis.lock(), _dataId, this->thread());
  return req->readSync(_status);
}

//------------------------------------------------------------------------------
EWriteStatus LQModbusDataSource::write(
    const QString& _dataId, const QByteArray& _data)
{
  auto req = LQWriteSyncReq::create(mwpThis.lock(), _dataId, thread());
  return req->writeSync(_data);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::read(
      const QString& _dataId, TReadHandler _handler)
{
  auto reader = LQModbusDataReader::create(_dataId, mwpThis);
  if(reader.isNull())
  {
    _handler(QSharedPointer<QByteArray>(new QByteArray), EReadStatus::Undef);
    return;
  }
  reader->setHandler(_handler);
  reader->readRequest();
}

//------------------------------------------------------------------------------
void LQModbusDataSource::write(
      const QString& _dataId, 
      const QByteArray& _data,
      TWriteHandler _handler)
{
  auto writer = LQModbusDataWriter::create(_dataId, mwpThis);
  if(writer.isNull())
  {
    _handler(EWriteStatus::Failure);
    return;
  }
  writer->setHandler(_handler);
  writer->writeRequest(_data);
}
