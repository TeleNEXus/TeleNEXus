#ifndef LCXMLREMLABEL_H
#define LCXMLREMLABEL_H

#include "lcxmlwidgetcreatorinterface.h"

namespace xmlwidgetcreators
{

class LCXmlRemLabel : public LCXmlWidgetCreatorInterface
{
public:
    LCXmlRemLabel();
    virtual ~LCXmlRemLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLREMLABEL_H
