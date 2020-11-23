#ifndef LCXMLWIDGETBUILDER_H
#define LCXMLWIDGETBUILDER_H

#include "LIXmlWidgetBuilder.h"
#include <QFrame>


class LCXmlWidgetBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlWidgetBuilder();
    virtual ~LCXmlWidgetBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};

#endif // LCXMLWIDGETBUILDER_H
