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
#include "lqmodbusdatawriter.h"
#include "lqmodbusdatasource.h"

#include <QDebug>

#include <QCoreApplication>

//==============================================================================CQEventDataIsWrite
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataWriter::CQEventDataIsWrite);

LQModbusDataWriter::CQEventDataIsWrite::CQEventDataIsWrite(
    LERemoteDataStatus _status) : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
  mStatus(_status)
{
}

//==============================================================================doDeleteLater
static void doDeleteLater(LQModbusDataWriter* _writer)
{
  _writer->deleteLater();
}

//==============================================================================LQModbusDataWriter
LQModbusDataWriter::LQModbusDataWriter(
    const QString& _dataName,
    LTWriteAction _writeAction,
    QSharedPointer<LQModbusDataSource> _dataSource) : 
  QObject(nullptr),
  mDataName(_dataName),
  mWriteAction(_writeAction),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LQModbusDataWriter::~LQModbusDataWriter()
{
}

//------------------------------------------------------------------------------
QSharedPointer<LQModbusDataWriter> LQModbusDataWriter::create(
    const QString& _dataName,
    LTWriteAction _writeAction,
    QSharedPointer<LQModbusDataSource> _dataSource)
{
  auto sp  = QSharedPointer<LQModbusDataWriter>(
      new LQModbusDataWriter(_dataName, _writeAction, _dataSource),
      doDeleteLater);
  sp->mwpThis = sp;
  return sp;
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::writeRequest(const QByteArray& _data)
{
  auto sp = mwpDataSource.lock();

  if(sp.isNull())
  {
    mWriteAction(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp.data()->write(mwpThis.lock(), _data);
  }
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsWrite(_status));
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::customEvent(QEvent *_event)
{
  if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
  {
    CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
    if(e == nullptr)
    {
      mWriteAction(LERemoteDataStatus::DS_WRONG);
    }
    else
    {
      mWriteAction(e->mStatus);
    }
  }
}

