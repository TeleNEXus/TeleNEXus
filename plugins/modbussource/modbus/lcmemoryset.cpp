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
template < typename T> typename QLinkedList<T>::Iterator FindGreaterItem(
    QLinkedList<T>& _list, 
    const T& _item)
{
  auto it = _list.begin();
  while(it != _list.end())
  {
    if(*it > _item) 
    {
      return it;
    }
    it++;
  };
  return _list.end();
}

//==============================================================================CMemoryDataItem
CMemoryDataItem::CMemoryDataItem(qint32 _addr, qint32 _size):
  mAddr(_addr), mSize(_size)
{
  if(_addr < 0) mAddr = 0;
  else mAddr = _addr;
  if(_size < 1) mSize = 1;
  else mSize = _size;
}

//------------------------------------------------------------------------------
bool operator<(const CMemoryDataItem& _first, const CMemoryDataItem& _second)
{
  if(_first.getAddressBegin() != _second.getAddressBegin())
    return (_first.getAddressBegin() < _second.getAddressBegin());
  else
    return (_first.getAddressEnd() < _second.getAddressEnd());
}

//------------------------------------------------------------------------------
bool operator>(const CMemoryDataItem& _first, const CMemoryDataItem& _second)
{
  if(_first.getAddressBegin() != _second.getAddressBegin())
    return (_first.getAddressBegin() > _second.getAddressBegin());
  else
    return (_first.getAddressEnd() > _second.getAddressEnd());
}

//------------------------------------------------------------------------------
bool operator==(const CMemoryDataItem& _first, const CMemoryDataItem& _second)
{
  if((_first.getAddressBegin() == _second.getAddressBegin()) ||
      (_first.getAddressEnd() == _second.getAddressEnd()))
    return false;
  else
    return true;
}

QDebug operator<<(QDebug _debug, const CMemoryDataItem& _item)
{
  _debug << qPrintable(QString("DataItem(%1, %2, %3)")
    .arg(_item.getAddressBegin())
    .arg(_item.getAddressEnd())
    .arg(_item.getSize()));
  return _debug;
}

//==============================================================================CMemorySetItem
CMemorySetItem CMemorySetItem::unite(
    const CMemorySetItem& _i1, 
    const CMemorySetItem& _i2)
{
  CMemorySetItem ji;

  if(_i1.isNull() || _i2.isNull()) return ji;

  if(_i1.first < _i2.first)
  {
    ji.first = _i1.first; 
  }
  else
  {
    ji.first = _i2.first;
  }

  if(_i1.second > _i2.second)
  {
    ji.second = _i1.second;
  }
  else
  {
    ji.second = _i2.second;
  }
  
  ji.mDataItems = _i1.mDataItems + _i2.mDataItems;  
  ji.mData = _i1.mData + _i2.mData;

  return ji;
}

//------------------------------------------------------------------------------
QDebug operator<<(QDebug _debug, const CMemorySetItem& _item)
{
  _debug << qPrintable(QString("Item(%1, %2, %3)")
    .arg(_item.first)
    .arg(_item.second)
    .arg(_item.getData()));
  return _debug;
}

//------------------------------------------------------------------------------
QDebug operator<<(QDebug _debug, const QLinkedList<CMemorySetItem>& _list)
{
  _debug << "MemorySetList(\n";

  for(auto it = _list.begin(); it != _list.end()-1; it++)
  {
    _debug << *it << ",\n";
  } 
  if(!_list.isEmpty()) _debug << _list.last();
  _debug.nospace() << "\n)";
  return _debug;
}

