#ifndef LCXMLGRIDLAYOUTBUILDER_H
#define LCXMLGRIDLAYOUTBUILDER_H

#include "LIXmlLayoutBuilder.h"
#include <qglobal.h>

class LCXmlGridLayoutBuilder : public LIXmlLayoutBuilder
{
public:
    LCXmlGridLayoutBuilder();
    virtual ~LCXmlGridLayoutBuilder();
    virtual QLayout* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLGRIDLAYOUTBUILDER_H
