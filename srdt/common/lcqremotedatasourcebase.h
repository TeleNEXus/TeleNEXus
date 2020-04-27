#ifndef LCQREMOTEDATASOURCEBASE_H
#define LCQREMOTEDATASOURCEBASE_H

#include <QString>
#include <QByteArray>
#include <QEvent>
#include <QSharedPointer>
#include "lqextendevent.h"

enum class ERemoteDataStatus
{
    DS_UNDEF,
    DS_OK,
    DS_WRONG
};

class LCRemoteDataReadListnerInterface
{
public:
    LCRemoteDataReadListnerInterface(){}
    virtual void dataIsRead(ERemoteDataStatus status, const QByteArray& _data);
};

class LCRemoteDataWriteListnerInterface
{
public:
    LCRemoteDataWriteListnerInterface(){}
    virtual void dataIsWrite(const QByteArray& _data);
};

class LCRemoteDataReaderInterface
{
public:
    LCRemoteDataReaderInterface(){}
};

class LCRemoteDataWriterInterface
{
public:
    LCRemoteDataWriterInterface(){}
};

class LCQRemoteDataSourceInterface
{
public:
    LCQRemoteDataSourceInterface(){}
    virtual LCRemoteDataReaderInterface* createReader();
    virtual LCRemoteDataReaderInterface* createWriter();
};


class LCQRemoteDataSourceBase
{
public:

    enum class EDataStatus
    {
        DS_UNDEF,
        DS_OK,
        DS_WRONG,
    };

protected:
    //---------------------------------------------------------------------------------------------------CQEventDataRead
    class CQEventDataIsRead : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        QSharedPointer<QByteArray> mData;
        EDataStatus mStatus;
        explicit CQEventDataIsRead(const QByteArray& _data, EDataStatus _status);
        explicit CQEventDataIsRead(EDataStatus _status);
    };

    //--------------------------------------------------------------------------------------------------CQEventDataWrite
    class CQEventDataIsWrite : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        const EDataStatus mStatus;
        explicit CQEventDataIsWrite(EDataStatus _status);
    };

    LCQRemoteDataSourceBase(){}

protected:
    virtual void connectReader(const QString& _dataName, QObject* _reader) = 0;
    virtual void disconnectReader(QObject* _reader) = 0;
    virtual void read(const QString& _dataName, QObject* _reader)  = 0;
    virtual void write(const QString& _dataName, const QByteArray& _data, QObject* _writer = nullptr) = 0;

    friend class LCQRemoteDataReader;
    friend class LCQRemoteDataWriter;
};

#endif // LCQREMOTEDATASOURCEBASE_H
