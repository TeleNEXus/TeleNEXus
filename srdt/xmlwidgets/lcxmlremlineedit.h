#ifndef LCXMLREMLINEEDIT_H
#define LCXMLREMLINEEDIT_H

#include "lcxmlwidgetcreatorbase.h"

namespace xmlwidgetcreators
{

class LCXmlRemLineEdit : public LCXmlWidgetCreatorBase
{
public:
    LCXmlRemLineEdit();
    virtual QWidget* create(const QDomElement& _element) override;
};

}

#endif // LCXMLREMLINEEDIT_H
