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
#include <qnamespace.h>

class CReaderStub : public LIRemoteDataReader
{
public:
  CReaderStub(){}
  virtual ~CReaderStub(){}
  virtual void readRequest()override {}
  virtual void connectToSource()override {}
  virtual void disconnectFromSource()override {}
  virtual void setHandler(THandler)override {}
};

#define lm_UndefValue QStringLiteral("----")

//==============================================================================LCQDataLcdNumber
LCQDataLcdNumber::LCQDataLcdNumber(QWidget* _parent) : 
  QLCDNumber(_parent), 
  mFlagActive(false)
{
  display(lm_UndefValue);
  setEnabled(false);
}


LCQDataLcdNumber::LCQDataLcdNumber(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    
  QLCDNumber(_parent), 
  mFormatter(_formatter),
  mFlagActive(false)
{
  display(lm_UndefValue);
  setEnabled(false);

  mDataReader = _dataSource->createReader(_dataName);

  if(mDataReader.isNull())
  {
    mDataReader = QSharedPointer<LIRemoteDataReader>(new CReaderStub);
    return;
  }

  mDataReader->setHandler(
      [this](
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
            display(mFormatter.data()->toString(*_data));
            setEnabled(true);
          }
        }
      });
}

LCQDataLcdNumber::~LCQDataLcdNumber()
{
}

//------------------------------------------------------------------------------setActive
void LCQDataLcdNumber::setActive(bool _flag)
{
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


