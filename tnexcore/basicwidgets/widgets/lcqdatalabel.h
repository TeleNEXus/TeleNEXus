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
#ifndef LCQDATALABEL_H
#define LCQDATALABEL_H

#include "lqextendevent.h"

#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

#include <QLabel>

class LCQDataLabel : public QLabel
{
  Q_OBJECT;
private:

  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIDataFormatter> mFormatter;

public:
  explicit LCQDataLabel(QWidget* _parent = nullptr);

  explicit LCQDataLabel(QString _text, QWidget* _parent = nullptr);

  explicit LCQDataLabel(   
      const QString&                      _dataName,
      QSharedPointer<LIRemoteDataSource>  _dataSource,
      QSharedPointer<LIDataFormatter>     _formatter,
      QWidget* _parent = nullptr);

  virtual ~LCQDataLabel();
  virtual bool event(QEvent *e) override;
private:
  bool mFlagActive;
  void setActive(bool _flag);
};

#endif // LCQDATALABEL_H
