#ifndef LCXMLREMCOMBOBOXBUILDER_H
#define LCXMLREMCOMBOBOXBUILDER_H

#include "LIXmlWidgetBuilder.h"


class LCXmlRemComboBoxBuilder: public LIXmlWidgetBuilder
{
public:
    LCXmlRemComboBoxBuilder();
    virtual ~LCXmlRemComboBoxBuilder();
    virtual QWidget* build( const QDomElement& _element, 
                            const LIApplication& _app,
                            LIWindow& _window) override;
};

#endif // LCXMLREMCOMBOBOXBUILDER_H
