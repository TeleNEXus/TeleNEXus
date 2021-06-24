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


#include "lcqlocaldatareader.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataReader::CQEventDataIsRead);

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead(
        const QByteArray& _data,
        EReadStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray(_data)),
    mStatus(_status)
{
}

//------------------------------------------------------------------------------
LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead( 
        EReadStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================
static void pointerDeleter(LCQLocalDataReader* _reader)
{
  _reader->deleteLater();
}

//==============================================================================LCQRemoteDataListener
LCQLocalDataReader::LCQLocalDataReader(
    const QString& _dataName, 
    QSharedPointer<LCQLocalSourceHiden> _dataSource) :   
  QObject(nullptr),
  mDataName(_dataName),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LCQLocalDataReader::~LCQLocalDataReader()
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataReader> LCQLocalDataReader::create(
    const QString& _dataName, 
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp = QSharedPointer<LCQLocalDataReader>(
      new LCQLocalDataReader( _dataName, _dataSource), 
      pointerDeleter);
  sp->mwpThis = sp;
  return sp;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::readRequest()
{
  auto sp = mwpDataSource.lock();
  if(sp.isNull()) return;
  sp->read(mwpThis.lock());
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::connectToSource()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->connectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::disconnectFromSource()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->disconnectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::notify(const QByteArray& _data)
{
  QCoreApplication::postEvent(this, 
      new CQEventDataIsRead(_data, EReadStatus::Valid));
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::notify(EReadStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsRead(_status));
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::customEvent(QEvent* _event)
{
  if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;
  CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);
  if(e == nullptr) return;
  mHandler(e->mspData, e->mStatus);
}

