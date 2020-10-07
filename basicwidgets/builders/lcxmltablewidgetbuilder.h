#ifndef LCQTABLEWIDGETBUILDER_H
#define LCQTABLEWIDGETBUILDER_H

#include "LIXmlWidgetBuilder.h"

class LCXmlTableWidgetBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlTableWidgetBuilder();
    virtual ~LCXmlTableWidgetBuilder();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif /* LCQTABLEWIDGETBUILDER_H */
