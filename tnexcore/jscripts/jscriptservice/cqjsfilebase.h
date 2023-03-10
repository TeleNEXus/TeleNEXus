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

protected:
  QFile mFile;
  QJSEngine* mpEngine;

  static const struct SOpenModes
  {
    QString NotOpen     = "no";
    QString ReadOnly    = "r";
    QString WriteOnly   = "w";
    QString ReadWrite   = "rw";
    QString Append      = "a";
  }mOpenModes;

protected:
  CQJSFileBase() = delete;

  Q_INVOKABLE explicit CQJSFileBase(QJSEngine* _jsengine);
  Q_INVOKABLE explicit CQJSFileBase(const QString& _fileName, 
      QJSEngine* _jsengine);
  Q_INVOKABLE virtual ~CQJSFileBase();

  bool readAllowCheck();
  bool writeAllowCheck();

public slots:
  virtual bool open(const QString& _openMode);
  QString openMode() const;
  QString fileName() const;
  void  setFileName(const QString& _fileName);
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
  bool copy(const QString& _fileName);
};

#endif //CQJSFILE_H_
