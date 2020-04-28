#ifndef LCREMOTEDATAREADLISTNERINTERFACE_H
#define LCREMOTEDATAREADLISTNERINTERFACE_H

#include "LCRemoteDataSourceInterface.h"

class LCRemoteDataReadListnerInterface
{
public:
    LCRemoteDataReadListnerInterface(){}
    virtual void dataIsRead(QSharedPointer<QByteArray> _data, ERemoteDataStatus status) = 0;
};

#endif // LCREMOTEDATAREADLISTNERINTERFACE_H
