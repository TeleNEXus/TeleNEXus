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
#ifndef LCQJSAPPSERVICE_H_
#define LCQJSAPPSERVICE_H_

#include "lqextendevent.h"

#include <QObject>
#include <QSharedPointer>
#include <QEvent>
#include <QMutex>
#include <QWaitCondition>

class QThread;
class LIApplication;

class LCQJSAppService final 
{
private:
  QThread* mpThread;
  LCQJSAppService();
  LCQJSAppService(const LCQJSAppService&) = delete;
  LCQJSAppService& operator=(const LCQJSAppService&) = delete;

public:

  ~LCQJSAppService();
  static QSharedPointer<LCQJSAppService> getService();

  QByteArray readData(
      const QString& _sourceId, 
      const QString& _dataId);

  int writeData(
      const QString&    _sourceId, 
      const QString&    _dataId, 
      const QByteArray& _data);

  QString getProjectPath();

};
#endif
