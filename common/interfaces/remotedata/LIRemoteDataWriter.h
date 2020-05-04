#ifndef LIREMOTEDATAWRITER_H
#define LIREMOTEDATAWRITER_H

#include <QWeakPointer>

class LIRemoteDataSource;
class LIRemoteDataWriteListner;

class LIRemoteDataWriter
{
public:
    LIRemoteDataWriter(){}

    virtual void setDataName( const QString& _dataName) = 0;
    virtual void setDataSource(QWeakPointer<LIRemoteDataSource> _source) = 0;
    virtual void setDataWriteListener( QWeakPointer<LIRemoteDataWriteListner> _listener) = 0;
    virtual void writeRequest( const QByteArray& _data) = 0;
};

#endif // LIREMOTEDATAWRITER_H
