#ifndef LCXMLREMLINEEDIT_H
#define LCXMLREMLINEEDIT_H

#include "lcxmlwidgetcreatorinterface.h"

namespace xmlwidgetcreators
{

class LCXmlRemLineEdit : public LCXmlWidgetCreatorInterface
{
public:
    LCXmlWidgetOrderControlInterface* mpOrderCtrl;
public:
    LCXmlRemLineEdit();
    virtual ~LCXmlRemLineEdit();
    virtual QWidget* create(const QDomElement& _element) override;
};

}

#endif // LCXMLREMLINEEDIT_H
