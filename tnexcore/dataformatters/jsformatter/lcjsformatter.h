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
#ifndef LCJSFORMATTER_H_
#define LCJSFORMATTER_H_

#include "LIDataFormatter.h"
#include <QSharedPointer>

class QDomElement;
class LCJSFormatter : public LIDataFormatter
{
private:
  void* mpData = nullptr;

  LCJSFormatter() = delete;
  LCJSFormatter(const QDomElement& _element);
public:
  ~LCJSFormatter();
  virtual QString toString(const QByteArray& _data)override;
  virtual QByteArray toBytes(const QString& _str)override;
  virtual QString fitting(const QString& _str)override;
  virtual QValidator* validator()override;

  static QSharedPointer<LCJSFormatter> create(const QDomElement& _element);
};
#endif //LCJSFORMATTER_H_
