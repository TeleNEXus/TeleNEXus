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

#include "lcmemoryset.h"
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
bool LCMemorySetAccess::CDataItem::updateData(FReader& _reader)
{
  QByteArray data(second - first + 1, 0);
  int read_status = _reader(first, data);
  auto it = mDataList.begin();
  while(it != mDataList.end())
  {
    QSharedPointer<CIData> sp = (*it).lock();
    //Return false if weak pointer not locked (for recompil data map).
    if(sp.isNull()) return false;
    int local_addr = sp->getAddress() - first;
    sp->setData(data.mid(local_addr, sp->getSize()), read_status);
    it++;
  }
  return true;
}

//------------------------------------------------------------------------------
LCMemorySetAccess::CDataItem LCMemorySetAccess::CDataItem::unite(
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

//==============================================================================LCMemorySetAccess
LCMemorySetAccess::LCMemorySetAccess(FReader _reader) :
  mfReader(_reader),
  mFlagForCompil(false)
{
}

//------------------------------------------------------------------------------
LCMemorySetAccess::~LCMemorySetAccess()
{
}

//------------------------------------------------------------------------------
void LCMemorySetAccess::compilDataMap()
{
  mListCompil.clear();

  enum class EState
  {
    st0,
    st1
  };

  EState state = EState::st0;
  CMemoryDataMap::Iterator oit = mDataMap.begin();

  CDataItem item_acc;

  auto pass = 
    [&]()
    {
      switch(state)
      {
      case EState::st0:
        if(oit == mDataMap.end())
        {
          return false;
        }

        if(oit.value().lock().isNull())
        {
          oit = mDataMap.erase(oit);
          break;
        }

        item_acc = CDataItem(oit.key().first, oit.key().second, oit.value());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
        state = EState::st1;
      case EState::st1:
#pragma GCC diagnostic pop
        {
          oit++;
          CDataItem next_item;

          if(oit == mDataMap.end())
          {
            mListCompil << item_acc;
            return false;
          }

          if(oit.value().lock().isNull())
          {
            oit = mDataMap.erase(oit);
            oit--;
            break;
          }

          next_item = CDataItem(oit.key().first, oit.key().second, oit.value());

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
          
          mListCompil << item_acc;
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
using CIData = LCMemorySetAccess::CIData;

//------------------------------------------------------------------------------
void LCMemorySetAccess::insert(QWeakPointer<CIData> _wp_data)
{
  auto sp = _wp_data.lock();
  if(sp.isNull())return;

  if(sp->getSize() < 1) return;
  if(sp->getAddress() < 0) return;

  CDataItem key(sp->getAddress(), 
        sp->getAddress() + sp->getSize() - 1);

  if(mDataMap.contains(key, sp)) return;

  mDataMap.insert(key, sp);

  mFlagForCompil = true;
}

//------------------------------------------------------------------------------
void LCMemorySetAccess::remove(QWeakPointer<CIData> _wp_data)
{

  auto sp = _wp_data.lock();
  if(sp.isNull()) return;

  if( mDataMap.remove(
        CDataItem(
          sp->getAddress(), 
          sp->getAddress() + sp->getSize() - 1),
        sp) > 0) 
  {
    mFlagForCompil = true;
  }
}

//------------------------------------------------------------------------------
void LCMemorySetAccess::update()
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
    }
    else
    {
      if(!(*it).updateData(mfReader))
      {
        mFlagForCompil = true;
      }
      it++;
    }
  }
};


