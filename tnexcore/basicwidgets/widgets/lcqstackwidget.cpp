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

#include "lcqstackwidget.h"
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
  QMap<QByteArray, int> mValueIndex;
  CLocalData(){}
};

#define ld  (*(reinterpret_cast<CLocalData*>(mpLocal)))

//==============================================================================
LCQStackWidget::LCQStackWidget(
    QSharedPointer<LIRemoteDataSource> _source,
    QString _data,
    QWidget* _parent) :
  QStackedWidget(_parent)
{
  mpLocal = new CLocalData;
  /* QStackedWidget::addWidget(new QWidget); */

  auto read_handler =
      [this](QSharedPointer<QByteArray> _data, 
          LIRemoteDataReader::EReadStatus _dataStatus)
      {
        if(_dataStatus != LIRemoteDataReader::EReadStatus::Valid) return;
        auto it = ld.mValueIndex.find(*_data);
        if(it != ld.mValueIndex.end())
        {
          setCurrentIndex(it.value());
        }
        /* else */
        /* { */
        /*   setCurrentIndex(0); */
        /*   qDebug() << "LCQStackWidget undef widget"; */
        /* } */
      };

  ld.mDataReader = _source->createReader(_data);

  if(ld.mDataReader.isNull()) return;

  ld.mDataReader->setHandler(read_handler);
  ld.mDataReader->connectToSource();
}

LCQStackWidget::~LCQStackWidget()
{
  delete &ld;
}

//------------------------------------------------------------------------------
void LCQStackWidget::addWidget(QWidget* _widget, const QByteArray& _matching)
{
  ld.mValueIndex.insert(_matching, QStackedWidget::addWidget(_widget));
}


