#ifndef LCXMLCONTROLWINDOWSBUTTON_H_
#define LCXMLCONTROLWINDOWSBUTTON_H_

#include "LIXmlWidgetBuilder.h"

class LCXmlControlWindowButtonBuilder: public LIXmlWidgetBuilder
{
public:
    LCXmlControlWindowButtonBuilder();
    virtual ~LCXmlControlWindowButtonBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app,
                            LIWindow& _window) override;
};

#endif /* LCXMLCONTROLWINDOWSBUTTON_H_ */
