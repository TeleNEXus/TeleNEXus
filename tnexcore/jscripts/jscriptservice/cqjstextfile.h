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
#ifndef CQJSTEXTFILE_H_
#define CQJSTEXTFILE_H_

#include "cqjsfilebase.h"

class CQJSTextFile final : public CQJSFileBase
{
  Q_OBJECT;

public:
  Q_INVOKABLE explicit CQJSTextFile(CQJSFileBase* _parent = nullptr);
  Q_INVOKABLE explicit CQJSTextFile(
      const QString& _fileName, 
      CQJSFileBase* _parent = nullptr);

public slots:

  QString read(quint64 _maxlen);
  QString readAll();
  QString readLine(quint64 _maxlen = 0);
  quint64 write(const QString& _str);
};

#endif //CQJSTEXTFILE_H_
