#ifndef LCXMLTABBUILDER_H_
#define LCXMLTABBUILDER_H_

#include "LIXmlWidgetBuilder.h"

class LCXmlTabWidgetBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlTabWidgetBuilder();
    virtual ~LCXmlTabWidgetBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};
#endif /* LCXMLTABBUILDER_H_ */
