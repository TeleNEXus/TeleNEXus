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
#ifndef LCQREMLINEEDIT_H
#define LCQREMLINEEDIT_H

#include "lqextendevent.h"
#include "LIDataFormatter.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

#include <QLineEdit>

class LCQRemLineEdit : public QLineEdit
{
  Q_OBJECT;

private:

  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIRemoteDataWriter>  mDataWriter;
  QSharedPointer<LIDataFormatter>     mFormatter;

public:

  explicit LCQRemLineEdit(const QString& _dataNameRead,
      const QString& _dataNameWrite,
      QSharedPointer<LIRemoteDataSource> _dataSource,
      QSharedPointer<LIDataFormatter> _formatter,
      QWidget* _parent = nullptr);

  virtual ~LCQRemLineEdit();

protected:

  bool mFlagUpdateOn = false;
  void setActive(bool _flag);
  virtual void keyPressEvent(QKeyEvent *_ev) override;
  virtual void focusInEvent(QFocusEvent *) override;
  virtual void focusOutEvent(QFocusEvent *) override;
  virtual bool event(QEvent *e) override;
};

#endif // LCQREMLINEEDIT_H
