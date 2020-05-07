#ifndef LCXMLTEXTLABELBUILDER_H
#define LCXMLTEXTLABELBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlTextLabelBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlTextLabelBuilder();
    virtual ~LCXmlTextLabelBuilder();
    virtual QSharedPointer<QWidget> build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLTEXTLABELBUILDER_H
