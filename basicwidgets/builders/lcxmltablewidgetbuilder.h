#ifndef LCQTABLEWIDGETBUILDER_H
#define LCQTABLEWIDGETBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlTableWidgetBuilder : public LCXmlBuilderBase 
{
public:
    LCXmlTableWidgetBuilder();
    virtual ~LCXmlTableWidgetBuilder();

protected:
    virtual QWidget* buildLocal( const QDomElement& _element, 
                            const LIApplication& _app) override;
};

#endif /* LCQTABLEWIDGETBUILDER_H */
