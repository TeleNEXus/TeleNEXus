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
#ifndef LQMODBUSDATAWRITER_H
#define LQMODBUSDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class LQModbusDataSource;
class LQModbusDataWriter : public QObject, public LIRemoteDataWriter
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

  LTWriteAction mWriteAction;
  QWeakPointer<LQModbusDataSource> mwpDataSource;
  QWeakPointer<LQModbusDataWriter> mwpThis;

private:
  explicit LQModbusDataWriter() = delete;

  explicit LQModbusDataWriter(
      const QString& _dataName,
      LTWriteAction _writeAction,
      QSharedPointer<LQModbusDataSource> _dataSource);
public:

  virtual ~LQModbusDataWriter();

  static QSharedPointer<LQModbusDataWriter> create(
      const QString& _dataName,
      LTWriteAction _writeAction,
      QSharedPointer<LQModbusDataSource> _dataSource);

  virtual void writeRequest(const QByteArray& _data) override;
  QString getDataName(){ return mDataName; }
  void notifyListener(LERemoteDataStatus _status);

private:
  virtual void customEvent(QEvent *_event) override;
};

#endif // LQMODBUSDATAWRITER_H
