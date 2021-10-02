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

//==============================================================================LCMemoryReadSet
class LCMemoryReadSet
{
public:
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

private:
  using CAddrPair = QPair<int,int>;
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
    bool updateData(FReader& _reader);
    static CDataItem unite( const CDataItem& _i1, const CDataItem& _i2);
  };

private:
  using CMemorySetList = QLinkedList<CDataItem>;
  using CMemorySetMap  = QMultiMap<CDataItem, int>;
  using CMemoryDataMap = QMultiMap<QPair<int,int>, QWeakPointer<CIData>>;

private:
  CMemorySetList mListCompil;
  CMemoryDataMap mDataMap;

  FReader mfReader;
  bool mFlagForCompil;

public:
  LCMemoryReadSet() = delete;
  LCMemoryReadSet(FReader _reader);
  ~LCMemoryReadSet();

private:
  void compilDataMap();

public:
  void insert(QWeakPointer<CIData> _sp_data);
  void remove(QWeakPointer<CIData> _sp_data);
  void update();
};

#endif  //LCMEMORYSET_H_
