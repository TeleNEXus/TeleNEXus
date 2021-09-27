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

#ifndef LCMEMORYSET_H_
#define LCMEMORYSET_H_

#include <QLinkedList>
#include <QDebug>
#include <QPair>
#include <QSharedPointer>
#include <QMap>
#include <QSet>

//==============================================================================
class CMemoryDataItem
{
private:
  qint32 mAddr;
  qint32 mSize;
public:
  CMemoryDataItem() = delete;
  CMemoryDataItem(qint32 _addr, qint32 _size);
  qint32 getAddressBegin(void)const{ return mAddr;}
  qint32 getAddressEnd(void)const{ return (mAddr + mSize - 1);}
  qint32 getSize(void)const{ return mSize;}
};

bool operator<(const CMemoryDataItem& _first, const CMemoryDataItem& _second);
bool operator>(const CMemoryDataItem& _first, const CMemoryDataItem& _second);
bool operator==(const CMemoryDataItem& _first, const CMemoryDataItem& _second);

QDebug operator<<(QDebug _debug, const CMemoryDataItem& _item);

//==============================================================================
class CMemorySetItem : public QPair<qint32, qint32>
{
private:
  using CPair = QPair<qint32, qint32>;
  qint32 mData;
  QSet<QSharedPointer<CMemoryDataItem>> mDataItems;
public:

  CMemorySetItem() : CPair(-1, -1), mData(0){}

  explicit CMemorySetItem(
      quint32 _first, qint32 _second, qint32 _data = 0) :
    CPair(_first, _second), mData(_data){}

  CMemorySetItem(QSharedPointer<CMemoryDataItem> _spDataItem)
  {
    mDataItems.insert(_spDataItem);
    first = _spDataItem->getAddressBegin();
    second = _spDataItem->getAddressEnd();
  }

  bool isNull() const
  { 
    return ((first < 0) || (second < 0)) ? (true) : (false);
  }

  qint32 getData(void)const{return mData;}

  static CMemorySetItem unite(
      const CMemorySetItem& _i1, 
      const CMemorySetItem& _i2,
      qint32 _uniteMaxSize = 0);
  
  /* CMemorySetItem& operator+=(const CMemorySetItem& _item) */
  /* { */
  /*   if(second + 1 < _item.first) return *this; */

  /*   second = _item.second; */
  /*   mData += _item.mData; */
  /*   mDataItems += _item.mDataItems; */

  /*   return *this; */
  /* } */
};

QDebug operator<<(QDebug _debug, const CMemorySetItem& _item);
QDebug operator<<(QDebug _debug, const QLinkedList<CMemorySetItem>& _list);

//==============================================================================
class LCMemorySet
{
private:
  using CMemorySetList = QLinkedList<CMemorySetItem>;

private:
  qint32 mFragmentMaxSize;
  CMemorySetList mListOrder;
  CMemorySetList mListCompil;

  QMap<QSharedPointer<CMemoryDataItem>, CMemorySetList::iterator> mDataItemMap;

public:
  explicit LCMemorySet(qint32 _fragmentMaxSize = 0);
  ~LCMemorySet();

  void addDataItem(const CMemoryDataItem& _dataItem);

private:
  void addSetItem(const CMemorySetItem& _item);
  void compilOrderList();

  CMemorySetList::Iterator addItem(const CMemorySetItem& _item);

public:
  void addDataItem(QSharedPointer<CMemoryDataItem> _item);
};


#endif  //LCMEMORYSET_H_
