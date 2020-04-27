#ifndef LCXMLTESTWIDGETCREATORS_H
#define LCXMLTESTWIDGETCREATORS_H

#include "lcxmlwidgetcreatorbase.h"
#include <QLabel>

namespace xmltestwidget
{

class LCXmlWidget : public LCXmlWidgetCreatorBase
{
public:
    LCXmlWidget();
    virtual QWidget* create(const QDomElement& _element) override;
};

class LCXmlWidgetLabel : public LCXmlWidgetCreatorBase
{
public:
    LCXmlWidgetLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

}

#endif // LCXMLTESTWIDGETCREATORS_H
