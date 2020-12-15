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

#include <QSharedPointer>
#include <QByteArray>
#include <functional>

enum class LERemoteDataStatus
{
  DS_UNDEF, // Первичной инициализации статуса операции с данными 
            // и при попытке чтения несуществующих данных.
  DS_OK,    // Операция с данными корректна.
  DS_WRONG  // Операция с данными ошибочна.
};

class LIRemoteDataReader;
class LIRemoteDataWriter;

class LIRemoteDataSource
{
public:

  using LTWriteAction = std::function<void(LERemoteDataStatus)>;
  using LTReadAction = 
    std::function<void(QSharedPointer<QByteArray>, LERemoteDataStatus)>; 

  LIRemoteDataSource(){}
  virtual ~LIRemoteDataSource(){}

  virtual QSharedPointer<LIRemoteDataReader> createReader(
      const QString& _dataName,
      LTReadAction _readAction) = 0;

  virtual QSharedPointer<LIRemoteDataWriter> createWriter(
      const QString& _dataName,
      LTWriteAction _writeAction = [](LERemoteDataStatus){}) = 0;


  /* //Блокирующие вызовы. */
  /* virtual QByteArray read( */
  /*     const QString& _dataName, */ 
  /*     LERemoteDataStatus* _status = nullptr) = 0; */

  /* virtual LERemoteDataStatus write( */
  /*     const QString& _dataName, */ 
  /*     const QByteArray& _data) = 0; */

  /* //Асинхронные вызовы. */
  /* virtual void read( */
  /*     const QString& _dataName, */ 
  /*     std::function<void( */
  /*       QSharedPointer<QByteArray>, LERemoteDataStatus)> _listener) = 0; */

  /* virtual LERemoteDataStatus write( */
  /*     const QString& _dataName, */ 
  /*     std::function<void(LERemoteDataStatus)> _isWrite) = 0; */
};

#endif // LIREMOTEDATASOURCE_H
