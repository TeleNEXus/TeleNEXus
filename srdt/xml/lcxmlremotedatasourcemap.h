#ifndef LCXMLREMOTEDATASOURCEMAP_H
#define LCXMLREMOTEDATASOURCEMAP_H

#include <QMap>
#include <QSharedPointer>
#include "LIRemoteDataSource.h"

#include "LIXmlRemoteDataSourceBuilder.h"

class LCXmlRemoteDataSourceMap
{
private:
    LQDataSources mMap;
private:
    static LCXmlRemoteDataSourceMap mInstance;
    LCXmlRemoteDataSourceMap();
    LCXmlRemoteDataSourceMap(const LCXmlRemoteDataSourceMap&) = delete;
    LCXmlRemoteDataSourceMap& operator=(const LCXmlRemoteDataSourceMap&) = delete;
public:
    static LCXmlRemoteDataSourceMap& instace();
    void addRemoteDataSorce(const QString& _name, QSharedPointer<LIRemoteDataSource> _source);
    void addRemoteDataSorce(const LQDataSources& _sources);
    QSharedPointer<LIRemoteDataSource> getRemoteDataSorce(const QString& _name);
};

#endif // LCXMLREMOTEDATASOURCEMAP_H
