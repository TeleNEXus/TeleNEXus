#ifndef LIREMOTEDATAREADER_H
#define LIREMOTEDATAREADER_H

#include <QWeakPointer>

class LIRemoteDataSource;
class LIRemoteDataReadListener;

class LIRemoteDataReader
{
public:
    LIRemoteDataReader(){}
    ~LIRemoteDataReader(){}
    virtual void setDataName(const QString& _dataName) = 0;
    virtual void setDataReadListener(
            QWeakPointer<LIRemoteDataReadListener> _listener) = 0;
    virtual void readRequest() = 0;
    virtual void connectToSource() = 0;
    virtual void disconnectFromSource() = 0;
};

#endif // LIREMOTEDATAREADER_H
