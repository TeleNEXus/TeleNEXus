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

#ifndef LCLOCALSOURCEHIDEN_H_
#define LCLOCALSOURCEHIDEN_H_

#include "lcdataitemmap.h"
#include "lqextendevent.h"
#include "LIRemoteDataSource.h"

#include <QObject>

class LCQLocalDataReader;
class LCQLocalDataWriter;

class LCQLocalSourceHiden final : public QObject 
{

private:
  using LTReadAction = LIRemoteDataSource::LTReadAction;
  using LTWriteAction = LIRemoteDataSource::LTWriteAction;
  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQLocalSourceHiden* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventConnetcReader
  class CEventConnectReader : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventConnectReader() = delete;
    CEventConnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventDisconnectReader
  class CEventDisconnectReader : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventDisconnectReader() = delete;
    CEventDisconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventRead
  class CEventRead : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventRead() = delete;
    CEventRead(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventWrite
  class CEventWrite : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataWriter> mspDataWriter;
    QByteArray mData;
  public:
    CEventWrite() = delete;
    CEventWrite(
        QSharedPointer<LCQLocalDataWriter> _sp_writer, 
        const QByteArray& _data);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  LCDataItemMap mDataMap;
  QWeakPointer<LCQLocalSourceHiden> mwpThis;
  explicit LCQLocalSourceHiden();
public:
  virtual ~LCQLocalSourceHiden();

  static QSharedPointer<LCQLocalSourceHiden> create();

  void addDataItem(const QString& _id, const QByteArray& _data); 
  void addDataItem(const QString& _id, const QBitArray& _data); 
  void addDataItem(const QString& _id, const QString& _data); 
  void addStreamItem(const QString& _id); 

//TODO: Добавление сущности потока данных.

  QSharedPointer<LCQLocalDataReader> createReader(
      const QString& _dataName, 
      LTReadAction _readAction);


  QSharedPointer<LCQLocalDataWriter> createWriter(
      const QString& _dataName, 
      LTWriteAction _writeListener);

private:
  int counter = 0;

  void connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void disconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void read(QSharedPointer<LCQLocalDataReader> _ps_reader);
  void write(
      const QByteArray& _data, 
      QSharedPointer<LCQLocalDataWriter> _sp_writer);

  virtual void customEvent(QEvent* _event) override;

  friend class LCQLocalDataReader;
  friend class LCQLocalDataWriter;
};

#endif //LCLOCALSOURCEHIDEN_H_
