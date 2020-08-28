#ifndef LCXMLREMOTEDATALINEEDITBUILDER_H
#define LCXMLREMOTEDATALINEEDITBUILDER_H

#include "LIXmlWidgetBuilder.h"

class LCXmlRemoteDataLineEditBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlRemoteDataLineEditBuilder();
    virtual ~LCXmlRemoteDataLineEditBuilder();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};


#endif // LCXMLREMOTEDATALINEEDITBUILDER_H
