#ifndef LCXMLWIDGET_H
#define LCXMLWIDGET_H

#include "LIXmlWidgetBuilder.h"

class LCXmlWidget : public LIXmlWidgetBuilder
{
public:
    LCXmlWidget();
    virtual QWidget* build(const QDomElement& _element, const LIApplication& _app) override;
};

#endif // LCXMLWIDGET_H
