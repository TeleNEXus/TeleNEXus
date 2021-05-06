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
#ifndef LCQLOCALDATAWRITER_H
#define LCQLOCALDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class LCQLocalSourceHiden;

class LCQLocalDataWriter final : public QObject, public LIRemoteDataWriter
{
  Q_OBJECT;
private:
  using LTWriteAction = LIRemoteDataSource::LTWriteAction;

  //--------------------------------------------------------------------------
  class CQEventDataIsWrite : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      const LERemoteDataStatus mStatus;
      explicit CQEventDataIsWrite(LERemoteDataStatus _status);
  };

  QString mDataName;
  LTWriteAction mListener;
  QWeakPointer<LCQLocalSourceHiden> mwpDataSource;
  QWeakPointer<LCQLocalDataWriter> mwpThis;

private:
  LCQLocalDataWriter() = delete;
  LCQLocalDataWriter(
      const QString& _dataName,
      LTWriteAction _writeListener,
      QSharedPointer<LCQLocalSourceHiden> _dataSource);

public:
  virtual ~LCQLocalDataWriter();
  static QSharedPointer<LCQLocalDataWriter> create(
      const QString& _dataName,
      LTWriteAction _writeListener,
      QSharedPointer<LCQLocalSourceHiden> _dataSource);

  virtual void writeRequest(const QByteArray& _data) override;

  QString getDataName(){ return mDataName; }
  void notify(LERemoteDataStatus _status);

private:
  virtual void customEvent(QEvent *_event) override;
};


#endif // LCQLOCALDATAWRITER_H
