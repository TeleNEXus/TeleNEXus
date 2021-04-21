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
#ifndef CQJSBINARYFILE_H_
#define CQJSBINARYFILE_H_

#include "cqjsfilebase.h"
class QJSEngine;

class CQJSBinaryFile : public CQJSFileBase
{
  Q_OBJECT;
private:
  CQJSBinaryFile() = delete;

public:
  Q_INVOKABLE CQJSBinaryFile(int _engineId);
  Q_INVOKABLE CQJSBinaryFile(const QString& _fileName, int _engineId);
  Q_INVOKABLE virtual ~CQJSBinaryFile();
public slots:
  qint64 write(const QVariantList& _data);
  QVariantList read(qint64 _maxSize);
  QVariantList readAll();
};

#endif /*CQJSBINARYFILE_H_*/
