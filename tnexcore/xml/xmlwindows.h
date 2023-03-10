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
#ifndef XMLWINDOWS_H_
#define XMLWINDOWS_H_

#include <QSharedPointer>

class QDomElement;
class LIApplication;
class LIWindow;


namespace xmlwindows 
{
void upload( const QDomElement &_element);
void show();
QSharedPointer<LIWindow> getWindow(const QString& _windowId);
}

#endif /* XMLWINDOWS_H_ */
