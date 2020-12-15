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
#ifndef LCQREMWRITEBUTTON_H_
#define LCQREMWRITEBUTTON_H_

#include "LIRemoteDataSource.h"
#include <QPushButton>
#include <QList>

class LCQRemWriteButton : public QPushButton
{
private:

  struct SWriteSet
  {
    QByteArray mData;
    QString mDataName;
    QSharedPointer<LIRemoteDataWriter> mWriter;
    void write();
  };

  QList<SWriteSet> mListWriteSets;

public:
  explicit LCQRemWriteButton(QPushButton* _parent = nullptr);
  explicit LCQRemWriteButton(
      const QString& _text, 
      QPushButton* _parent = nullptr);

  void addDataWrite(
      QSharedPointer<LIRemoteDataSource>  _source, 
      const QString&                      _dataName, 
      const QByteArray&                   _data);
};

#endif /* LCQREMWRITEBUTTON_H_ */
