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

#include "lcqlistwidget.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIDataFormatter.h"
#include <QMap>
#include <QByteArray>
#include <QDebug>


//==============================================================================
class CLocalData
{
public:
  QSharedPointer<LIRemoteDataReader> mDataReader;
  QSharedPointer<LIRemoteDataWriter> mDataWriter;
  QMap<int, QByteArray> mRowValue;
  QMap<QByteArray, int> mValueRow;
  CLocalData(){}
};

#define ld  (*(reinterpret_cast<CLocalData*>(mpLocal)))

//==============================================================================
LCQListWidget::LCQListWidget(
    QSharedPointer<LIRemoteDataSource> _source,
    QString _data,
    QWidget* _parent) :
  QListWidget(_parent)
{
  mpLocal = new CLocalData;
  ld.mDataWriter = _source->createWriter(_data);

  if(!(ld.mDataWriter.isNull()))
  {
    connect(this, &LCQListWidget::currentRowChanged, 
        [this](int _row)
        {
          auto it = ld.mRowValue.find(_row);
          if(it == ld.mRowValue.end()) return;
          ld.mDataWriter->writeRequest(it.value());
        });
  }

  ld.mDataReader = _source->createReader(_data);

  if(ld.mDataReader.isNull()) return; 

  ld.mDataReader->setHandler(
      [this](QSharedPointer<QByteArray> _data, 
        LIRemoteDataReader::EReadStatus _dataStatus)
      {
        if(_dataStatus != LIRemoteDataReader::EReadStatus::Valid) return;

        auto it = ld.mValueRow.find(*(_data.data()));
        if(it == ld.mValueRow.end())
        {
          auto i = ld.mRowValue.find(currentRow());
          if(i == ld.mRowValue.end()) return;
          ld.mDataWriter->writeRequest(i.value());
        }
        setCurrentRow(it.value());
      });
  ld.mDataReader->connectToSource();
}

LCQListWidget::~LCQListWidget()
{
  delete &ld;
}

//------------------------------------------------------------------------------
void LCQListWidget::addItem(QListWidgetItem* _item, const QByteArray& _matching) 
{
  QListWidget::addItem(_item);
  int r = row(_item);
  ld.mRowValue.insert(r, _matching);
  ld.mValueRow.insert(_matching, r);
}


