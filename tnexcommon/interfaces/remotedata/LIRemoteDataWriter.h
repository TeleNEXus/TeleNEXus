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
#ifndef LIREMOTEDATAWRITER_H
#define LIREMOTEDATAWRITER_H

#include "LIRemoteDataSource.h"
#include <functional>
#include <QSharedPointer>

class QByteArray;

class LIRemoteDataWriter
{
public:

  using EWriteStatus = LIRemoteDataSource::EWriteStatus;

  using THandler = std::function<void(EWriteStatus)>;

  LIRemoteDataWriter(){}
  virtual ~LIRemoteDataWriter(){}
  virtual void writeRequest( const QByteArray& _data) = 0;
  virtual void setHandler(THandler _handler) = 0;

  /* virtual EWriteStatus writeSync(const QByteArray& _data) = 0; */
};

#endif // LIREMOTEDATAWRITER_H
