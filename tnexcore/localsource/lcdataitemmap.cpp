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
#include "lcdataitemmap.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include "LIRemoteDataSource.h"
#include <QDebug>

using EReadStatus = LIRemoteDataReader::EReadStatus;
using EWriteStatus = LIRemoteDataWriter::EWriteStatus;

//==============================================================================
void LCDataItemMap::CDataItemBase::notifyAll(const QByteArray& _data)
{
  auto it = mReadersList.begin();

  while(it != mReadersList.end())
  {
    auto sp = it->lock();
    if(sp.isNull()) 
    {
      it = mReadersList.erase(it);
      continue;
    }
    sp->notify(_data);
    it++;
  }
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBase::connectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  mReadersList << _sp_reader;
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBase::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  mReadersList.removeAll(_sp_reader);
}

//==============================================================================
void LCDataItemMap::CDataItemStorageBase::connectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  mReadersList << _sp_reader;
  notify(_sp_reader);
}

//==============================================================================
int LCDataItemMap::CDataItemBytes::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  if(_data.size() != mData.size()) return 0;
  mData = _data;
  notifyAll(_data);
  return mData.size();
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBytes::notify(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  _sp_reader->notify(mData);
}


//==============================================================================
int LCDataItemMap::CDataItemBits::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  if(_data.size() != mData.size()) return 0;
  QByteArray notify_data;
  for(int i = 0; i < _data.size(); i++)
  {
    if(_data.at(i) == 0)
    {
      mData[i] = false;
      notify_data[i] = 0;
    }
    else
    {
      mData[i] = true;
      notify_data[i] = 1;
    }
  }

  notifyAll(notify_data);

  return mData.size();
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBits::notify(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QByteArray ret_data;
  for(int i = 0; i < mData.size(); i++)
  {
    ret_data[i] = (mData.at(i) == false) ? (0):(1);
  }
  _sp_reader->notify(ret_data);
}

//==============================================================================
int LCDataItemMap::CDataItemString::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  mData = _data;
  notifyAll(_data);
  return mData.size();
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemString::notify(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  _sp_reader->notify(mData);
}

//==============================================================================
int LCDataItemMap::CDataItemStream::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  notifyAll(_data);
  return _data.size();
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemStream::notify(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  Q_UNUSED(_sp_reader);
}

//==============================================================================
LCDataItemMap::LCDataItemMap()
{
}

//------------------------------------------------------------------------------
void LCDataItemMap::addDataItem(const QString& _id, const QByteArray& _data)
{
  if(_data.isNull()) return;
  auto item = 
    QSharedPointer<CDataItemBase>(new CDataItemBytes(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addDataItem(const QString& _id, const QBitArray& _data)
{
  if(_data.isNull()) return;
  auto item  = 
    QSharedPointer<CDataItemBase>(new CDataItemBits(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addDataItem(const QString& _id, const QString& _data)
{
  if(_data.isNull()) return;
  auto item = 
    QSharedPointer<CDataItemBase>(new CDataItemString(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addStreamItem(const QString& _id)
{
  auto item = 
    QSharedPointer<CDataItemBase>(new CDataItemStream());
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::readData(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it ==  mDataMap.end()) 
  {
    _sp_reader->notify(EReadStatus::Undef);
    return;
  }
  it.value()->notify(_sp_reader);
}

//------------------------------------------------------------------------------
void LCDataItemMap::writeData(
    QSharedPointer<LCQLocalDataWriter> _sp_writer,
    const QByteArray& _data)
{
  auto ret = 
    [&_sp_writer](EWriteStatus _status)
    {
      _sp_writer->notify(_status);
    };
  auto it = mDataMap.find(_sp_writer->getDataName());
  if(_data.size() == 0) return ret(EWriteStatus::Success);
  if(it == mDataMap.end()) return ret(EWriteStatus::Failure);
  if(it.value()->setData(_data) <= 0) return ret(EWriteStatus::Failure);
  return ret(EWriteStatus::Success);
}

//------------------------------------------------------------------------------
void LCDataItemMap::connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it == mDataMap.end()) 
  {
    _sp_reader->notify(EReadStatus::Undef);
    return;
  }
  it.value()->connectReader(_sp_reader);
}

//------------------------------------------------------------------------------
void LCDataItemMap::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it ==  mDataMap.end()) return;
  it.value()->disconnectReader(_sp_reader);
}


