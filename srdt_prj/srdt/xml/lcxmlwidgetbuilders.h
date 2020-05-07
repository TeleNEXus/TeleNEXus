#ifndef LCXMLWIDGETBUILDERS_H
#define LCXMLWIDGETBUILDERS_H

#include <QSharedPointer>
#include <QDomElement>

#include "LIXmlWidgetBuilder.h"
#include "lcxmlbuildersloader.h"

class LCXmlWidgetBuilders : public LCXmlBuildersLoader
{
private:
    LCXmlWidgetBuilders();
    LCXmlWidgetBuilders(const LCXmlWidgetBuilders&) = delete ;
    LCXmlWidgetBuilders& operator=(const LCXmlWidgetBuilders&) = delete;
public:
    static LCXmlWidgetBuilders& instance();
    QSharedPointer<LIXmlWidgetBuilder> getBuilder(const QString _name);
    virtual bool add(const QString &_name, void *_builder) override;
};

#endif // LCXMLWIDGETBUILDERS_H
