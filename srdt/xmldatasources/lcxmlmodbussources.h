#ifndef LCXMLMODEBUSSOURCES_H
#define LCXMLMODEBUSSOURCES_H

#include "LIXmlRemoteDataSourceCreator.h"

class LCXmlModbusSources : public LIXmlRemoteDataSourceCreator
{
public:
    LCXmlModbusSources();
    virtual LQDataSources create(const QDomElement &_element) override;
private:
};

#endif // LCXMLMODEBUSSOURCES_H
