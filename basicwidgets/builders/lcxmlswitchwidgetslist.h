#ifndef LCXMLSWITCHWIDGETSLISTBUILDER_H_
#define LCXMLSWITCHWIDGETSLISTBUILDER_H_

#include "lcxmlbuilderbase.h"

class LCXmlSwitchWidgetsListBuilder : public LCXmlBuilderBase 
{
public:
    LCXmlSwitchWidgetsListBuilder();
    virtual ~LCXmlSwitchWidgetsListBuilder();

protected:
    virtual QWidget* buildLocal( const QDomElement& _element, 
                            const LIApplication& _app) override;
};
#endif /* LCXMLSWITCHWIDGETSLISTBUILDER_H_ */
