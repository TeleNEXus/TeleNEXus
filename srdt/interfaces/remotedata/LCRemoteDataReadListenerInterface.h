#ifndef LCREMOTEDATAREADLISTNERINTERFACE_H
#define LCREMOTEDATAREADLISTNERINTERFACE_H

#include "LCRemoteDataSourceInterface.h"

class LCRemoteDataReadListenerInterface
{
public:
    LCRemoteDataReadListenerInterface(){}
    virtual void dataIsRead(QSharedPointer<QByteArray> _data, ERemoteDataStatus status) = 0;
};

#endif // LCREMOTEDATAREADLISTNERINTERFACE_H
