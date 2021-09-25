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
/* #include <QList> */
#include <QDebug>
#include <QPair>

//==============================================================================
class CMemorySetItem : public QPair<qint32, qint32>
{
private:
  using CPair = QPair<qint32, qint32>;
  qint32 mData;

public:
  CMemorySetItem() : CPair(-1, -1), mData(0){}

  explicit CMemorySetItem(
      quint32 _first, qint32 _second, qint32 _data = 0) :
    CPair(_first, _second), mData(_data){}

  bool isNull()
  { 
    return ((first < 0) || (second < 0)) ? (true) : (false);
  }

  qint32 getData(void)const{return mData;}
};

QDebug operator<<(QDebug _debug, const CMemorySetItem& _item);
QDebug operator<<(QDebug _debug, const QLinkedList<CMemorySetItem>& _list);

//==============================================================================
class LCMemorySet
{
private:
  using CMemoryList = QLinkedList<CMemorySetItem>;
  using CMemoryListIterator = CMemoryList::Iterator;
private:
  QLinkedList<CMemorySetItem> mList;

public:
  LCMemorySet();
  ~LCMemorySet();

private:
  CMemoryListIterator findGreater(const CMemorySetItem& _item);
  void add(const CMemorySetItem& _item);
};

#endif  //LCMEMORYSET_H_
