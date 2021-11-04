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
#ifndef LIREMOTEDATASOURCE_H
#define LIREMOTEDATASOURCE_H
#pragma once

#include <QSharedPointer>
#include <functional>

class LIRemoteDataReader;
class LIRemoteDataWriter;

class LIRemoteDataSource
{
public:


  enum class EReadStatus
  {
    Undef,
    Valid,
    Wrong
  };

  enum class EWriteStatus
  {
    Success,
    Failure
  };

  using TReadHandler = std::function<void(QSharedPointer<QByteArray>, EReadStatus)>;
  using TWriteHandler = std::function<void(EWriteStatus)>;

  LIRemoteDataSource(){}
  virtual ~LIRemoteDataSource(){}

  virtual QSharedPointer<LIRemoteDataReader> createReader(
      const QString& _dataName) = 0;

  virtual QSharedPointer<LIRemoteDataWriter> createWriter(
      const QString& _dataName) = 0;

  //Synchronous call.
  virtual QByteArray read(
      const QString& _dataId, EReadStatus* _status = nullptr) = 0;

  virtual EWriteStatus write(
      const QString& _dataId, const QByteArray& _data) = 0;

  //Asynchronous call.
  virtual void read(
      const QString& _dataId, TReadHandler _handler) = 0;

  virtual void write(
      const QString& _dataId, 
      const QByteArray& _data, 
      TWriteHandler _handler) = 0;

};

#endif // LIREMOTEDATASOURCE_H
