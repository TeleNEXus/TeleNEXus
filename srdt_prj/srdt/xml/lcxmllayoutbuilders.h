#ifndef LCXMLLAYOUTBUILDERS_H
#define LCXMLLAYOUTBUILDERS_H

#include <QSharedPointer>
#include <QDomElement>

#include "lcxmlbuildersloader.h"
#include "LIXmlLayoutBuilder.h"

class LCXmlLayoutBuilders  : public LCXmlBuildersLoader
{
private:
    LCXmlLayoutBuilders();
    LCXmlLayoutBuilders(const LCXmlLayoutBuilders&) = delete ;
    LCXmlLayoutBuilders& operator=(const LCXmlLayoutBuilders&) = delete;
public:
    static LCXmlLayoutBuilders& instance();
    QSharedPointer<LIXmlLayoutBuilder> getBuilder(const QString _name);
    virtual bool add(const QString &_name, void *_builder) override;
};

#endif // LCXMLLAYOUTBUILDERS_H
