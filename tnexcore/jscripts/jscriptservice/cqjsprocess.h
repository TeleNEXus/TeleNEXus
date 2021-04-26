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
#ifndef CQJSPROCESS_H_
#define CQJSPROCESS_H_

#include <QProcess>
#include <QJSEngine>

class CQJSProcess : public QObject
{
  Q_OBJECT;
private:
  QProcess mProcess;
  QJSEngine* mpEngine;
public:

  CQJSProcess() = delete;
  Q_INVOKABLE CQJSProcess(int _engineId);
  Q_INVOKABLE virtual ~CQJSProcess();

public slots:
  void start(const QString& _command);
  void kill();
  void terminate();

  void waitForStarted(int msec = 30000);
  void waitForFinished(int msec = 30000);
  void waitForBytesWritten(int msecs = 30000);
  void waitForReadyRead(int msecs = 30000);

  void setWorkingDirectory(const QString& _dir);
  QString workingDirecory();

  bool isRunning();
  bool isStarting();
  bool isCrashed();
  int exitCode();

  void setReadChannelStdOut();
  void setReadChannelStdErr();
  bool isReadChannelStdOut();
  bool isReadCnannelStdErr();
  void closeReadChannelStdOut();
  void closeReadChannelStdErr();
  void closeWriteChannel();

  QVariantList read(qint64 _maxSize);
  QVariantList readAll();
  qint64 write(const QVariantList& _data);

  /* void setEnvironment(const QStringList& _env); */
  /* QStringList getSystemEnvironment(); */
  void setEnvironment(const QVariantMap& _envmap);
  QVariantMap environment();
  /* void Environment(const QVariantMap& _envmap); */
  QVariantMap systemEnvironment();

private slots:
  friend class QProcess;
};

#endif /* CQJSPROCESS_H_ */
