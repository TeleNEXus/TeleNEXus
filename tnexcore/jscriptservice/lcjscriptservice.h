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

class LCJScriptService : public LIJScriptService
{
private:
  void* mpData;
public:
  LCJScriptService() = delete;
  explicit LCJScriptService(const QString& _script, int _interval = -1);
  virtual ~LCJScriptService();
  virtual void start() override;
  virtual void stop() override;
};

#endif //LCJSCRIPTSERVICE_H_

