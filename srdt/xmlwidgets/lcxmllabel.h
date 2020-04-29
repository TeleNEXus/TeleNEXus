#ifndef LCXMLLABEL_H
#define LCXMLLABEL_H

#include "lcxmlwidgetcreatorinterface.h"

namespace xmlwidgetcreators
{

class LCXmlLabel : public LCXmlWidgetCreatorInterface
{
public:
    LCXmlLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLLABEL_H
