#ifndef LIXMLREMOTEDATASOURCECREATOR_H
#define LIXMLREMOTEDATASOURCECREATOR_H

#include <QMap>
#include <QSharedPointer>

class LIRemoteDataSource;
class QDomElement;

using LQDataSources = QMap<QString, QSharedPointer<LIRemoteDataSource>>;

class LIXmlRemoteDataSourceCreator
{
public:
    LIXmlRemoteDataSourceCreator(){}
    virtual LQDataSources create(const QDomElement &_element) = 0;
};

#endif // LIXMLREMOTEDATASOURCECREATOR_H
