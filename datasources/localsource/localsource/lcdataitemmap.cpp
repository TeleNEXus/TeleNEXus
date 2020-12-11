#include "lcdataitemmap.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include "LIRemoteDataSource.h"

//==============================================================================
void LCDataItemMap::CDataItemBase::notifyReaders(const QByteArray& _data)
{
  for(auto it = mReadersList.begin(); 
      it != mReadersList.end();
      it++)
  {
    auto sp = it->lock();
    if(sp.isNull()) 
    {
      mReadersList.erase(it);
      continue;
    }
    sp->notifyListener(_data);
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
int LCDataItemMap::CDataItemBytes::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  if(_data.size() != mData.size()) return 0;
  mData = _data;
  notifyReaders(_data);
  return mData.size();
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

  notifyReaders(notify_data);

  return mData.size();
}

//------------------------------------------------------------------------------
QByteArray LCDataItemMap::CDataItemBits::getData()
{
  QByteArray ret_data;
  for(int i = 0; i < mData.size(); i++)
  {
    ret_data[i] = (mData.at(i) == false) ? (0):(1);
  }
  return ret_data;
}

//==============================================================================
LCDataItemMap::LCDataItemMap()
{
}

//------------------------------------------------------------------------------
void LCDataItemMap::addItem(const QString& _id, const QByteArray& _data)
{
  if(_data.isNull()) return;
  auto item = 
    QSharedPointer<CDataItemBase>(new CDataItemBytes(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addItem(const QString& _id, const QBitArray& _data)
{
  if(_data.isNull()) return;
  auto item  = 
    QSharedPointer<CDataItemBase>(new CDataItemBits(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::readData(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it ==  mDataMap.end()) 
  {
    _sp_reader->notifyListener(LERemoteDataStatus::DS_UNDEF);
    return;
  }
  _sp_reader->notifyListener(it.value()->getData());
}

//------------------------------------------------------------------------------
void LCDataItemMap::writeData(
    QSharedPointer<LCQLocalDataWriter> _sp_writer,
    const QByteArray& _data)
{
  auto it = mDataMap.find(_sp_writer->getDataName());
  LERemoteDataStatus ret_status = LERemoteDataStatus::DS_OK;
  if(it ==  mDataMap.end())
  {
    ret_status = LERemoteDataStatus::DS_WRONG;
  }else if(it.value()->setData(_data) <= 0)
  {
    ret_status = LERemoteDataStatus::DS_WRONG;
  }
  _sp_writer->notifyListener(ret_status);
}

//------------------------------------------------------------------------------
void LCDataItemMap::connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it == mDataMap.end()) 
  {
    _sp_reader->notifyListener(LERemoteDataStatus::DS_UNDEF);
    return;
  }
  it.value()->connectReader(_sp_reader);
  _sp_reader->notifyListener(it->data()->getData());
}

//------------------------------------------------------------------------------
void LCDataItemMap::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it ==  mDataMap.end()) return;
  it.value()->disconnectReader(_sp_reader);
}

