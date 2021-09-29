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
using CDataItem = LCMemorySetAccess::CDataItem;

//------------------------------------------------------------------------------
void CDataItem::updateData(FReader& _reader)
{
  int size = second - first;
  QByteArray data(size, 0);

  int read_status = _reader(first, data);

  auto it = mDataList.begin();

  while(it != mDataList.end())
  {
    QSharedPointer<CIData> sp = (*it).lock();
    if(sp.isNull())
    {
      it = mDataList.erase(it);
    }
    else
    {
      qDebug() << "setData";
      sp->setData(data.mid(sp->getAddress(), sp->getSize()), read_status);
      it++;
    }
  }

  if(mDataList.isEmpty())
  {
    *this = CDataItem();
  }
}

//------------------------------------------------------------------------------
CDataItem LCMemorySetAccess::CDataItem::unite(
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

//------------------------------------------------------------------------------
QDebug operator<<(QDebug _debug, const CDataItem& _item)
{
  _debug << qPrintable(QString("\nCDataItem(%1, %2, %3)")
    .arg(_item.first)
    .arg(_item.second)
    .arg(_item.getUniteIndex()));

  for(auto it = _item.mDataList.begin(); it != _item.mDataList.end(); it++)
  {
    auto sp = (*it).lock();
    if(sp.isNull()) continue;
    _debug << "\n" << *sp;
  }

  return _debug;
}

//------------------------------------------------------------------------------
QDebug operator<<(QDebug _debug, const QLinkedList<CDataItem>& _list)
{
  _debug << "MemorySetList(";

  for(auto it = _list.begin(); it != _list.end()-1; it++)
  {
    _debug << *it << ",\n";
  } 
  if(!_list.isEmpty()) _debug << _list.last();
  _debug.nospace() << ")";
  return _debug;
}

//------------------------------------------------------------------------------
QDebug operator<<(QDebug _debug, const QMultiMap<QPair<int, int>, 
    QSharedPointer<LCMemorySetAccess::CIData>>& _map)
{
  _debug << "MemorySetDataMap(\n";

  auto out_element =
    [&_debug](QMultiMap<QPair<int, int>, QSharedPointer<LCMemorySetAccess::CIData>>::ConstIterator _it)
    {
      if(_it.value().isNull())
      {
        _debug << "(" << _it.key() << "," << "NULL" << ")\n";
      }
      else
      {
        _debug << "(" << _it.key() << "," << ( *_it.value() ) << ")\n";
      }
    };

  for(auto it = _map.begin(); it != _map.end()-1; it++)
  {
    out_element(it);
  } 

  if(_map.size() > 0) out_element(--_map.end());

  _debug.nospace() << ")";
  return _debug;
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

/* //------------------------------------------------------------------------------ */
/* void LCMemorySetAccess::addSetItem(const CDataItem& _item) */
/* { */
/*   mMapOrder.insert(_item, 1); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void LCMemorySetAccess::compilOrderList() */
/* { */
/*   enum class EState */
/*   { */
/*     st0, */
/*     st1 */
/*   }; */

/*   EState state = EState::st0; */
/*   CMemorySetMap::Iterator oit = mMapOrder.begin(); */
/*   CDataItem item_acc; */

/*   auto pass = */ 
/*     [&]() */
/*     { */
/*       switch(state) */
/*       { */
/*       case EState::st0: */
/*         if(oit == mMapOrder.end()) */
/*         { */
/*           return false; */
/*         } */
/*         item_acc = oit.key(); */
/* #pragma GCC diagnostic push */
/* #pragma GCC diagnostic ignored "-Wimplicit-fallthrough" */
/*         state = EState::st1; */
/*       case EState::st1: */
/* #pragma GCC diagnostic pop */
/*         { */
/*           oit++; */
/*           CDataItem next_item; */

/*           if(oit != mMapOrder.end()) */
/*           { */
/*             next_item = oit.key(); */
/*           } */
/*           else */
/*           { */
/*             mListCompil << item_acc; */
/*             return false; */
/*           } */

/*           if(SegmentIsCrossed<qint32>( */
/*                 item_acc.first, item_acc.second, */
/*                 next_item.first, next_item.second) || */ 
/*               SegmentIsNeighbour<qint32>( */
/*                 item_acc.first, item_acc.second, */
/*                 next_item.first, next_item.second)) */
/*           { */
/*             auto ibuff = CDataItem::unite(item_acc, next_item); */
/*             if(!ibuff.isNull()) */
/*             { */
/*               item_acc = ibuff; */
/*               break; */
/*             } */
/*           } */
          
/*           mListCompil << item_acc; */
/*           state = EState::st0; */
/*         } */
/*         break; */

/*       default: */
/*         return false; */
/*         break; */
/*       } */
/*       return true; */
/*     }; */

/*   while(pass()){} */
/* } */


//------------------------------------------------------------------------------
/* LCMemorySetAccess::CMemorySetMap::Iterator */ 
/* LCMemorySetAccess::addItem(const CDataItem& _item) */
/* { */
/*   Q_UNUSED(_item); */
/*   return mMapOrder.begin(); */
/* } */

//------------------------------------------------------------------------------
void LCMemorySetAccess::compilDataMap()
{
  qDebug() << "==============================";
  qDebug() << "Compil Data Map";
  qDebug() << "==============================";
  qDebug() << mDataMap;

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
        item_acc = CDataItem(oit.key().first, oit.key().second, oit.value());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
        state = EState::st1;
      case EState::st1:
#pragma GCC diagnostic pop
        {
          oit++;
          CDataItem next_item;

          if(oit != mDataMap.end())
          {
            next_item = CDataItem(oit.key().first, oit.key().second, oit.value());
          }
          else
          {
            mListCompil << item_acc;
            return false;
          }

          if(SegmentIsCrossed<qint32>(
                item_acc.first, item_acc.second,
                next_item.first, next_item.second) || 
              SegmentIsNeighbour<qint32>(
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


  qDebug() << "==============================";
  qDebug() << "List compil";
  qDebug() << "==============================";


  qDebug() << mListCompil;


  mFlagForCompil = false;
}

//------------------------------------------------------------------------------
using CIData = LCMemorySetAccess::CIData;

//------------------------------------------------------------------------------
void LCMemorySetAccess::insert(QSharedPointer<CIData> _sp_data)
{
  if(_sp_data->getSize() < 1) return;
  if(_sp_data->getAddress() < 0) return;

  CDataItem key(_sp_data->getAddress(), 
        _sp_data->getAddress() + _sp_data->getSize() - 1);

  if(mDataMap.contains(key, _sp_data)) return;

  mDataMap.insert(key, _sp_data);

  mFlagForCompil = true;

  qDebug() << "insert(" << key << "," << *_sp_data << ")";
}

//------------------------------------------------------------------------------
void LCMemorySetAccess::remove(
    QSharedPointer<CIData> _sp_data)
{
  if( mDataMap.remove(
        CDataItem(
          _sp_data->getAddress(), 
          _sp_data->getAddress() + _sp_data->getSize() - 1),
        _sp_data) > 0) 
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
      (*it).updateData(mfReader);
      it++;
    }
  }

};

//==============================================================================
QDebug operator<<(QDebug _debug, const LCMemorySetAccess::CIData& _data)
{
  _debug << qPrintable(QString("CIData(%1, %2)")
    .arg(_data.getAddress())
    .arg(_data.getSize()));
  return _debug;
}

