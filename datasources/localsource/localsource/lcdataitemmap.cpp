#include "lcdataitemmap.h"

//==============================================================================
int LCDataItemMap::CDataItemBytes::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  if(_data.size() != mData.size()) return 0;
  mData = _data;
  return mData.size();
}

//==============================================================================
int LCDataItemMap::CDataItemBits::setData(const QByteArray& _data)
{
  if(_data.isNull()) return 0;
  if(_data.size() != mData.size()) return 0;
  for(int i = 0; i < _data.size(); i++)
  {
    mData[i] = (_data.at(i) == 0) ? (false):(true);
  }
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
  auto item = QSharedPointer<CDataItemBase>(new CDataItemBytes(_data));
  mDataMap.insert(_id, item);
}

//------------------------------------------------------------------------------
void LCDataItemMap::addItem(const QString& _id, const QBitArray& _data)
{
  if(_data.isNull()) return;
  auto item  = QSharedPointer<CDataItemBase>(new CDataItemBits(_data));
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

