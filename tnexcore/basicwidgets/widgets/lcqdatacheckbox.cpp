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
#include "lcqdatacheckbox.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIDataFormatter.h"
#include <QDebug>
#include <qnamespace.h>

//==============================================================================
LCQDataCheckBox::LCQDataCheckBox(   
    const QString&                      _text,
    QSharedPointer<LIRemoteDataReader>  _dataReader,
    QSharedPointer<LIRemoteDataWriter>  _dataWriter,
    const QByteArray&                   _checkData,
    const QByteArray&                   _uncheckData,
    QWidget* _parent):
  QCheckBox(_text, _parent),
  mspDataReader(_dataReader),
  mspDataWriter(_dataWriter),
  mCheckData(_checkData),
  mUncheckData(_uncheckData)
{

  mspDataReader->setHandler(
      [this](QSharedPointer<QByteArray> _data, LIRemoteDataReader::EReadStatus _status)
      {
        if(mFlagActive)
        {
          if(_status != LIRemoteDataReader::EReadStatus::Valid)
          {
            setEnabled(false);
          }
          else
          {
            if(*_data == mCheckData) setChecked(true);
            else setChecked(false);
            setEnabled(true);
          }
        }
      });

  connect(this, &QCheckBox::clicked,
     [this](bool)
     { 
       if(checkState() == Qt::CheckState::Checked)
       {
         mspDataWriter->writeRequest(mCheckData);
       }
       else
       {
         mspDataWriter->writeRequest(mUncheckData);
       }
     });
  setActive(false);
}

//------------------------------------------------------------------------------
LCQDataCheckBox::~LCQDataCheckBox()
{
}

//------------------------------------------------------------------------------
void LCQDataCheckBox::setActive(bool _flag)
{
  if(_flag)
  {
    mFlagActive = true;
    mspDataReader->connectToSource();
  }
  else
  {
    mFlagActive = false;
    setEnabled(false);
    mspDataReader->disconnectFromSource();
  }
}

//------------------------------------------------------------------------------
bool LCQDataCheckBox::event(QEvent *_event)
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
    ret = QCheckBox::event(_event);
    break;
  }
  return ret;
}
