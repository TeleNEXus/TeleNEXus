#ifndef LCXMLMODBUSSOURCEBUILDER_H
#define LCXMLMODBUSSOURCEBUILDER_H

#include "LIXmlRemoteDataSourceBuilder.h"

class LCXmlModbusSourceBuilder : public LIXmlRemoteDataSourceBuilder
{
public:
    LCXmlModbusSourceBuilder();
    virtual LQDataSources build(const QDomElement &_element, const LIApplication& _app) override;
};

#endif // LCXMLMODBUSSOURCEBUILDER_H
