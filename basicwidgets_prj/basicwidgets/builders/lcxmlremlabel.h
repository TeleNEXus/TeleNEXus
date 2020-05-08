#ifndef LCXMLREMLABEL_H
#define LCXMLREMLABEL_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemLabel : public LIXmlWidgetBuilder
{
public:
    LCXmlRemLabel();
    virtual ~LCXmlRemLabel();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLREMLABEL_H
