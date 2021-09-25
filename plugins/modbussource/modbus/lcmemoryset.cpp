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

  add(CMemorySetItem(26, 30, 1));
  add(CMemorySetItem(20, 25, 1));

  add(CMemorySetItem(23, 24, 1));
  add(CMemorySetItem(20, 22, 1));


    add(CMemorySetItem(1, 1, 1));
    add(CMemorySetItem(2, 2, 1));
    add(CMemorySetItem(3, 3, 1));
    add(CMemorySetItem(4, 4, 1));
    add(CMemorySetItem(5, 5, 1));
    add(CMemorySetItem(4, 4, 1));
    add(CMemorySetItem(5, 5, 1));
    add(CMemorySetItem(1, 1, 1));
    add(CMemorySetItem(2, 2, 1));
    add(CMemorySetItem(3, 5, 1));
    add(CMemorySetItem(3, 3, 1));
    add(CMemorySetItem(5, 5, 1));

  add(CMemorySetItem(6, 8, 1));
  add(CMemorySetItem(3, 3, 1));
  add(CMemorySetItem(2, 2, 1));

  add(CMemorySetItem(5, 5, 1));
  add(CMemorySetItem(6, 8, 1));
  add(CMemorySetItem(8, 8, 1));
  add(CMemorySetItem(9, 10, 1));
  add(CMemorySetItem(11, 13, 1));
  add(CMemorySetItem(14, 14, 1));


  add(CMemorySetItem(3, 3, 1));
  add(CMemorySetItem(2, 2, 1));

  add(CMemorySetItem(20, 25, 1));

  qDebug() << "=====================Result";
  qDebug() << mList;
}

//------------------------------------------------------------------------------
LCMemorySet::~LCMemorySet()
{
}

//------------------------------------------------------------------------------
LCMemorySet::CMemoryListIterator 

LCMemorySet::findGreater(const CMemorySetItem& _item)
{
  auto it = mList.begin();
  while(it != mList.end())
  {
    if(*it > _item) 
    {
      return it;
    }
    it++;
  };
  return mList.end();
}

//------------------------------------------------------------------------------
void LCMemorySet::add(const CMemorySetItem& _item)
{

  auto cross_top = 
    [this](const CMemorySetItem& _item, const CMemoryListIterator& _it)
    {
      auto it_before = _it-1;

      if(it_before == mList.end()) 
      {
        return false;
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
            return false;
          }
        }

        CMemorySetItem new_item(
            first, 
            second,
            (*it_before).getData() + _item.getData());
        auto it = mList.erase(it_before);
        mList.insert(it, new_item);
        return true;
      }

      return false;
    };

  auto cross_bottom = 
    [this](const CMemorySetItem& _item, const CMemoryListIterator& _it)
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


  auto it = findGreater(_item);

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

