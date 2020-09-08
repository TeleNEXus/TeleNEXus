#ifndef LIREMOTEDATAREADLISTNER_H
#define LIREMOTEDATAREADLISTNER_H

#include "LIRemoteDataSource.h"

class QByteArray;

class LIRemoteDataReadListener
{
public:
    LIRemoteDataReadListener(){}
    virtual ~LIRemoteDataReadListener(){}
    virtual void dataIsRead(QSharedPointer<QByteArray> _data, LERemoteDataStatus status) = 0;
};

#endif // LIREMOTEDATAREADLISTNER_H
