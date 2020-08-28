#ifndef LCXMLGRIDLAYOUTBUILDER_H
#define LCXMLGRIDLAYOUTBUILDER_H

#include "lcxmllayoutbuilderbase.h"

class LCXmlGridLayoutBuilder : public LCXmlLayoutBuilderBase
{
public:
    LCXmlGridLayoutBuilder();
    virtual ~LCXmlGridLayoutBuilder();
    virtual QLayout* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLGRIDLAYOUTBUILDER_H
