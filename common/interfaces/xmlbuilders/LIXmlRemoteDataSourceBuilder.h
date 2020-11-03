#ifndef LIXMLREMOTEDATASOURCEBUILDER_H
#define LIXMLREMOTEDATASOURCEBUILDER_H

#include <QMap>
#include <QSharedPointer>

class LIRemoteDataSource;
class LIApplication;
class QDomElement;

using LQDataSources = QMap<QString, QSharedPointer<LIRemoteDataSource>>;

class LIXmlRemoteDataSourceBuilder
{
public:
    LIXmlRemoteDataSourceBuilder(){}
    virtual ~LIXmlRemoteDataSourceBuilder(){}
    virtual LQDataSources build(
            const QDomElement &_element, 
            const LIApplication& _app) = 0;
};

#endif // LIXMLREMOTEDATASOURCEBUILDER_H
