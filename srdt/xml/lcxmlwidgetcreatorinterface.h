#ifndef LCXMLWIDGETCREATORINTERFACE_H
#define LCXMLWIDGETCREATORINTERFACE_H

#include <QDomElement>

class QWidget;
class LCXmlWidgetOrderControlInterface;

class LCXmlWidgetCreatorInterface
{
public:
    LCXmlWidgetCreatorInterface(){}
    virtual ~LCXmlWidgetCreatorInterface(){}
    virtual QWidget* create(const QDomElement& _element) = 0;
};

#endif // LCXMLWIDGETCREATORINTERFACE_H
