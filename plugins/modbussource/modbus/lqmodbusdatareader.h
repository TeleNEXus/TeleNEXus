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
#ifndef LQMODBUSDATAREADER_H
#define LQMODBUSDATAREADER_H

#include "lqextendevent.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"

#include <functional>
class LQModbusDataSource;

class LQModbusDataReader : public QObject, public LIRemoteDataReader
{
  Q_OBJECT;

private:
  //----------------------------------------------------------------------------CQEventDataRead
  class CQEventDataIsRead : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      QSharedPointer<QByteArray> mData;
      EReadStatus mStatus;
      explicit CQEventDataIsRead(const QByteArray& _data, 
          EReadStatus _status);
      explicit CQEventDataIsRead(EReadStatus _status);
  };

  QString mDataName;
  THandler mHandler;
  QWeakPointer<LQModbusDataSource> mwpDataSource;
  QWeakPointer<LQModbusDataReader> mwpThis;

private:
  explicit LQModbusDataReader() = delete;

  explicit LQModbusDataReader(  
      const QString& _dataName,
      QWeakPointer<LQModbusDataSource> _dataSource);

public:
  virtual ~LQModbusDataReader();

  static QSharedPointer<LQModbusDataReader> 
    create( const QString& _dataName, 
        QWeakPointer<LQModbusDataSource> _dataSource);

  virtual void readRequest() override;
  virtual void connectToSource() override;
  virtual void disconnectFromSource() override;
  virtual void setHandler(THandler _handler) override { mHandler = _handler; }

  void notifyListener(const QByteArray& _data, EReadStatus _status);
  void notifyListener(EReadStatus _status);
  QString getDataName(){return mDataName;}

private:
  virtual void customEvent(QEvent* _event) override;
};

#endif // LQMODBUSDATAREADER_H
