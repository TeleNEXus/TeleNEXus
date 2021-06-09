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
#include "lcqdatalabel.h"
#include <QCoreApplication>
#include <QDebug>
#include <qnamespace.h>

//==============================================================================LCQDataLabel
LCQDataLabel::LCQDataLabel(QWidget* _parent) : QLabel(_parent), mFlagActive(false)
{
  setText("LCQDataLabel");
  setEnabled(false);
}

LCQDataLabel::LCQDataLabel(QString _text, QWidget* _parent) : 
  QLabel(_text, _parent),
  mFlagActive(false)
{
  setEnabled(false);
}

LCQDataLabel::LCQDataLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QLabel(_parent),
  mFormatter(_formatter),
  mFlagActive(false)
{
  setEnabled(false);
  QString str = "Undef";
  setText(str);

  mDataReader = _dataSource->createReader( _dataName,
      [this, str](
        QSharedPointer<QByteArray> _data, 
        LERemoteDataStatus _status)
      {
        if(mFlagActive)
        {
          if(_status != LERemoteDataStatus::DS_OK)
          {
            setText(str);
            setEnabled(false);
            return;
          }
          setText(mFormatter.data()->toString(*_data));
          setEnabled(true);
        }
      }
      );
}

LCQDataLabel::~LCQDataLabel()
{
}

//------------------------------------------------------------------------------setActive
void LCQDataLabel::setActive(bool _flag)
{
  if(mDataReader.isNull()) return;
  if(_flag)
  {
    mFlagActive = true;
    mDataReader->connectToSource();
  }
  else
  {
    mFlagActive = false;
    setEnabled(false);
    mDataReader->disconnectFromSource();
  }
}

//------------------------------------------------------------------------------event
bool LCQDataLabel::event(QEvent *_event)
{
  bool ret = false;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    setActive(true);
    ret = true;
    break;

  case QEvent::Type::Hide:
    setActive(false);
    ret = true;
    break;

  default:
    ret = QLabel::event(_event);
    break;
  }
  return ret;
}

