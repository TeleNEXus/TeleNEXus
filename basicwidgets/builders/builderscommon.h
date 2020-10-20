#ifndef BUILDERSCOMMON_H_
#define BUILDERSCOMMON_H_

#include <QString>
#include <QSharedPointer>

class LIRemoteDataSource;
class LIApplication;
class QDomNamedNodeMap;

class CBuildersCommonAttributes
{
public:
    struct SSourceAttr
    {
        QString dataSourceName      = "sourceName";
        QString dataReadName        = "dataNameRead";
        QString dataWriteName       = "dataNameWrite";
        QString dataReadWriteName   = "dataName";
    };
    static const SSourceAttr mSourceAttr;
private:
    CBuildersCommonAttributes(){}
};

#endif /* BUILDERSCOMMON_H_ */
