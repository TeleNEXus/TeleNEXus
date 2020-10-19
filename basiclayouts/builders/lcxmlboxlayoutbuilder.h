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
        QString layout      = "layout";
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
            const QDomElement& _element, 
            const LIApplication& _app) override;

};
#endif // LCXMLBOXLAYOUTBUILDER_H
