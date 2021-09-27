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
    const CMemorySetItem& _i2,
    qint32 _uniteMaxSize)
{
  CMemorySetItem ji;

  if(_i1.isNull() || _i2.isNull()) return ji;

  /* if(!SegmentIsCrossed<qint32>( */
  /*       _i1.first, _i1.second, _i2.first, _i2.second)) */
  /*   return ji; */

  /* if(_i1 < _i2) */
  /* { */
  /*   ji.first = _i1.first; */
  /*   ji.second = _i2.second; */
  /* } */
  /* else if(_i1 > _i2) */
  /* { */
  /*   ji.first = _i2.first; */
  /*   ji.second = _i1.second; */
  /* } */
  /* else */
  /* { */
  /*   qDebug() << "Unite 0"; */ 
  /*   ji.first = _i1.first; */
  /*   ji.second = _i1.second; */
  /* } */


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

  if(_uniteMaxSize > 0)
  {
    if((ji.second - ji.first) > _uniteMaxSize)
      return CMemorySetItem();
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

  addSetItem(CMemorySetItem(21, 28, 1));
  addSetItem(CMemorySetItem(5, 6, 1));
  addSetItem(CMemorySetItem(7, 8, 1));
  addSetItem(CMemorySetItem(5, 6, 1));
  addSetItem(CMemorySetItem(7, 8, 1));
  /* addSetItem(CMemorySetItem(10, 14, 1)); */
  /* addSetItem(CMemorySetItem(5, 8, 1)); */
  /* addSetItem(CMemorySetItem(8, 10, 1)); */

  addSetItem(CMemorySetItem(22, 24, 1));
  addSetItem(CMemorySetItem(21, 24, 1));
  addSetItem(CMemorySetItem(21, 28, 1));




  /* addSetItem(CMemorySetItem(26, 30, 1)); */
  /* addSetItem(CMemorySetItem(20, 25, 1)); */

  /* addSetItem(CMemorySetItem(23, 24, 1)); */
  /* addSetItem(CMemorySetItem(20, 22, 1)); */


  /* addSetItem(CMemorySetItem(1, 1, 1)); */
  /* addSetItem(CMemorySetItem(2, 2, 1)); */
  /* addSetItem(CMemorySetItem(3, 3, 1)); */
  /* addSetItem(CMemorySetItem(4, 4, 1)); */
  /* addSetItem(CMemorySetItem(5, 5, 1)); */
  /* addSetItem(CMemorySetItem(4, 4, 1)); */
  /* addSetItem(CMemorySetItem(5, 5, 1)); */
  /* addSetItem(CMemorySetItem(1, 1, 1)); */
  /* addSetItem(CMemorySetItem(2, 2, 1)); */
  /* addSetItem(CMemorySetItem(3, 5, 1)); */
  /* addSetItem(CMemorySetItem(3, 3, 1)); */
  /* addSetItem(CMemorySetItem(5, 5, 1)); */

  /* addSetItem(CMemorySetItem(6, 8, 1)); */
  /* addSetItem(CMemorySetItem(3, 3, 1)); */
  /* addSetItem(CMemorySetItem(2, 2, 1)); */

  /* addSetItem(CMemorySetItem(5, 5, 1)); */
  /* addSetItem(CMemorySetItem(6, 8, 1)); */
  /* addSetItem(CMemorySetItem(8, 8, 1)); */
  /* addSetItem(CMemorySetItem(9, 10, 1)); */
  /* addSetItem(CMemorySetItem(11, 13, 1)); */
  /* addSetItem(CMemorySetItem(14, 14, 1)); */

  /* addSetItem(CMemorySetItem(3, 3, 1)); */
  /* addSetItem(CMemorySetItem(2, 2, 1)); */

  /* addSetItem(CMemorySetItem(20, 25, 1)); */

  qDebug() << "=====================Result";
  qDebug() << mListOrder;

  QLinkedList<CMemoryDataItem> list_data;


  qDebug() << "========================";
  qDebug() << "Compil order list";
  qDebug() << "========================";
  compilOrderList();

}

//------------------------------------------------------------------------------
LCMemorySet::~LCMemorySet()
{
}

//------------------------------------------------------------------------------
void LCMemorySet::addSetItem(const CMemorySetItem& _item)
{
  /* auto insert = */ 
  /*   [this]( */
  /*       const CMemorySetItem& _item, */ 
  /*       const CMemorySetList::Iterator& _insert_before) */
  /*   { */

  /*     auto it = mListOrder.insert(_insert_before, _item); */

  /*     auto unite = */
  /*       [this](CMemorySetList::Iterator& _first) */
  /*       { */
  /*         auto second = _first + 1; */
  /*         if(second == mListOrder.end()) */ 
  /*         { */
  /*           qDebug() << "insert x"; */
  /*           return _first; */
  /*         } */

  /*         if( !SegmentIsCrossed<qint32>( */
  /*               (*_first).first, (*_first).second, */
  /*               (*second).first, (*second).second)) */
  /*         { */
  /*           if(!SegmentIsNeighbour<qint32>( */
  /*               (*_first).first, (*_first).second, */
  /*               (*second).first, (*second).second)) */
  /*           { */
  /*             qDebug() << "insert 0"; */
  /*             return _first; */
  /*           } */
  /*         } */

  /*         CMemorySetItem item = */ 
  /*           CMemorySetItem::unite(*_first, *second, mFragmentMaxSize); */

  /*         if(item.isNull()) */
  /*         { */
  /*           qDebug() << "insert 1"; */
  /*           return _first; */
  /*         } */
          
  /*         qDebug() << "-------erase" << " first " << *_first << " second " << *second; */
  /*         qDebug() << "-------new item " << item; */
  /*         auto it = mListOrder.erase(mListOrder.erase(_first)); */
  /*         return mListOrder.insert(it, item); */
  /*       }; */

  /*     it--; */

  /*     if( it != mListOrder.end()) */
  /*     { */
  /*       it = unite(it); */
  /*           qDebug() << "insert 2"; */
  /*     } */

  /*     if( it != mListOrder.end()) */
  /*     { */
  /*       it = unite(it); */
  /*           qDebug() << "insert 3"; */
  /*     } */
  /*   }; */






  /* qDebug() << "--------------------Add item" << _item; */
  auto it = FindGreaterItem<CMemorySetItem>(mListOrder, _item);
  /* auto ret = */ 
  /*   [this]() */
  /*   { */
  /*     qDebug() << "list = "; */
  /*     qDebug() << mListOrder; */
  /*   }; */

  mListOrder.insert(it, _item);
  /* insert(_item, it); */

  /* return ret(); */
  
}

//------------------------------------------------------------------------------
void LCMemorySet::compilOrderList()
{

  if(mListOrder.size() == 0) return;

  enum EState 
  {
    ST_1,
    ST_2
  }state;

  CMemorySetList::Iterator oit = mListOrder.begin();
  CMemorySetItem item_acc = *oit;
  CMemorySetList::Iterator cit;

  auto operate =
    [&]()
    {

      auto next_order = oit + 1;


      if(next_order == mListOrder.end())
      {
        return true;
      }



      if((SegmentIsCrossed<qint32>(
              item_acc.first, item_acc.second,
              (*next_order).first, (*next_order).second)) || 
          SegmentIsNeighbour<qint32>(
            item_acc.first, item_acc.second,
            (*next_order).first, (*next_order).second))
      {
        auto item = CMemorySetItem::unite(item_acc, (*next_order));
        if(!item.isNull())
        {
          item_acc = item;
        }
        else
        {
          mListCompil << item_acc;
        }
      }




      bool ret = false;
      switch(state)
      {
      case EState::ST_1:
        oit++;
        if(oit == mListOrder.end()) 
        {
          ret = true;
        }
        break;

      case EState::ST_2:
        break;

      default:
        break;
      }
      return ret;
    };

  while(true)
  {
    if(operate()) break;
  }

  /* for(auto it = mListOrder.begin(); it != mListOrder.end(); it++) */
  /* { */
  /*   /1* qDebug() << "List Order item = " << *it; *1/ */
  /* } */



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
