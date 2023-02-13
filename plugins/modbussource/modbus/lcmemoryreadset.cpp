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

#include "lcmemoryreadset.h"
#include <QDebug>

//==============================================================================CrossSegment
template <class T> bool SegmentIsCrossed(T a1, T b1, T a2, T b2)
{
  if((a1 >= a2) && (a1 <= b2)) return true;
  if((b1 >= a2) && (b1 <= b2)) return true;
  if((a2 >= a1) && (a2 <= b1)) return true;
  if((b2 >= a1) && (b2 <= b1)) return true;
  return false;
}

//==============================================================================SgmentIsNeighbour
template <class T> bool SegmentIsNeighbour(T a1, T b1, T a2, T b2)
{
  if(a1 < a2) 
  {
    if(b1+1 == a2) return true;
  }
  else if(a2 < a1)
  {
    if(b2+1 == a1) return true;
  }
  return false;
}

//==============================================================================FindGreaterItem
/* template < typename T> typename QLinkedList<T>::Iterator FindGreaterItem( */
/*     QLinkedList<T>& _list, */ 
/*     const T& _item) */
/* { */
/*   auto it = _list.begin(); */
/*   while(it != _list.end()) */
/*   { */
/*     if(*it > _item) */ 
/*     { */
/*       return it; */
/*     } */
/*     it++; */
/*   }; */
/*   return _list.end(); */
/* } */



//==============================================================================CDataItem

//------------------------------------------------------------------------------
bool LCMemoryReadSet::CDataItem::updateData(FReader& _reader, int  _dataDimention)
{
  QByteArray data;
  int read_status = _reader(first, (second - first + 1), data);
  auto it = mDataList.begin();
  while(it != mDataList.end())
  {
    QSharedPointer<CIData> sp = (*it).lock();
    if(sp.isNull()) return false;
    int local_addr = (sp->getAddress() - first) * _dataDimention;
    sp->setData(data.mid(local_addr, sp->getSize() * _dataDimention), read_status);
    it++;
  }
  return true;
}

//------------------------------------------------------------------------------
LCMemoryReadSet::CDataItem LCMemoryReadSet::CDataItem::unite(
    const CDataItem& _i1, 
    const CDataItem& _i2)
{
  CDataItem ik;

  if(_i1.isNull() || _i2.isNull()) return ik;

  if(_i1.first < _i2.first)
  {
    ik.first = _i1.first; 
  }
  else
  {
    ik.first = _i2.first;
  }

  if(_i1.second > _i2.second)
  {
    ik.second = _i1.second;
  }
  else
  {
    ik.second = _i2.second;
  }
  
  ik.mUniteIndex = _i1.mUniteIndex + _i2.mUniteIndex;
  ik.mDataList = _i1.mDataList + _i2.mDataList;
  return ik;
}

//==============================================================================LCMemoryReadSet
LCMemoryReadSet::LCMemoryReadSet(FReader _reader, int _dataDimention) :
  mfReader(_reader),
  mDataDimention(_dataDimention),
  mFlagForCompil(false)
{
  if(mDataDimention < 1) mDataDimention = 1;
}

//------------------------------------------------------------------------------
LCMemoryReadSet::~LCMemoryReadSet()
{
}

//------------------------------------------------------------------------------
void LCMemoryReadSet::compilDataMap()
{
  mListCompil.clear();

  enum class EState
  {
    st0,
    st1
  };

  EState state = EState::st0;
  CMemoryDataMap::Iterator itdm = mDataMap.begin();

  CDataItem item_acc;

  auto pass = 
    [&]()
    {
      switch(state)
      {
      case EState::st0:
        if(itdm == mDataMap.end())
        {
          return false;
        }

        if(itdm.value().lock().isNull())
        {
          itdm = mDataMap.erase(itdm);
          break;
        }

        item_acc = CDataItem(itdm.key().first, itdm.key().second, itdm.value());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
        state = EState::st1;
      case EState::st1:
#pragma GCC diagnostic pop
        {
          itdm++;
          CDataItem next_item;

          if(itdm == mDataMap.end())
          {
            mListCompil.push_back(item_acc);
            return false;
          }

          if(itdm.value().lock().isNull())
          {
            itdm = mDataMap.erase(itdm);
            itdm--;
            break;
          }

          next_item = CDataItem(
              itdm.key().first, itdm.key().second, itdm.value());

          if(SegmentIsCrossed<int>(
                item_acc.first, item_acc.second,
                next_item.first, next_item.second) || 
              SegmentIsNeighbour<int>(
                item_acc.first, item_acc.second,
                next_item.first, next_item.second))
          {
            auto ibuff = CDataItem::unite(item_acc, next_item);
            if(!ibuff.isNull())
            {
              item_acc = ibuff;
              break;
            }
          }
          
          mListCompil.push_back(item_acc);
          state = EState::st0;
        }
        break;

      default:
        return false;
        break;
      }
      return true;
    };

  while(pass()){}

  mFlagForCompil = false;
}

//------------------------------------------------------------------------------
using CIData = LCMemoryReadSet::CIData;

//------------------------------------------------------------------------------
void LCMemoryReadSet::insert(QSharedPointer<CIData> _spData)
{

  if(_spData->getSize() < 1) return;
  if(_spData->getAddress() < 0) return;

  CAddrPair key(_spData->getAddress(), _spData->getAddress() + _spData->getSize() - 1);

  if(mDataMap.contains(key, _spData.toWeakRef())) return;

  mDataMap.insert(key, _spData.toWeakRef());

  mFlagForCompil = true;
}

//------------------------------------------------------------------------------
void LCMemoryReadSet::remove(QSharedPointer<CIData> _spData)
{
  mDataMap.remove(
      CAddrPair(_spData->getAddress(), _spData->getAddress() + _spData->getSize() - 1), 
      _spData.toWeakRef());
  mFlagForCompil = true;
}

//------------------------------------------------------------------------------
void LCMemoryReadSet::update()
{
  if(mFlagForCompil)
  {
    compilDataMap();
  }

  auto it = mListCompil.begin();

  while(it != mListCompil.end())
  {
    if((*it).isNull())
    {
      it = mListCompil.erase(it);
      continue;
    }

    if(!(*it).updateData(mfReader, mDataDimention))
    {
      mFlagForCompil = true;
    }
    it++;
  }
};


