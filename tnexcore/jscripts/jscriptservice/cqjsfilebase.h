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
#ifndef CQJSFILE_H_
#define CQJSFILE_H_

#include <QJSEngine>
#include <QFile>

class CQJSFileBase : public QObject
{

  Q_OBJECT;

private:
  QFile mFile;

private:
  Q_INVOKABLE explicit CQJSFileBase(QObject* _parent = nullptr);
  Q_INVOKABLE explicit CQJSFileBase(const QString& _fileName, QObject* _parent = nullptr);
  Q_INVOKABLE virtual ~CQJSFileBase();

public slots:
  virtual bool open(const QString& _openMode);
  QString openMode() const;
  QString fileName() const;
  bool setFileName(const QString& _fileName);
  virtual void close();
  bool isOpen() const;
  bool atEnd() const;
  bool exists() const;
  bool remove();
  bool rename(const QString& _newName);
  quint64 pos() const;
  virtual bool seek(quint64 _pos);
  quint64 size() const;
  bool flush();

  friend class CQJSTextFile;
  friend class CQJSBinaryFile;
};

#endif //CQJSFILE_H_
