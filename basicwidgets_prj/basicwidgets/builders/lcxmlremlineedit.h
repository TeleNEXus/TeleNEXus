#ifndef LCXMLREMLINEEDIT_H
#define LCXMLREMLINEEDIT_H

#include "LIXmlWidgetBuilder.h"

class LCXmlRemLineEdit : public LIXmlWidgetBuilder
{
public:
    LCXmlRemLineEdit();
    virtual ~LCXmlRemLineEdit();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};


#endif // LCXMLREMLINEEDIT_H
