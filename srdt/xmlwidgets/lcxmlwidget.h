#ifndef LCXMLWIDGET_H
#define LCXMLWIDGET_H

#include "lcxmlwidgetcreatorbase.h"

namespace xmlwidgetcreators
{

class LCXmlWidget : public LCXmlWidgetCreatorBase
{
public:
    LCXmlWidget();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLWIDGET_H
