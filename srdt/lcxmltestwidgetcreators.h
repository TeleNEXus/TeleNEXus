#ifndef LCXMLTESTWIDGETCREATORS_H
#define LCXMLTESTWIDGETCREATORS_H

#include "lcxmlwidgetcreatorinterface.h"
#include <QLabel>

namespace xmltestwidget
{

class LCXmlWidget : public LCXmlWidgetCreatorInterface
{
public:
    LCXmlWidget();
    virtual QWidget* create(const QDomElement& _element) override;
};

class LCXmlWidgetLabel : public LCXmlWidgetCreatorInterface
{
public:
    LCXmlWidgetLabel();
    virtual QWidget* create(const QDomElement& _element) override;
};

}

#endif // LCXMLTESTWIDGETCREATORS_H
