#ifndef LCREMOTEDATAWRITELISTNERINTERFACE_H
#define LCREMOTEDATAWRITELISTNERINTERFACE_H

#include "LCRemoteDataSourceInterface.h"

class LCRemoteDataWriteListnerInterface
{
public:
    LCRemoteDataWriteListnerInterface(){}
    virtual void dataIsWrite(ERemoteDataStatus _status) = 0;
};

#endif // LCREMOTEDATAWRITELISTNERINTERFACE_H
