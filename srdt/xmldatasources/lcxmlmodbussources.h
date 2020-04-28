#ifndef LCXMLMODEBUSSOURCES_H
#define LCXMLMODEBUSSOURCES_H

#include "lcxmlremotedatasourcecreatebase.h"

class LCXmlModbusSources : public LCXmlRemoteDataSourceCreateBase
{
public:
    LCXmlModbusSources();
    virtual LTDataSources create(const QString &_xmlfilename) override;
private:
};

#endif // LCXMLMODEBUSSOURCES_H
