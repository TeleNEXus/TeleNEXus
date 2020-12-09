#ifndef LIREMOTEDATAWRITER_H
#define LIREMOTEDATAWRITER_H

#include <QWeakPointer>

class LIRemoteDataSource;
class LIRemoteDataWriteListener;

class LIRemoteDataWriter
{
public:
    LIRemoteDataWriter(){}
    virtual ~LIRemoteDataWriter(){}
    /* virtual void setDataName( const QString& _dataName) = 0; */
    /* virtual void setDataWriteListener( */ 
    /*         QWeakPointer<LIRemoteDataWriteListener> _listener) = 0; */
    virtual void writeRequest( const QByteArray& _data) = 0;
};

#endif // LIREMOTEDATAWRITER_H
