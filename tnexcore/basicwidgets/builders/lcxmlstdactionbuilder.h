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

#ifndef LCXMLSTDACTIONBUILDER_H_
#define LCXMLSTDACTIONBUILDER_H_

#include <functional>
#include <QList>

class LIApplication;
class QDomElement;

class LCXmlStdActionBuilder
{
public:

using TActions = QList<std::function<void(void)>>;

private:
  LCXmlStdActionBuilder(const LCXmlStdActionBuilder&) = delete;
  LCXmlStdActionBuilder& operator=(const LCXmlStdActionBuilder&) = delete;
  LCXmlStdActionBuilder();
public:

  static LCXmlStdActionBuilder& instance();
  TActions build( const QDomElement& _element, const LIApplication& _app);

};
#endif /*LCXMLSTDACTIONBUILDER_H_*/
