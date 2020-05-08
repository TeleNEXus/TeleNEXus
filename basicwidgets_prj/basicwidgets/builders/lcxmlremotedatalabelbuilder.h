#ifndef LCXMLREMOTEDATALABELBUILDER_H
#define LCXMLREMOTEDATALABELBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemoteDataLabelBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlRemoteDataLabelBuilder();
    virtual ~LCXmlRemoteDataLabelBuilder();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLREMOTEDATALABELBUILDER_H
