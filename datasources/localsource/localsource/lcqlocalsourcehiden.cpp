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
#include "lcqlocalsourcehiden.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include <QCoreApplication>
#include <QDebug>

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalSourceHiden::CEventBase);

LCQLocalSourceHiden::CEventBase::CEventBase() :
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventConnectReader
LCQLocalSourceHiden::CEventConnectReader::CEventConnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventConnectReader::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.connectReader(mspDataReader);
}

//==============================================================================CEventDisconnectReader
LCQLocalSourceHiden::CEventDisconnectReader::CEventDisconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventDisconnectReader::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.disconnectReader(mspDataReader);
}

//==============================================================================CEventRead
LCQLocalSourceHiden::CEventRead::CEventRead(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventRead::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.readData(mspDataReader);
}

//==============================================================================CEventWrite
LCQLocalSourceHiden::CEventWrite::CEventWrite(
    QSharedPointer<LCQLocalDataWriter> _sp_writer,
    const QByteArray& _data
    ) :
  mspDataWriter(_sp_writer),
  mData(_data)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventWrite::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.writeData(mspDataWriter, mData);
}

//==============================================================================
void pointerDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//==============================================================================LCQLocalSourceHiden
LCQLocalSourceHiden::LCQLocalSourceHiden() :
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
LCQLocalSourceHiden::~LCQLocalSourceHiden()
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalSourceHiden> LCQLocalSourceHiden::create()
{
  auto sp = QSharedPointer<LCQLocalSourceHiden>(
      new LCQLocalSourceHiden(),
      pointerDeleter);
  sp->mwpThis= sp;
  return sp;
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addDataItem(const QString& _id, const QByteArray& _data)
{
  mDataMap.addDataItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addDataItem(const QString& _id, const QBitArray& _data)
{
  mDataMap.addDataItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addDataItem(const QString& _id, const QString& _data)
{
  mDataMap.addDataItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addStreamItem(const QString& _id)
{
  mDataMap.addStreamItem(_id);
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataReader> LCQLocalSourceHiden::createReader(
    const QString& _dataName, 
    LTReadAction _readAction)
{
  return LCQLocalDataReader::create(_dataName, _readAction, mwpThis);
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataWriter> LCQLocalSourceHiden::createWriter(
    const QString& _dataName, 
    LTWriteAction _writeAction)
{
  return LCQLocalDataWriter::create(_dataName, _writeAction, mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::customEvent(QEvent* _event)
{
  if(_event->type() != CEventBase::msExtendedEventType) return;
  CEventBase* e = dynamic_cast<CEventBase*>(_event);
  if(e == nullptr) return;
  e->handle(this);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::connectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QCoreApplication::postEvent(this, new CEventConnectReader(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QCoreApplication::postEvent(this, new CEventDisconnectReader(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::read(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QCoreApplication::postEvent(this, new CEventRead(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::write(
    const QByteArray& _data, 
    QSharedPointer<LCQLocalDataWriter> _sp_writer)
{
  QCoreApplication::postEvent(this, new CEventWrite(_sp_writer, _data));
}



