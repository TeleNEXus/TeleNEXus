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
#ifndef LCLOCALSOURCE_H_
#define LCLOCALSOURCE_H_

#include "LIRemoteDataSource.h"
#include <QSharedPointer>

class LCLocalDataSource : public LIRemoteDataSource
{
private:
  void* mpData;
  LCLocalDataSource();
  LCLocalDataSource(const LCLocalDataSource&) = delete;
  LCLocalDataSource& operator=(const LCLocalDataSource&) = delete;
public:
  virtual ~LCLocalDataSource();

  static QSharedPointer<LCLocalDataSource> create();

  void addByteItem(const QString& _dataName, const QByteArray& _data); 
  void addBitItem(const QString& _dataName, const QBitArray& _data); 
  void addStringItem(const QString& _dataName, const QString& _data); 
  void addStreamItem(const QString& _dataName); 

  virtual QSharedPointer<LIRemoteDataReader> 
    createReader(const QString& _dataName) override;

  virtual QSharedPointer<LIRemoteDataWriter> 
    createWriter(const QString& _dataName) override;

  //Synchronous call.
  virtual QByteArray read(
      const QString& _dataId, EReadStatus* _status = nullptr) override;

  virtual EWriteStatus write(
      const QString& _dataId, const QByteArray& _data) override;

  //Asynchronous call.
  virtual void read(
      const QString& _dataId, TReadHandler _handler) override;

  virtual void write(
      const QString& _dataId, 
      const QByteArray& _data, 
      TWriteHandler _handler) override;
};

#endif //LCLOCALSOURCE_H_
