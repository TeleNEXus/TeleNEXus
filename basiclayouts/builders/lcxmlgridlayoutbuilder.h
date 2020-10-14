#ifndef LCXMLGRIDLAYOUTBUILDER_H
#define LCXMLGRIDLAYOUTBUILDER_H

#include "LIXmlLayoutBuilder.h"
#include <qglobal.h>

class LCXmlGridLayoutBuilder : public LIXmlLayoutBuilder
{
public:
    LCXmlGridLayoutBuilder();
    virtual ~LCXmlGridLayoutBuilder();
    virtual QLayout* build(const QDomElement& _element, const LIApplication& _app) override
    {
        Q_UNUSED(_element);
        Q_UNUSED(_app);
        return nullptr;
    };
};

#endif // LCXMLGRIDLAYOUTBUILDER_H
