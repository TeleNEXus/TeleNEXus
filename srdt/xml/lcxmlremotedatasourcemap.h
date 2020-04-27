#ifndef LCXMLREMOTEDATASOURCEMAP_H
#define LCXMLREMOTEDATASOURCEMAP_H

#include <QMap>
#include <QSharedPointer>

class LCQRemoteDataSourceBase;

#include "xmldatasources/lcxmlremotedatasourcecreatebase.h"

class LCXmlRemoteDataSourceMap
{
private:
    QMap<QString, QSharedPointer<LCQRemoteDataSourceBase>> mMap;
private:
    static LCXmlRemoteDataSourceMap mInstance;
    LCXmlRemoteDataSourceMap();
    LCXmlRemoteDataSourceMap(const LCXmlRemoteDataSourceMap&) = delete;
    LCXmlRemoteDataSourceMap& operator=(const LCXmlRemoteDataSourceMap&) = delete;
public:
    static LCXmlRemoteDataSourceMap& instace();
    void addRemoteDataSorce(const QString& _name, QSharedPointer<LCQRemoteDataSourceBase> _source);
    void addRemoteDataSorce(const LCRemoteDataSourceMap& _sources);
    QSharedPointer<LCQRemoteDataSourceBase> getRemoteDataSorce(const QString& _name);

};

#endif // LCXMLREMOTEDATASOURCEMAP_H
