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
#include "lqmodbusdatareader.h"
#include "lqmodbusdatasource.h"

#include <QCoreApplication>
#include <QDebug>

//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataReader::CQEventDataIsRead);

LQModbusDataReader:: CQEventDataIsRead:: CQEventDataIsRead(
        const QByteArray& _data,
        LERemoteDataStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray(_data)),
    mStatus(_status)
{
}

LQModbusDataReader:: CQEventDataIsRead:: CQEventDataIsRead( 
        LERemoteDataStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================LCQRemoteDataListener
LQModbusDataReader::LQModbusDataReader(
    const QString& _dataName,
    LTReadAction _readAction,
    QWeakPointer<LQModbusDataSource> _dataSource) :   
  QObject(nullptr),
  mDataName(_dataName),
  mReadAction(_readAction),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LQModbusDataReader::~LQModbusDataReader()
{
}

//==============================================================================create
static void doDeleteLater(LQModbusDataReader* _reader)
{
  qDebug() << "LQModbusDataReader  doDeleteLater";
  _reader->deleteLater();
}

QSharedPointer<LQModbusDataReader> LQModbusDataReader::create(
    const QString& _dataName, 
    LTReadAction _readAction,
    QWeakPointer<LQModbusDataSource> _dataSource)
{
  auto sp = QSharedPointer<LQModbusDataReader>(
      new LQModbusDataReader( _dataName, _readAction, _dataSource), 
      doDeleteLater);
  sp->mwpThis = sp;
  return sp;
}

//------------------------------------------------------------------------------
void LQModbusDataReader::readRequest()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->read(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::connectToSource()
{
    qDebug() << "//-------------------------------------------------------------";
    qDebug() << "LCQModbusDataReader::connectToSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->connectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::disconnectFromSource()
{
    qDebug() << "LQModbusDataReader::disconnectFromSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(mwpThis);
}

//------------------------------------------------------------------------------
void LQModbusDataReader::notifyListener(
    const QByteArray& _data, LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, 
      new CQEventDataIsRead(_data, _status));
}

//------------------------------------------------------------------------------
void LQModbusDataReader::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsRead(_status));
}

//------------------------------------------------------------------------------
void LQModbusDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;
    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);
    if(e == nullptr) return;
    mReadAction(e->mData, e->mStatus);
}

