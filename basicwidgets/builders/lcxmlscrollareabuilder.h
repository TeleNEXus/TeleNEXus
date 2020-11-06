#ifndef LCXMLSCROLLAREABUILDER_H_ 
#define LCXMLSCROLLAREABUILDER_H_

#include "LIXmlWidgetBuilder.h"

class LCXmlScrollAreaBuilder: public LIXmlWidgetBuilder
{
public:
    LCXmlScrollAreaBuilder();
    virtual ~LCXmlScrollAreaBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};

#endif /* LCXMLSCROLLAREABUILDER_H_ */

