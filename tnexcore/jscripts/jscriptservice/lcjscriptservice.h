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
 * * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LCJSCRIPTSERVICE_H_
#define LCJSCRIPTSERVICE_H_

#include "LIJScriptService.h"
#include <QString>
#include <QSharedPointer>

class LIApplication;

class LCJScriptService : public LIJScriptService
{
private:
  void* mpLocal;
  LCJScriptService() = delete;
  explicit LCJScriptService(
      const QString& _script, 
      const QMap<QString, QString>& _attributes, 
      const QString& _fileName, 
      const QString& _scriptId);
public:
  virtual ~LCJScriptService();

  static QSharedPointer<LIJScriptService> create(
      const QString& _script, 
      const QMap<QString, QString>& _attributes,
      const QString& _fileName,
      const QString& _scriptId);

  virtual void launch(int _interval) override;
  virtual void stop() override;
  virtual void execute() override;
  virtual void action(const QString& _action) override;
};

#endif //LCJSCRIPTSERVICE_H_

