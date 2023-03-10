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
#ifndef LCXMLBOXLAYOUTBUILDER_H
#define LCXMLBOXLAYOUTBUILDER_H

#include "LIXmlLayoutBuilder.h"
#include <QString>

class LCXmlBoxLayoutBuilder final : public LIXmlLayoutBuilder
{
public:
    enum class EOrientation
    {
        VERTICAL, 
        HORIZONTAL
    };

    struct SAttributes
    {
        struct
        {
            QString name = "dir";
            struct 
            {
                QString reverse = "reverse";
                QString forward = "forward";
            }vals;
        }dir;

        QString spacing = "spacing";
        QString value = "value";

    };

    struct STags
    {
        QString widgets     = "widgets";
        QString layout      = "layouts";
        QString spacing     = "spacing"; 
        QString stretch     = "stretch";
    };

    static const SAttributes mAttributes;
    static const STags mTags;

private:
    EOrientation mOrientation;
    LCXmlBoxLayoutBuilder();
public:
    LCXmlBoxLayoutBuilder(EOrientation _orient);
    virtual ~LCXmlBoxLayoutBuilder();
    virtual QLayout* build(
            const QDomElement&      _element, 
            const LIApplication&    _app) override;

};

#endif // LCXMLBOXLAYOUTBUILDER_H
