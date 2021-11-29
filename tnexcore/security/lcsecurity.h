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

#ifndef LCSECURITY_H_
#define LCSECURITY_H_

#include <QObject>
#include <QSet>
#include <QEvent>
#include <QPair>
#include <QMap>
#include <QSharedPointer>

class QDomElement;
class LIApplication;
class QTimer;

class LIRemoteDataReader;
class LIRemoteDataWriter;

class LCSecurity
{
private:

  bool mFlagInit;
  int mCurrentLevel;
  QString mAccessId;
  QMap<QString, QPair<int, QByteArray>> mAccesses;
  QTimer* mpTimer;

  QSharedPointer<LIRemoteDataReader> mspPasswordReader;
  QSharedPointer<LIRemoteDataReader> mspAccessIdReader;
  QSharedPointer<LIRemoteDataReader> mspCommandReader;
  QSharedPointer<LIRemoteDataWriter> mspMessageWriter;

private:
  LCSecurity();
  LCSecurity(const LCSecurity&) = delete;
  LCSecurity& operator=(const LCSecurity&) = delete;
  ~LCSecurity();

public:

  static LCSecurity& instance();
  void init(const QDomElement& _element, const LIApplication& _app);
  QObject* createEventFilter(const QDomElement& _element, const QSet<QEvent::Type>& _events) const;
  bool checkAccess(const QString& _accessId);

private:
  void autorize(const QString& _accessId, const QString& _password);
  void writeMessage(const QString& _message);
};

#endif
