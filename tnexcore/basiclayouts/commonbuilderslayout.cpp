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
#include "commonbuilderslayout.h"
#include <qnamespace.h>

//==============================================================================
const CCommonAttributes::SAligns CCommonAttributes::mAligns;

CCommonAttributes::CCommonAttributes()
{
}

//------------------------------------------------------------------------------
Qt::Alignment
CCommonAttributes::SAligns::toFlags(const QString& _attributes) const
{
  Qt::Alignment flags = 0;

    if(_attributes.isNull())
    {
        return Qt::AlignLeft;
    }

    QString alines = _attributes.toLower();

    if(alines.contains(vals.Left))
    {
        flags |= Qt::AlignLeft;
    }

    if(alines.contains(vals.Right))
    {
        flags |= Qt::AlignRight;
    }

    if((alines.contains(vals.Center)) && 
        (!alines.contains(vals.HCenter)) &&
        (!alines.contains(vals.VCenter)))
    {
        flags |= Qt::AlignCenter;
    }
    
    if(alines.contains(vals.HCenter))
    {
        flags |= Qt::AlignHCenter;
    }

    if(alines.contains(vals.Top))
    {
        flags |= Qt::AlignTop;
    }

    if(alines.contains(vals.Bottom))
    {
        flags |= Qt::AlignBottom;
    }

    if(alines.contains(vals.VCenter))
    {
        flags |= Qt::AlignVCenter;
    }

    if(flags == 0) return Qt::AlignLeft;

    return flags;
}
