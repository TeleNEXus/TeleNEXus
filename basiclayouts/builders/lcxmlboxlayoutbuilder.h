#ifndef LCXMLBOXLAYOUTBUILDER_H
#define LCXMLBOXLAYOUTBUILDER_H

#include "lcxmllayoutbuilderbase.h"
#include <QBoxLayout>

class LCXmlBoxLayoutBuilder final : public LCXmlLayoutBuilderBase
{
private:
    struct SAttributes
    {
        QString direction = "direction";
        struct
        {
           QString leftToRight = "LeftToRight";
           QString rightToLeft = "RightToLeft";
           QString topToBottom = "TopToBottom";
           QString bottomToTop = "BottomToTop";
        } directionVals;
    };
    static const SAttributes mAttributes;
    /* bool mFlagDirModeDef; */
    QBoxLayout::Direction mDirectionDef;
public:
    LCXmlBoxLayoutBuilder();
    /* LCXmlBoxLayoutBuilder(QBoxLayout::Direction _direction); */
    virtual ~LCXmlBoxLayoutBuilder();
    virtual QLayout* build(const QDomElement& _element, const LIApplication& _app) override;

};
#endif // LCXMLBOXLAYOUTBUILDER_H
