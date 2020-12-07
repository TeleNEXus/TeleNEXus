#ifndef LCXMLLOCALSOURCEBUILDER_H
#define LCXMLLOCALSOURCEBUILDER_H

#include "LIXmlRemoteDataSourceBuilder.h"

class LCXmlLocalSourceBuilder : public LIXmlRemoteDataSourceBuilder
{
public:
    LCXmlLocalSourceBuilder();
    virtual LQDataSources build(
        const QDomElement &_element, 
        const LIApplication& _app) override;
};

#endif // LCXMLLOCALSOURCEBUILDER_H
