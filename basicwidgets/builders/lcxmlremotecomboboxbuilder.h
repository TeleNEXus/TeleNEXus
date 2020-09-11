#ifndef LCXMLREMOTEDATALABELBUILDER_H
#define LCXMLREMOTEDATALABELBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemoteComboBoxBuilder : public LIXmlWidgetBuilder
{
public:
    LCXmlRemoteComboBoxBuilder();
    virtual ~LCXmlRemoteComboBoxBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app) override;
};

#endif // LCXMLREMOTEDATALABELBUILDER_H
