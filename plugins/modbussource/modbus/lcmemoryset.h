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
#include <QByteArray>
#include <functional>

//==============================================================================LCMemorySetAccess
class LCMemorySetAccess
{

public:

  using CAddrPair = QPair<int,int>;
  using FReader = std::function<int(int _addr, QByteArray& _data)>;

  //----------------------------------------------------------------------------CIData
  class CIData
  {
  public:
    CIData(){}
    virtual int getAddress() const = 0;
    virtual int getSize() const = 0; 
    virtual void setData(const QByteArray& _data, int status) = 0;
  };

  //----------------------------------------------------------------------------CDataItem
  class CDataItem : public QPair<int, int>
  {
  private:
    QList<QWeakPointer<CIData>> mDataList;
    int mUniteIndex;
  public:

    CDataItem() : CAddrPair(-1, -1), mUniteIndex(1){}

    CDataItem(int _first, int _second) :
      CAddrPair(_first, _second), mUniteIndex(1){}

    CDataItem(int _first, int _second, QWeakPointer<CIData> _wp_data) :
      CAddrPair(_first, _second), mUniteIndex(1){ mDataList << _wp_data;}

    bool isNull() const
    { 
      return ((first < 0) || (second < 0)) ? (true) : (false);
    }

    int getUniteIndex(void)const{return mUniteIndex;}
    void updateData(FReader& _reader);
    static CDataItem unite( const CDataItem& _i1, const CDataItem& _i2);
    friend QDebug operator<<(QDebug _debug, const CDataItem& _item); 
  };


private:
  using CMemorySetList = QLinkedList<CDataItem>;
  using CMemorySetMap  = QMultiMap<CDataItem, int>;
  using CMemoryDataMap = QMultiMap<QPair<int,int>, QSharedPointer<CIData>>;

private:
  /* CMemorySetMap mMapOrder; */
  CMemorySetList mListCompil;
  CMemoryDataMap mDataMap;

  FReader mfReader;
  bool mFlagForCompil;

public:
  LCMemorySetAccess() = delete;
  LCMemorySetAccess(FReader _reader);
  ~LCMemorySetAccess();

private:
  /* void addSetItem(const CDataItem& _item); */
  /* void compilOrderList(); */
  /* CMemorySetMap::Iterator addItem(const CDataItem& _item); */
  void compilDataMap();

public:
  void insert(QSharedPointer<CIData> _sp_data);
  void remove(QSharedPointer<CIData> _sp_data);
  void update();
};

QDebug operator<<(QDebug _debug, const LCMemorySetAccess::CDataItem& _item);
QDebug operator<<(QDebug _debug, const QLinkedList<LCMemorySetAccess::CDataItem>& _list);
QDebug operator<<(QDebug _debug, const LCMemorySetAccess::CIData& _data);

#endif  //LCMEMORYSET_H_
