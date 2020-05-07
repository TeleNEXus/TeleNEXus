#ifndef LCXMLSOURCEBUILDERLOADER_H
#define LCXMLSOURCEBUILDERLOADER_H

#include <QSharedPointer>
#include <QDomElement>

#include "lcxmlbuildersloader.h"
#include "LIXmlRemoteDataSourceBuilder.h"


class LCXmlRemoteDataSourceBuilders : public LCXmlBuildersLoader
{
public:

private:
    LCXmlRemoteDataSourceBuilders();
    LCXmlRemoteDataSourceBuilders(const LCXmlRemoteDataSourceBuilders&) = delete;
    LCXmlRemoteDataSourceBuilders& operator=(const LCXmlRemoteDataSourceBuilders&) = delete;
    virtual ~LCXmlRemoteDataSourceBuilders(){}
public:
    static LCXmlRemoteDataSourceBuilders& instance();
    QSharedPointer<LIXmlRemoteDataSourceBuilder> getBuilder(const QString _name);
    virtual bool add(const QString &_name, void *_builder) override;
};

#endif // LCXMLSOURCEBUILDERLOADER_H
