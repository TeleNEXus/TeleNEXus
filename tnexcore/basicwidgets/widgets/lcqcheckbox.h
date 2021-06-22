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
#ifndef LCQCHECKBOX_H_
#define LCQCHECKBOX_H_

#include "lqextendevent.h"

/* #include "LIDataFormatter.h" */
/* #include "LIRemoteDataReader.h" */
/* #include "LIRemoteDataSource.h" */

#include <QCheckBox>
class LIRemoteDataReader;
class LIRemoteDataWriter;
class LIDataFormatter;

class LCQCheckBox : public QCheckBox 
{
  Q_OBJECT;
private:

  QSharedPointer<LIRemoteDataReader>  mspDataReader;
  QSharedPointer<LIRemoteDataWriter>  mspDataWriter;
  QByteArray mCheckData;
  QByteArray mUncheckData;

public:
  explicit LCQCheckBox() = delete;

  explicit LCQCheckBox(   
      const QString&                      _text,
      QSharedPointer<LIRemoteDataReader>  _dataReader,
      QSharedPointer<LIRemoteDataWriter>  _dataWriter,
      const QByteArray&                   _checkData,
      const QByteArray&                   _uncheckData,
      QWidget* _parent = nullptr);

  virtual ~LCQCheckBox();
  virtual bool event(QEvent *_event) override;

private:
  bool mFlagActive;
  void setActive(bool _flag);
};

#endif // LCQCHECKBOX_H_

