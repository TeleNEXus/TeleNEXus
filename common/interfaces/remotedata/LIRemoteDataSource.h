#ifndef LIREMOTEDATASOURCE_H
#define LIREMOTEDATASOURCE_H

#include <QSharedPointer>

enum class LERemoteDataStatus
{
    DS_UNDEF,
    DS_OK,
    DS_WRONG
};

class LIRemoteDataReader;
class LIRemoteDataWriter;

class LIRemoteDataSource
{
public:
    LIRemoteDataSource(){}
    virtual ~LIRemoteDataSource(){}
    virtual QSharedPointer<LIRemoteDataReader> createReader() = 0;
    virtual QSharedPointer<LIRemoteDataWriter> createWriter() = 0;
};

#endif // LIREMOTEDATASOURCE_H
