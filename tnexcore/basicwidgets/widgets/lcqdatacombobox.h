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
#ifndef LCQDATACOMBOBOX_H
#define LCQDATACOMBOBOX_H

#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataSource.h"

#include <QComboBox>

class LCQDataComboBox : public QComboBox
{
  Q_OBJECT;
private:
  bool mIsVisibleFlag = false;
  QSharedPointer<LIRemoteDataReader>   mDataReader;
  QSharedPointer<LIDataFormatter>      mFormatterRead;
  QSharedPointer<LIRemoteDataWriter>   mDataWriter;
  QSharedPointer<LIDataFormatter>      mFormatterWrite;
  bool mFlagPopupOn; //Флаг состояния всплывающего меню.

public:
  explicit LCQDataComboBox(QWidget* _parent = nullptr) = delete;

  explicit LCQDataComboBox(    
      QSharedPointer<LIRemoteDataReader>  _dataReader,
      QSharedPointer<LIDataFormatter>     _formatterRead,
      QSharedPointer<LIRemoteDataWriter>  _dataWriter,
      QSharedPointer<LIDataFormatter>     _formatterWrite,
      QWidget* _parent = nullptr);

  virtual ~LCQDataComboBox();
  virtual bool event(QEvent *e) override;
private:
  virtual void showPopup(void) override;
  virtual void hidePopup(void) override;
};

#endif // LCQDATACOMBOBOX_H
