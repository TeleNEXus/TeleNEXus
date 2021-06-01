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
#include "lcqremcombobox.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"

#include <qglobal.h>
#include <QKeyEvent>
#include <QDebug>

//------------------------------------------------------------------------------
LCQRemComboBox::LCQRemComboBox( 
    const QString&                       _dataNameRead, 
    const QString&                       _dataNameWrite,
    QSharedPointer<LIRemoteDataSource>   _dataSource,
    QSharedPointer<LIDataFormatter>      _formatter,
    QWidget* _parent):  
  QComboBox(_parent),
  mFormatter(_formatter),
  mFlagPopupOn(false)
{

  mDataReader = _dataSource->createReader(_dataNameRead, 
      [this](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
      {
        switch(_status)
        {
        case LERemoteDataStatus::DS_OK:
          setCurrentIndex(findData( mFormatter->toString( *_data.data()))); 
          setEnabled(true);
          break;

        case LERemoteDataStatus::DS_WRONG:
          setCurrentIndex(-1);
          setEnabled(true);
          break;

        case LERemoteDataStatus::DS_UNDEF:
          setCurrentIndex(-1);
          setEnabled(false);
        default:
          break;
        }
      });

  mDataWriter = _dataSource->createWriter(_dataNameWrite);

  connect(this, static_cast <void(LCQRemComboBox::*)(int)> 
      (&LCQRemComboBox::activated),
      [&](int index)
      {
        Q_UNUSED(index);
        mDataWriter->writeRequest(
            mFormatter->toBytes(currentData().toString()));
      });

  setEnabled(false);
}

//------------------------------------------------------------------------------
LCQRemComboBox::~LCQRemComboBox()
{
}

//------------------------------------------------------------------------------
bool LCQRemComboBox::event(QEvent *_event)
{
  int key;
  bool ret = false;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    if(!mIsVisibleFlag)
    {
      mIsVisibleFlag = true;
      mDataReader->connectToSource();
      setCurrentIndex(-1);
      ret = true;
    }
    break;

  case QEvent::Type::Hide:
    if(mIsVisibleFlag)
    {
      mIsVisibleFlag = false;
      mDataReader->disconnectFromSource();
      ret = true;
    }
    break;

  case QEvent::Type::KeyPress:
    //Очиска фокуса видета при нажатии клавиши Escape.
    key = static_cast<QKeyEvent*>(_event)->key();
    if(( key == Qt::Key_Enter) || ( key == Qt::Key_Return))
    {
      showPopup();
    } else if( static_cast<QKeyEvent*>(_event)->key() == Qt::Key_Escape)
    {
      if ( mFlagPopupOn )
      {
        hidePopup();
      }
      else
      {
        clearFocus();
      }
    }
    ret = true;
    break;

  default:
  ret = QComboBox::event(_event);
    break;
  }
  return ret;
}

//------------------------------------------------------------------------------
void LCQRemComboBox::showPopup(void)
{
  mFlagPopupOn = true;
  QComboBox::showPopup();
}

//------------------------------------------------------------------------------
void LCQRemComboBox::hidePopup(void)
{
  mFlagPopupOn = false;
  mDataReader->readRequest();
  QComboBox::hidePopup();
}
