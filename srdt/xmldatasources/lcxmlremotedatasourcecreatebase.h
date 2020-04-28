#ifndef LCXMLDATASOURCECREATEBASE_H
#define LCXMLDATASOURCECREATEBASE_H

#include <QMap>
#include <QSharedPointer>

#include "LCRemoteDataSourceInterface.h"

using LCRemoteDataSourceMap = QMap<QString, QSharedPointer<LCRemoteDataSourceInterface>>;
class LCXmlRemoteDataSourceCreateBase
{

public:
    LCXmlRemoteDataSourceCreateBase();
    virtual LCRemoteDataSourceMap create(const QString& _xmlfilename) = 0;
};

#endif // LCXMLDATASOURCECREATEBASE_H
