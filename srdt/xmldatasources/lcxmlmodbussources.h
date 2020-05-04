#ifndef LCXMLMODEBUSSOURCES_H
#define LCXMLMODEBUSSOURCES_H

#include "LCXmlRemoteDataSourceCreatorInterface.h"

class LCXmlModbusSources : public LCXmlRemoteDataSourceCreatorInterface
{
public:
    LCXmlModbusSources();
    virtual LTDataSources create(const QDomElement &_element) override;
private:
};

#endif // LCXMLMODEBUSSOURCES_H
