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

  static void addQMetaObject(
      QJSEngine& _jsengine, 
      QJSValue& _jsvalue, 
      const QString& _objectName);

public slots:
  void start(const QString& _command);
  void kill();
  void terminate();
  bool waitForStarted(int msec = 30000);
  bool waitForFinished(int msec = 30000);
  int exitCode();
  int stateCode();
  int errorCode();

  void closeReadChannel(int _channel);
  void closeWriteChannel();
  int readChannel();
  void setReadChannel(int _channel);
  QVariantList read(qint64 _maxSize);
  QVariantList readAll();
  qint64 write(const QVariantList& _data);
  bool waitForBytesWritten(int msecs = 30000);
  bool waitForReadyRead(int msecs = 30000);

private slots:
  friend class QProcess;
};

#endif /* CQJSPROCESS_H_ */
