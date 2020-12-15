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
#include "lcqlocaldatawriter.h"
#include "lcqlocalsourcehiden.h"
#include <QCoreApplication>

//==============================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataWriter::CQEventDataIsWrite);

LCQLocalDataWriter::CQEventDataIsWrite::CQEventDataIsWrite(
    LERemoteDataStatus _status) : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
  mStatus(_status)
{
}

//==============================================================================
LCQLocalDataWriter::LCQLocalDataWriter(
    const QString& _dataName,
    LTWriteAction _writeListener,
    QSharedPointer<LCQLocalSourceHiden> _dataSource):
  QObject(nullptr),
  mDataName(_dataName),
  mListener(_writeListener),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LCQLocalDataWriter::~LCQLocalDataWriter()
{
}

//==============================================================================
static void pointerDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataWriter> LCQLocalDataWriter::create(
    const QString& _dataName,
    LTWriteAction _writeListener,
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp_writer = 
    QSharedPointer<LCQLocalDataWriter>(
        new LCQLocalDataWriter(_dataName, _writeListener, _dataSource), 
        pointerDeleter);
  sp_writer->mwpThis = sp_writer;
  return sp_writer;
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::writeRequest(const QByteArray& _data)
{
  auto sp = mwpDataSource.lock();
  if(sp.isNull())
  {
    mListener(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp->write(_data, mwpThis);
  }
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsWrite(_status));
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::customEvent(QEvent *_event)
{
  if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
  {
    CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
    if(e == nullptr)
    {
      mListener(LERemoteDataStatus::DS_WRONG);
    }
    else
    {
      mListener(e->mStatus);
    }
  }
}

