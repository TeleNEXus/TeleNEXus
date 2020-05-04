#ifndef LCREMOTEDATASOURCEINTERFACE_H
#define LCREMOTEDATASOURCEINTERFACE_H

#include <QSharedPointer>

enum class ERemoteDataStatus
{
    DS_UNDEF,
    DS_OK,
    DS_WRONG
};

class LCRemoteDataReaderInterface;
class LCRemoteDataWriterInterface;

class LCRemoteDataSourceInterface
{
public:
    LCRemoteDataSourceInterface(){}
    virtual QSharedPointer<LCRemoteDataReaderInterface> createReader() = 0;
    virtual QSharedPointer<LCRemoteDataWriterInterface> createWriter() = 0;
};

#endif // LCREMOTEDATASOURCEINTERFACE_H
