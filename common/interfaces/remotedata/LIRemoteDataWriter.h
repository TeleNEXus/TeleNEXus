#ifndef LIREMOTEDATAWRITER_H
#define LIREMOTEDATAWRITER_H

/* #include <QWeakPointer> */

/* class LIRemoteDataSource; */

#include <QByteArray>

class LIRemoteDataWriter
{
public:
    LIRemoteDataWriter(){}
    virtual ~LIRemoteDataWriter(){}
    virtual void writeRequest( const QByteArray& _data) = 0;
};

#endif // LIREMOTEDATAWRITER_H
