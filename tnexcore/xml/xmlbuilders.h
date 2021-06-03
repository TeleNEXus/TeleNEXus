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
#ifndef LCXMLBUILDERS_H
#define LCXMLBUILDERS_H

#include <QDomElement>
#include <QSharedPointer>

class LIXmlWidgetBuilder;
class LIXmlLayoutBuilder;
class LIXmlRemoteDataSourceBuilder;

namespace builders
{

namespace widgets
{
void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths);
QSharedPointer<LIXmlWidgetBuilder> getBuilder(const QString _id);
} /* namespace widget */ 

namespace layouts
{
void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths);

QSharedPointer<LIXmlLayoutBuilder> getBuilder(const QString _name);
} /* namespace layouts */

namespace sources 
{
void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths);

QSharedPointer<LIXmlRemoteDataSourceBuilder> getBuilder(const QString _name);
} /* namespace sources*/

} /* namespace uploadbuilders */

#endif // LCXMLBUILDERS_H
