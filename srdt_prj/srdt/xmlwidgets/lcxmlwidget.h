#ifndef LCXMLWIDGET_H
#define LCXMLWIDGET_H

#include "lcxmlwidgetcreatorinterface.h"

namespace xmlwidgetcreators
{

class LCXmlWidget : public LCXmlWidgetCreatorInterface
{
private:
    LCXmlWidgetOrderControlInterface* mpOrderCtrl;
public:
    LCXmlWidget();
    virtual QWidget* create(const QDomElement& _element) override;
};

} //namespace

#endif // LCXMLWIDGET_H
