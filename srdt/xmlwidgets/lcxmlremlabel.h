#ifndef LCXMLREMLABEL_H
#define LCXMLREMLABEL_H

#include "lcxmlwidgetcreatorbase.h"

namespace xmlwidgetcreators
{

class LCXmlRemLabel : public LCXmlWidgetCreatorBase
{
public:
    LCXmlRemLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLREMLABEL_H
