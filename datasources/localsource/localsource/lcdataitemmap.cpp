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
      edReadersMap.remove(*it);
      continue;
    }
    sp->notifyListener(_data);
  }
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBase::connectReader(
    QWeakPointer<LCQLocalDataReader> _pw_reader)
{
  auto sp = _pw_reader.lock();
  if(sp.isNull()) return;
  mReadersList << sp;
}

//------------------------------------------------------------------------------
void LCDataItemMap::CDataItemBase::disconnectReader(
    QWeakPointer<LCQLocalDataReader> _sw_reader)
{
  mReadersList.removeOne(_sw_reader);
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
  QByteArray ret_data(mData.size(), 0);
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
    QSharedPointer<CDataItemBase>(new CDataItemBytes(_data, mReadersMap));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addItem(const QString& _id, const QBitArray& _data)
{
  if(_data.isNull()) return;
  auto item  = 
    QSharedPointer<CDataItemBase>(new CDataItemBits(_data, mReadersMap));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
int LCDataItemMap::setData(const QString& _id, const QByteArray& _data)
{
  auto it = mDataMap.find(_id);
  if(it == mDataMap.end()) return -1;
  return it->data()->setData(_data);
}

//------------------------------------------------------------------------------
QByteArray LCDataItemMap::getData(const QString& _id)
{
  auto it = mDataMap.find(_id);
  if(it ==  mDataMap.end()) return QByteArray();
  return it->data()->getData();
}

//------------------------------------------------------------------------------
void LCDataItemMap::connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mDataMap.find(_sp_reader->getDataName());
  if(it == mDataMap.end()) return;
  it.value()->connectReader(_sp_reader);

  mReadersMap.insert(_sp_reader, _sp_reader->getDataName());
}

//------------------------------------------------------------------------------
void LCDataItemMap::disconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  auto it = mReadersMap.find(_sp_reader);
  if(it == mReadersMap.end()) return;
  mDataMap.find(it.value()).value()->disconnectReader(_sp_reader);
  mReadersMap.erase(it);
}


