#ifndef LCXMLREMOTEDATASOURCECREATORINTERFACE_H
#define LCXMLREMOTEDATASOURCECREATORINTERFACE_H

#include <QMap>
#include <QSharedPointer>

class LCRemoteDataSourceInterface;
class QDomElement;

using LTDataSources = QMap<QString, QSharedPointer<LCRemoteDataSourceInterface>>;

class LCXmlRemoteDataSourceCreatorInterface
{
public:
    LCXmlRemoteDataSourceCreatorInterface(){}
    virtual LTDataSources create(const QDomElement &_element) = 0;
};

#endif // LCXMLREMOTEDATASOURCECREATORINTERFACE_H