//==============================================================================LCMemorySet
LCMemorySet::LCMemorySet(qint32 _fragmentMaxSize) :
  mFragmentMaxSize(_fragmentMaxSize)
{

  addSetItem(CMemorySetItem(20, 21, 1));
  addSetItem(CMemorySetItem(5, 6, 1));
  addSetItem(CMemorySetItem(7, 8, 1));

  addSetItem(CMemorySetItem(21, 26, 1));
  addSetItem(CMemorySetItem(5, 6, 1));
  addSetItem(CMemorySetItem(7, 8, 1));
  addSetItem(CMemorySetItem(5, 6, 1));
  addSetItem(CMemorySetItem(7, 8, 1));

  addSetItem(CMemorySetItem(22, 24, 1));
  addSetItem(CMemorySetItem(21, 24, 1));
  addSetItem(CMemorySetItem(21, 26, 1));
  addSetItem(CMemorySetItem(22, 35, 1));

  addSetItem(CMemorySetItem(30, 30, 1));
  addSetItem(CMemorySetItem(31, 31, 1));
  addSetItem(CMemorySetItem(32, 32, 1));
  addSetItem(CMemorySetItem(33, 33, 1));
  addSetItem(CMemorySetItem(34, 34, 1));
  addSetItem(CMemorySetItem(35, 35, 1));
  addSetItem(CMemorySetItem(36, 36, 1));
  addSetItem(CMemorySetItem(37, 37, 1));
  addSetItem(CMemorySetItem(38, 38, 1));
  addSetItem(CMemorySetItem(39, 39, 1));
  addSetItem(CMemorySetItem(40, 40, 1));
  addSetItem(CMemorySetItem(41, 41, 1));
  addSetItem(CMemorySetItem(42, 42, 1));
  addSetItem(CMemorySetItem(43, 43, 1));
  addSetItem(CMemorySetItem(44, 44, 1));
  addSetItem(CMemorySetItem(45, 45, 1));
  addSetItem(CMemorySetItem(46, 46, 1));

  addSetItem(CMemorySetItem(22, 22, 1));
  addSetItem(CMemorySetItem(23, 23, 1));
  addSetItem(CMemorySetItem(24, 24, 1));

  qDebug() << "========================";
  qDebug() << "Order list";
  qDebug() << "========================";
  qDebug() << mListOrder;

  QLinkedList<CMemoryDataItem> list_data;


  qDebug() << "========================";
  qDebug() << "Compil order list";
  qDebug() << "========================";
  compilOrderList();
  qDebug() << mListCompil;

}

//------------------------------------------------------------------------------
LCMemorySet::~LCMemorySet()
{
}

//------------------------------------------------------------------------------
void LCMemorySet::addSetItem(const CMemorySetItem& _item)
{
  auto it = FindGreaterItem<CMemorySetItem>(mListOrder, _item);
  mListOrder.insert(it, _item);
}

//------------------------------------------------------------------------------
void LCMemorySet::compilOrderList()
{
  enum class EState
  {
    st0,
    st1
  };

  EState state = EState::st0;
  CMemorySetList::Iterator oit = mListOrder.begin();
  CMemorySetItem item_acc;

  auto pass = 
    [&]()
    {
      switch(state)
      {
      case EState::st0:
        if(oit == mListOrder.end())
        {
          return false;
        }
        item_acc = (*oit);

        state = EState::st1;
        break;

      case EState::st1:
        {
          oit++;
          CMemorySetItem next_item;

          if(oit != mListOrder.end())
          {
            next_item = *oit;
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
            auto ibuff = CMemorySetItem::unite(item_acc, next_item);
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
}


//------------------------------------------------------------------------------
LCMemorySet::CMemorySetList::Iterator 
LCMemorySet::addItem(const CMemorySetItem& _item)
{
  Q_UNUSED(_item);
  return mListOrder.begin();
}

//------------------------------------------------------------------------------
void LCMemorySet::addDataItem(QSharedPointer<CMemoryDataItem> _sp_data_item)
{
  if(mDataItemMap.find(_sp_data_item) != mDataItemMap.end()) return;
  mDataItemMap.insert(_sp_data_item, addItem(CMemorySetItem()));
}
