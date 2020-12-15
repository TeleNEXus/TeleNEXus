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
#include "lcqremlineedit.h"
#include <QKeyEvent>
#include <qnamespace.h>
#include <QKeyEvent>

//------------------------------------------------------------------------------
LCQRemLineEdit::LCQRemLineEdit(
    const QString& _dataNameRead,
    const QString& _dataNameWrite,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :  
  QLineEdit(_parent),
  mFormatter(_formatter)
{
  QString str = "Undef";
  mFormatter.data()->undefState(str);
  setText(str);
  setValidator(_formatter->validator());
  setEnabled(false);

  mDataReader = _dataSource->createReader(_dataNameRead,
      [this](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
      {
        if(mFlagUpdateOn)
        {
          if(_status != LERemoteDataStatus::DS_OK)
          {
            QString str = "Undef";
            mFormatter.data()->undefState(str);
            setText(str);
            setEnabled(false);
            return;
          }
          setEnabled(true);
          setText(mFormatter.data()->toString(*_data));
        }
      });

  mDataWriter = _dataSource->createWriter(_dataNameWrite);
}

//------------------------------------------------------------------------------
LCQRemLineEdit::~LCQRemLineEdit()
{

}

void LCQRemLineEdit::setActive(bool _flag)
{
  if(_flag)
  {
    mDataReader->connectToSource();
    mFlagUpdateOn = true;
  }else
  {
    mFlagUpdateOn = false;
    setEnabled(false);
    mDataReader->disconnectFromSource();
  }
}

//------------------------------------------------------------------------------keyPressEvent
void LCQRemLineEdit::keyPressEvent(QKeyEvent *_ev)
{
  if((_ev->key() == Qt::Key_Enter) ||(_ev->key() == Qt::Key_Return))
  {
    mDataWriter->writeRequest(mFormatter->toBytes(text()));
    mDataReader->readRequest();
    mFlagUpdateOn = true;
  }
  else
  {
    mFlagUpdateOn = false;
  }
  QLineEdit::keyPressEvent(_ev);
}

void LCQRemLineEdit::focusInEvent(QFocusEvent *_event)
{
  mFlagUpdateOn = false;
  QLineEdit::focusInEvent(_event);
}

void LCQRemLineEdit::focusOutEvent(QFocusEvent *_event)
{
  mFlagUpdateOn = true;
  mDataReader->readRequest();
  QLineEdit::focusOutEvent(_event);
}

//------------------------------------------------------------------------------event
bool LCQRemLineEdit::event(QEvent *_event)
{
  switch(_event->type())
  {
  case QEvent::Type::Show:
    setActive(true);
    return false;

  case QEvent::Type::Hide:
    setActive(false);
    return false;

  case QEvent::KeyPress:
    //Очиска фокуса видета при нажатии клавиши Escape.
    {
      QKeyEvent* e = dynamic_cast<QKeyEvent*>(_event);
      if(e)
      {
        if( e->key() == Qt::Key_Escape)
        {
          clearFocus();
          return false;
        }
      }
    }
    break;

  default:
    break;
  }
  return QLineEdit::event(_event);
}

