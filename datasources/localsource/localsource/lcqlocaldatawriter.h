#ifndef LCQLOCALDATAWRITER_H
#define LCQLOCALDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class LCQLocalSourceHiden;

class LCQLocalDataWriter final : public QObject, public LIRemoteDataWriter
{
    Q_OBJECT
private:
    //--------------------------------------------------------------------------
    class CQEventDataIsWrite : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        const LERemoteDataStatus mStatus;
        explicit CQEventDataIsWrite(LERemoteDataStatus _status);
    };

    QString mDataName;
    QWeakPointer<LCQLocalSourceHiden> mwpDataSource;
    QWeakPointer<LIRemoteDataWriteListener> mwpWriteListener;
    QWeakPointer<LCQLocalDataWriter> mwpThis;

private:
    LCQLocalDataWriter();

public:
    static QSharedPointer<LCQLocalDataWriter> create(QSharedPointer<LCQLocalSourceHiden> _dataSource);

    virtual void setDataName(const QString& _dataName) override;
    virtual void setDataWriteListener(
                    QWeakPointer<LIRemoteDataWriteListener> _listener) override;
    virtual void writeRequest(const QByteArray& _data) override;

private:
    virtual void customEvent(QEvent *_event) override;
};


#endif // LCQLOCALDATAWRITER_H
