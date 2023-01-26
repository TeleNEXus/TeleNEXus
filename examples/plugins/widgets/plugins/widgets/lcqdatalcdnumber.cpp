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

#include "lcqdatalcdnumber.h"
#include <QCoreApplication>
#include <QDebug>

#define lm_UndefValue QStringLiteral("----")

//==============================================================================LCQDataLcdNumber
LCQDataLcdNumber::LCQDataLcdNumber(QWidget* _parent) : 
  QLCDNumber(_parent),
  mFlagActive(false)
{
  display(lm_UndefValue);
  setEnabled(false);
}

LCQDataLcdNumber::~LCQDataLcdNumber()
{
}

//------------------------------------------------------------------------------init
void LCQDataLcdNumber::init(
    const QString&                      _dataId,
    QSharedPointer<LIRemoteDataSource>  _dataSource,
    QSharedPointer<LIDataFormatter>     _formatter)
{

  mDataReader = _dataSource->createReader(_dataId);

  if(mDataReader.isNull())
  {
    return;
  }

  mDataReader->setHandler(
      [this, _formatter](
        QSharedPointer<QByteArray> _data, 
        LIRemoteDataReader::EReadStatus _status)
      {
        if(mFlagActive)
        {
          if(_status != LIRemoteDataReader::EReadStatus::Valid)
          {
            display(lm_UndefValue);
            setEnabled(false);
          }
          else
          {
            display(_formatter.data()->toString(*_data));
            setEnabled(true);
          }
        }
      });
}

//------------------------------------------------------------------------------setActive
void LCQDataLcdNumber::setActive(bool _flag)
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
bool LCQDataLcdNumber::event(QEvent *_event)
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
    ret = QLCDNumber::event(_event);
    break;
  }
  return ret;
}


