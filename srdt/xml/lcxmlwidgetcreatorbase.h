#ifndef LCXMLWIDGETFACTORYBASE_H
#define LCXMLWIDGETFACTORYBASE_H

#include <QDomElement>

class QWidget;

class LCXmlWidgetCreatorBase
{
public:
    LCXmlWidgetCreatorBase();
    virtual ~LCXmlWidgetCreatorBase();
    virtual QWidget* create(const QDomElement& _element) = 0;
};

#endif // LCXMLWIDGETFACTORYBASE_H
