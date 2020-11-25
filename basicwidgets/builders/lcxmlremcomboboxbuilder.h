#ifndef LCXMLREMCOMBOBOXBUILDER_H
#define LCXMLREMCOMBOBOXBUILDER_H

#include "lcxmlbuilderbase.h"

class LCXmlRemComboBoxBuilder: public LCXmlBuilderBase 
{

public:
    LCXmlRemComboBoxBuilder();
    virtual ~LCXmlRemComboBoxBuilder();

protected:
    virtual QWidget* buildLocal( const QDomElement& _element, 
                            const LIApplication& _app) override final;
};

#endif // LCXMLREMCOMBOBOXBUILDER_H
