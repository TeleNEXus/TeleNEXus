#ifndef LCXMLDATASOURCECREATEBASE_H
#define LCXMLDATASOURCECREATEBASE_H

#include <QMap>
#include <QSharedPointer>

#include "LCRemoteDataSourceInterface.h"

using LTDataSources = QMap<QString, QSharedPointer<LCRemoteDataSourceInterface>>;

class LCXmlRemoteDataSourceCreateBase
{
public:

public:
    LCXmlRemoteDataSourceCreateBase(){};
    virtual LTDataSources create(const QString& _xmlfilename) = 0;
};

#endif // LCXMLDATASOURCECREATEBASE_H
