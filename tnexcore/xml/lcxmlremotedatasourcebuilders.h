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
#ifndef LCXMLSOURCEBUILDERLOADER_H
#define LCXMLSOURCEBUILDERLOADER_H

#include <QSharedPointer>
#include <QDomElement>

#include "lcxmlbuildersloader.h"
#include "LIXmlRemoteDataSourceBuilder.h"

class LCXmlRemoteDataSourceBuilders : public LCXmlBuildersLoader
{
public:

private:
    LCXmlRemoteDataSourceBuilders();
    LCXmlRemoteDataSourceBuilders(const LCXmlRemoteDataSourceBuilders&) = delete;
    LCXmlRemoteDataSourceBuilders& operator=(const LCXmlRemoteDataSourceBuilders&) = delete;
    virtual ~LCXmlRemoteDataSourceBuilders(){}
public:
    static LCXmlRemoteDataSourceBuilders& instance();
    QSharedPointer<LIXmlRemoteDataSourceBuilder> getBuilder(const QString _name);
    virtual bool add(const QString &_name, void *_builder) override;
    bool noItems();
};

#endif // LCXMLSOURCEBUILDERLOADER_H
