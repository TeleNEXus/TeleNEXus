#ifndef LCXMLWIDGETBUILDERS_H
#define LCXMLWIDGETBUILDERS_H

#include <QSharedPointer>
#include <QDomElement>

#include "LIXmlWidgetBuilder.h"

class LCXmlWidgetBuilders
{
private:
    LCXmlWidgetBuilders();
    LCXmlWidgetBuilders(const LCXmlWidgetBuilders&) = delete ;
    LCXmlWidgetBuilders& operator=(const LCXmlWidgetBuilders&) = delete;
public:
    static LCXmlWidgetBuilders& instance();
    QSharedPointer<LIXmlWidgetBuilder> getBuilder(const QString _name);
    int load(const QDomElement& _element);
};

#endif // LCXMLWIDGETBUILDERS_H
