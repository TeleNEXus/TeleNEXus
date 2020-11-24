#ifndef LCXMLWIDGETBUILDER_H
#define LCXMLWIDGETBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlWidgetBuilder : public LCXmlBuilderBase
{
public:
    LCXmlWidgetBuilder();
    virtual ~LCXmlWidgetBuilder();

protected:
    virtual QWidget* buildLocal( 
        const QDomElement& _element, 
        const LIApplication& _app) override;
};

#endif // LCXMLWIDGETBUILDER_H
