#ifndef LCXMLREMLINEEDITBUILDER_H
#define LCXMLREMLINEEDITBUILDER_H

#include "LIXmlWidgetBuilder.h"

class LCXmlRemLineEditBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlRemLineEditBuilder();
    virtual ~LCXmlRemLineEditBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};


#endif // LCXMLREMLINEEDITBUILDER_H
