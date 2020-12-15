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
#ifndef LCQREMCOMBOBOX_H
#define LCQREMCOMBOBOX_H

#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataSource.h"

#include <QComboBox>

class LCQRemComboBox : public QComboBox
{
  Q_OBJECT;
private:
  QSharedPointer<LIRemoteDataReader>   mDataReader;
  QSharedPointer<LIRemoteDataWriter>   mDataWriter;
  QSharedPointer<LIDataFormatter>      mFormatter;
  bool mFlagPopupOn; //Флаг состояния всплывающего меню.

public:
  explicit LCQRemComboBox(QWidget* _parent = nullptr) = delete;

  explicit LCQRemComboBox(    
      const QString&                      _dataNameRead,
      const QString&                      _dataNameWrite,
      QSharedPointer<LIRemoteDataSource>  _dataSource,
      QSharedPointer<LIDataFormatter>     _formatter,
      QWidget* _parent = nullptr);
  virtual ~LCQRemComboBox();
  virtual bool event(QEvent *e) override;
private:
  virtual void showPopup(void) override;
  virtual void hidePopup(void) override;
};

#endif // LCQREMCOMBOBOX_H
