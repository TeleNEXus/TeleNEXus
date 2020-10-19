#ifndef LCXMLGRIDLAYOUTBUILDER_H
#define LCXMLGRIDLAYOUTBUILDER_H

#include "LIXmlLayoutBuilder.h"
#include <qglobal.h>
#include <QString>
#include "common.h"

class LCXmlGridLayoutBuilder : public LIXmlLayoutBuilder
{
public:
    struct SAttributes
    {
        QString spacing     = "spacing";
        QString hspacing    = "hspacing";
        QString vspacing    = "vspacing";
        QString value       = "value";
        QString minheight   = "minheight";
        QString minwidth    = "minwidth";
    };

    struct STags
    {
        QString row         = "row";
        QString column      = "col";
        QString widgets     = "widgets";
        QString layout      = "layout";
        QString stretch     = "stretch"; 
    };

    static const SAttributes mAttributes;
    static const STags mTags;

public:
    LCXmlGridLayoutBuilder();
    virtual ~LCXmlGridLayoutBuilder();

    virtual QLayout* build(
            const QDomElement& _element, 
            const LIApplication& _app) override;
};

#endif // LCXMLGRIDLAYOUTBUILDER_H
