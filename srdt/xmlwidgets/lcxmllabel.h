#ifndef LCXMLLABEL_H
#define LCXMLLABEL_H

#include "lcxmlwidgetcreatorbase.h"

namespace xmlwidgetcreators
{

class LCXmlLabel : public LCXmlWidgetCreatorBase
{
public:
    LCXmlLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLLABEL_H
