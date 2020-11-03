#ifndef LCXMLSWITCHWIDGETSLISTBUILDER_H_
#define LCXMLSWITCHWIDGETSLISTBUILDER_H_

#include "LIXmlWidgetBuilder.h"

class LCXmlSwitchWidgetsListBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlSwitchWidgetsListBuilder();
    virtual ~LCXmlSwitchWidgetsListBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};
#endif /* LCXMLSWITCHWIDGETSLISTBUILDER_H_ */
