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
#ifndef LAYOUTBUILDERSCOMMON_H_
#define LAYOUTBUILDERSCOMMON_H_
/*
 * Общие данные и функции для всех построителей.
 */
#include <QString>
#include <qnamespace.h>

class CCommonAttributes
{
public:
    struct SAligns{
        const QString attrName = "align";
        const struct
        {
            QString Left    = "left";
            QString Right   = "right";
            QString Center  = "center";
            QString HCenter = "hcenter";
            QString Top     = "top";
            QString Bottom  = "bottom";
            QString VCenter = "vcenter";
        }vals;
        Qt::Alignment toFlags(const QString& _attributes) const;
        SAligns(){}
    };
    static const SAligns mAligns;
private:
    CCommonAttributes();
};

#endif /* LAYOUTBUILDERSCOMMON_H_ */
