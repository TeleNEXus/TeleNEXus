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
#include "lcqremwritebutton.h"
#include "LIRemoteDataWriter.h"

//==============================================================================SWriteSet
void LCQRemWriteButton::SWriteSet::write()
{
  mWriter->writeRequest(mData);
}

//==============================================================================LCQRemWriteButton
LCQRemWriteButton::LCQRemWriteButton(QPushButton* _parent)
{
  LCQRemWriteButton("RemWriteButton", _parent);
}

LCQRemWriteButton::LCQRemWriteButton(
    const QString&      _text, 
    QPushButton*        _parent) : 
  QPushButton(_text, _parent)
{
  QString text = _text;
  if(text.isNull()) text = "RemWriteButton";
  //Подключение обработки сигнала нажания.
  connect(this, &LCQRemWriteButton::pressed, 
      [&]()
      {
        for(    auto it = mListWriteSets.begin(); 
            it != mListWriteSets.end(); 
            it++)
        {
          it->write();
        }
      });
}

//------------------------------------------------------------------------------addDataWrite
void LCQRemWriteButton::addDataWrite(
    QSharedPointer<LIRemoteDataSource>  _source, 
    const QString&                      _dataName, 
    const QByteArray&                   _data)
{
  SWriteSet write_set;

  if(_source.isNull()) return;

  write_set.mData     = _data;
  write_set.mDataName = _dataName;
  write_set.mWriter   = _source->createWriter(write_set.mDataName);
  mListWriteSets << write_set;
}
