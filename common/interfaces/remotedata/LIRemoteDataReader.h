#ifndef LIREMOTEDATAREADER_H
#define LIREMOTEDATAREADER_H

class LIRemoteDataReader
{
public:
    LIRemoteDataReader(){}
    ~LIRemoteDataReader(){}
    virtual void readRequest() = 0;
    virtual void connectToSource() = 0;
    virtual void disconnectFromSource() = 0;
};

#endif // LIREMOTEDATAREADER_H
