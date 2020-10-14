#ifndef LCXMLBOXLAYOUTBUILDER_H
#define LCXMLBOXLAYOUTBUILDER_H

#include "LIXmlLayoutBuilder.h"

class LCXmlBoxLayoutBuilder final : public LIXmlLayoutBuilder
{
public:
    enum class EOrientation
    {
        VERTICAL, 
        HORIZONTAL
    };
private:
    EOrientation mOrientation;
    LCXmlBoxLayoutBuilder();
public:
    LCXmlBoxLayoutBuilder(EOrientation _orient);
    virtual ~LCXmlBoxLayoutBuilder();
    virtual QLayout* build(
            const QDomElement& _element, 
            const LIApplication& _app) override;

};
#endif // LCXMLBOXLAYOUTBUILDER_H
