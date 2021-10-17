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
  QMap<QByteArray, int> mValueIndex;
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
  LCQStackWidget(nullptr, QString(), _parent)
{
}

//------------------------------------------------------------------------------
LCQStackWidget::LCQStackWidget(
    QSharedPointer<LIRemoteDataSource> _source,
    QString _data,
    QWidget* _parent) :
  QStackedWidget(_parent)
{
  mpLocal = new CLocalData;

  auto read_handler =
      [this](QSharedPointer<QByteArray> _data, 
          LIRemoteDataReader::EReadStatus _dataStatus)
      {
        if(_dataStatus != LIRemoteDataReader::EReadStatus::Valid) 
        {
          setCurrentIndex(ld.indexUndef);
          return;
        }

        auto it = ld.mValueIndex.find(*_data);

        if(it == ld.mValueIndex.end())
        {
          setCurrentIndex(ld.indexWrong);
          return;
        }

        setCurrentIndex(it.value());
      };

  if(_source.isNull()) return;
  ld.mDataReader = _source->createReader(_data);

  if(ld.mDataReader.isNull()) return;

  ld.mDataReader->setHandler(read_handler);
  ld.mDataReader->connectToSource();

  connect(&(ld.timer), &QTimer::timeout, 
      [this]()
      {
        emit press();

        static QMouseEvent event = QMouseEvent(
            QEvent::Type::MouseButtonPress,
            QPointF(),
            QPointF(),
            Qt::MouseButton::LeftButton,
            Qt::MouseButtons(),
            Qt::KeyboardModifiers()
            );
        QGuiApplication::sendEvent(this->currentWidget(), &event);
      });
  installEventFilter(this);
}

LCQStackWidget::~LCQStackWidget()
{
  delete &ld;
}

//------------------------------------------------------------------------------
void LCQStackWidget::addWidget(QWidget* _widget, const QByteArray& _matching)
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

//------------------------------------------------------------------------------
void LCQStackWidget::setPushDelay(int _msec)
{
  if(_msec >= 0) ld.timer.setInterval(_msec);
}

//------------------------------------------------------------------------------
bool LCQStackWidget::eventFilter(QObject*, QEvent* _event)
{

  bool ret = false;

  //---------------------------------------state_released[]
  auto state_released = 
    [this, &ret](QEvent* _event)
    {
      auto to_state_pressed = 
        [this, &ret]()
        {
          ld.timer.start();
          ret = true;
          return EState::pressed;
        };

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonDblClick:
      case QEvent::Type::MouseButtonPress:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          return to_state_pressed();
        }
        break;

      case QEvent::Type::KeyPress:
        {
          QKeyEvent& ke = *(static_cast<QKeyEvent*>(_event));
          if(ke.key() == Qt::Key::Key_Space)
          {
            return to_state_pressed();
          }
        }
      default:
        break;
      }
      return EState::released;
    };

  //---------------------------------------state_pressed[]
  auto state_pressed = 
    [this](QEvent* _event)
    {
      auto to_state_released = 
        [this]()
        {
          if(!(ld.timer.isActive()))
          {
            emit release();
          }
          ld.timer.stop();
          return EState::released;
        };

      switch(_event->type())
      {
      case QEvent::Type::MouseButtonRelease:
        if(static_cast<QMouseEvent*>(_event)->button() == Qt::MouseButton::LeftButton)
        {
          return to_state_released();
        }
        break;

      case QEvent::Type::MouseMove:
        {
          QMouseEvent& me = *(static_cast<QMouseEvent*>(_event));
          if((me.localPos().x() < 0.0) || 
              (me.localPos().y() < 0.0) ||
              (me.localPos().x() > this->size().width()) ||
              (me.localPos().y() > this->size().height()))
          {
            return to_state_released();
          }
        }
        break;

      case QEvent::Type::KeyRelease:
        {
          QKeyEvent& ke = *(static_cast<QKeyEvent*>(_event));
          if((ke.key() == Qt::Key::Key_Space) && (!ke.isAutoRepeat()))
          {
            return to_state_released();
          }
        }
        break;

      default:
        break;
      }
      return EState::pressed;
    };


  switch(ld.state)
  {
  case EState::released:
    ld.state = state_released(_event);
    break;

  case EState::pressed:
    ld.state = state_pressed(_event);
    break;
  }

  return ret;
}




