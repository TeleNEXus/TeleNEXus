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
#include <QMouseEvent>
#include <QKeyEvent>
#include <qnamespace.h>
#include <QTimer>
#include <QGuiApplication>


//==============================================================================
enum class EState
{
  released,
  pressed
};

//==============================================================================
class CLocalData
{
public:
  QSharedPointer<LIRemoteDataReader> mDataReader;
  QSharedPointer<LIDataFormatter> mDataFormatter;
  QMap<QString, int> mValueIndex;
  int indexUndef = -1;
  int indexWrong = -1;
  QTimer timer;
  EState state;
  CLocalData()
  {
    state = EState::released;
    timer.setSingleShot(true);
  }
};

#define ld  (*(reinterpret_cast<CLocalData*>(mpLocal)))

//==============================================================================
LCQStackWidget::LCQStackWidget(QWidget* _parent) : 
  LCQStackWidget(nullptr, QString(), nullptr, _parent)
{
}

//------------------------------------------------------------------------------
LCQStackWidget::LCQStackWidget(
    QSharedPointer<LIRemoteDataSource> _source,
    QString _data,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :
  QStackedWidget(_parent)
{
  mpLocal = new CLocalData;



  auto read_handler =
      [this](QSharedPointer<QByteArray> _data, 
          LIRemoteDataReader::EReadStatus _dataStatus)
      {

        auto set_index = 
          [this](int _index)
          {
            if(_index != currentIndex()) 
            {
              setCurrentIndex(_index);
            }
          };

        if(_dataStatus != LIRemoteDataReader::EReadStatus::Valid) 
        {
          set_index(ld.indexUndef);
          return;
        }

        auto it = ld.mValueIndex.find(ld.mDataFormatter->toString(*_data));

        if(it == ld.mValueIndex.end())
        {
          set_index(ld.indexWrong);
          return;
        }
        set_index(it.value());
      };

  if(_source.isNull()) return;
  ld.mDataReader = _source->createReader(_data);

  if(ld.mDataReader.isNull()) return;
  ld.mDataFormatter = _formatter;

  ld.mDataReader->setHandler(read_handler);
  this->installEventFilter(this);
}

LCQStackWidget::~LCQStackWidget()
{
  delete &ld;
}

//------------------------------------------------------------------------------
void LCQStackWidget::addWidget(QWidget* _widget, const QString& _matching)
{
  ld.mValueIndex.insert(_matching, QStackedWidget::addWidget(_widget));
  _widget->installEventFilter(this);
}

//------------------------------------------------------------------------------
void LCQStackWidget::addWidgetUndef(QWidget* _widget)
{
  int index = QStackedWidget::addWidget(_widget);
  ld.indexUndef = index;
  _widget->installEventFilter(this);
}

//------------------------------------------------------------------------------
void LCQStackWidget::addWidgetWrong(QWidget* _widget)
{
  int index = QStackedWidget::addWidget(_widget);
  ld.indexWrong= index;
  _widget->installEventFilter(this);
}

bool LCQStackWidget::eventFilter(QObject* _obj, QEvent* _event)
{
  Q_UNUSED(_obj);
  switch(_event->type())
  {
  case QEvent::Type::Show:
    ld.mDataReader->connectToSource();
    break;

  case QEvent::Type::Hide:
    ld.mDataReader->disconnectFromSource();
    break;

  default:
    break;
  }
  return false;
}

