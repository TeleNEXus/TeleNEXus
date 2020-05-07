#ifndef LIXMLWIDGETBUILDER_H
#define LIXMLWIDGETBUILDER_H

#include "LIApplication.h"
#include <QSharedPointer>
#include <QDomElement>

class QWidget;

class LIXmlWidgetBuilder
{
public:
    LIXmlWidgetBuilder(){}
    virtual ~LIXmlWidgetBuilder(){}
    virtual QSharedPointer<QWidget> build(const QDomElement& _element, const LIApplication& _app) = 0;
};

#endif // LIXMLWIDGETBUILDER_H
