#ifndef LCXMLREMOTEDATASOURCEMAP_H
#define LCXMLREMOTEDATASOURCEMAP_H

#include <QMap>
#include <QSharedPointer>
#include "LCRemoteDataSourceInterface.h"

#include "LCXmlRemoteDataSourceCreatorInterface.h"

class LCXmlRemoteDataSourceMap
{
private:
    LTDataSources mMap;
private:
    static LCXmlRemoteDataSourceMap mInstance;
    LCXmlRemoteDataSourceMap();
    LCXmlRemoteDataSourceMap(const LCXmlRemoteDataSourceMap&) = delete;
    LCXmlRemoteDataSourceMap& operator=(const LCXmlRemoteDataSourceMap&) = delete;
public:
    static LCXmlRemoteDataSourceMap& instace();
    void addRemoteDataSorce(const QString& _name, QSharedPointer<LCRemoteDataSourceInterface> _source);
    void addRemoteDataSorce(const LTDataSources& _sources);
    QSharedPointer<LCRemoteDataSourceInterface> getRemoteDataSorce(const QString& _name);
};

#endif // LCXMLREMOTEDATASOURCEMAP_H
