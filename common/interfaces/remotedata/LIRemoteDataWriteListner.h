#ifndef LIREMOTEDATAWRITELISTNER_H
#define LIREMOTEDATAWRITELISTNER_H

#include "LIRemoteDataSource.h"

class LIRemoteDataWriteListner
{
public:
    LIRemoteDataWriteListner(){}
    virtual void dataIsWrite(LERemoteDataStatus _status) = 0;
};

#endif // LIREMOTEDATAWRITELISTNER_H
