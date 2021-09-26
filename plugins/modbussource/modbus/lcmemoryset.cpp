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
template <class T> bool CrossSegment(T a1, T b1, T a2, T b2)
{
  if((a1 >= a2) && (a1 <= b2)) return true;
  if((b1 >= a2) && (b1 <= b2)) return true;
  if((a2 >= a1) && (a2 <= b1)) return true;
  if((b2 >= a1) && (b2 <= b1)) return true;
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
QDebug operator<<(QDebug _debug, const CMemorySetItem& _item)
{
  _debug << qPrintable(QString("Item(%1, %2, %3)")
    .arg(_item.first)
    .arg(_item.second)
    .arg(_item.getData()));
  return _debug;
}

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

  addSetItem(CMemorySetItem(10, 14, 1));
  addSetItem(CMemorySetItem(5, 8, 1));
  addSetItem(CMemorySetItem(8, 10, 1));

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
  qDebug() << mList;

  QLinkedList<CMemoryDataItem> list_data;

  auto add_data_item = 
    [&list_data](const CMemoryDataItem& _item)
    {
      auto it = FindGreaterItem<CMemoryDataItem>(list_data, _item);
      list_data.insert(it, _item);
    };

  add_data_item(CMemoryDataItem(0, 1));
  add_data_item(CMemoryDataItem(1, 2));
  add_data_item(CMemoryDataItem(5, 1));
  add_data_item(CMemoryDataItem(2, 3));
  add_data_item(CMemoryDataItem(3, 1));
  add_data_item(CMemoryDataItem(4, 1));
  add_data_item(CMemoryDataItem(4, 4));
  add_data_item(CMemoryDataItem(4, 1));

  qDebug() << "========================";
  qDebug() << "List of data items";
  qDebug() << "========================";
  for(auto it = list_data.constBegin(); it != list_data.constEnd(); it++)
  {
    qDebug() << *it;
  }

}

//------------------------------------------------------------------------------
LCMemorySet::~LCMemorySet()
{
}

//------------------------------------------------------------------------------
void LCMemorySet::addSetItem(const CMemorySetItem& _item)
{

  auto cross_top = 
    [this](const CMemorySetItem& _item, const CMemorySetList::Iterator& _it)
    {
      auto it_before = _it-1;

      if(it_before == mList.end()) 
      {
        return false;
      }

      if(!((_item.first <= (*it_before).second) || (_item.first-1 == (*it_before).second)))
        return false;

      auto first = (*it_before).first;
      auto second = 
        ((*it_before).second > _item.second) ? ((*it_before).second) : (_item.second);

      if(mFragmentMaxSize > 0)
      {
        if(mFragmentMaxSize < (second - first)) 
        {
          return false;
        }
      }

      CMemorySetItem new_item(
          first, 
          second,
          (*it_before).getData() + _item.getData());
      auto it = mList.erase(it_before);


      it = mList.insert(it, new_item);

      auto it_after = it-1;

      if(it_after == mList.end()) return true;
      




      return false;
    };

  auto cross_bottom = 
    [this](const CMemorySetItem& _item, const CMemorySetList::Iterator& _it)
    {
      if(_it == mList.end()) return false;

      if((_item.second >= (*_it).first) || (_item.second+1 == (*_it).first))
      {
        auto first = _item.first;
        auto second = (_item.second > (*_it).second) ? (_item.second) : ((*_it).second);

        if(mFragmentMaxSize > 0)
        {
          if((second - first) > mFragmentMaxSize) return false;
        }

        CMemorySetItem new_item(
            first,
            second,
            (*_it).getData() + _item.getData());
        auto it = mList.erase(_it);
        mList.insert(it, new_item);
        return true;
      }
      return false;
    };

  auto it = FindGreaterItem<CMemorySetItem>(mList, _item);

  qDebug() << "--------------------Add item" << _item;

  auto ret = 
    [this]()
    {
      qDebug() << "list = ";
      qDebug() << mList;
    };

  if(cross_top(_item, it)) return ret();
  if(cross_bottom(_item, it)) return ret();
  mList.insert(it, _item);
  return ret();
}

//------------------------------------------------------------------------------
LCMemorySet::CMemorySetList::Iterator 
LCMemorySet::addItem(const CMemorySetItem& _item)
{
//+++++++++++++++++++++++++++++++++++++++++++++++



  auto cross_top = 
    [this](const CMemorySetItem& _item, const CMemorySetList::Iterator& _it)
    {
      auto it_before = _it-1;

      if(it_before == mList.end()) 
      {
        return mList.insert(it_before, _item);
      }

      if((_item.first <= (*it_before).second) || (_item.first-1 == (*it_before).second))
      {

        auto first = (*it_before).first;
        auto second = 
          ((*it_before).second > _item.second) ? ((*it_before).second) : (_item.second);

        if(mFragmentMaxSize > 0)
        {
          if(mFragmentMaxSize < (second - first)) 
          {
            return mList.insert(it_before, _item);
          }
        }


        CMemorySetItem new_item(
            first, 
            second,
            (*it_before).getData() + _item.getData());




        auto it = mList.erase(it_before);


        mList.insert(it, new_item);
        return it_before;
      }

      return it_before;
    };



  auto cross_bottom = 
    [this](const CMemorySetItem& _item, const CMemorySetList::Iterator& _it)
    {
      if(_it == mList.end()) return false;

      if((_item.second >= (*_it).first) || (_item.second+1 == (*_it).first))
      {
        auto first = _item.first;
        auto second = (_item.second > (*_it).second) ? (_item.second) : ((*_it).second);

        if(mFragmentMaxSize > 0)
        {
          if((second - first) > mFragmentMaxSize) return false;
        }

        CMemorySetItem new_item(
            first,
            second,
            (*_it).getData() + _item.getData());
        auto it = mList.erase(_it);
        mList.insert(it, new_item);
        return true;
      }
      return false;
    };



  auto ret = 
    [this](const CMemorySetList::Iterator& _it_before, const CMemorySetItem& _item)
    {
      return mList.insert(_it_before, _item);
    };


  auto it_greater = FindGreaterItem<CMemorySetItem>(mList, _item);

  if(it_greater == mList.end()) return ret(it_greater, _item);


  qDebug() << "--------------------Add item" << _item;


  /* auto ret = */ 
  /*   [this]() */
  /*   { */
  /*     qDebug() << "list = "; */
  /*     qDebug() << mList; */
  /*   }; */

  /* if(cross_top(_item, it)) return ret(); */
  /* if(cross_bottom(_item, it)) return ret(); */
  /* mList.insert(it, _item); */
  /* return ret(); */







//+++++++++++++++++++++++++++++++++++++++++++++++


  return mList.insert(mList.end(), _item); 
}

//------------------------------------------------------------------------------
void LCMemorySet::addDataItem(QSharedPointer<CMemoryDataItem> _sp_data_item)
{
  if(mDataItemMap.find(_sp_data_item) != mDataItemMap.end()) return;
  mDataItemMap.insert(_sp_data_item, addItem(CMemorySetItem()));
}
