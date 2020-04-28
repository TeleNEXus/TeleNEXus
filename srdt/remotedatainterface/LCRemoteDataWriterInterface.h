#ifndef LCREMOTEDATAWRITERINTERFACE_H
#define LCREMOTEDATAWRITERINTERFACE_H

#include <QWeakPointer>

class LCRemoteDataSourceInterface;
class LCRemoteDataWriteListnerInterface;

class LCRemoteDataWriterInterface
{
public:
    LCRemoteDataWriterInterface(){}

    virtual void setDataName( const QString& _dataName) = 0;
    virtual void setDataSource(QWeakPointer<LCRemoteDataSourceInterface> _source) = 0;
    virtual void setDataWriteListener( QWeakPointer<LCRemoteDataWriteListnerInterface> _listener) = 0;
    virtual void writeRequest( const QByteArray& _data) = 0;
};

#endif // LCREMOTEDATAWRITERINTERFACE_H
