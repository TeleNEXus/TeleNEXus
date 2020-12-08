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
    QWeakPointer<LCQLocalSourceHiden> mspDataSource;
    QWeakPointer<LIRemoteDataWriteListener> mwpWriteListener;

private:
    LCQLocalDataWriter(QWeakPointer<LCQLocalSourceHiden> _dataSource);
public:
    virtual void setDataName(const QString& _dataName) override;
    virtual void setDataWriteListener(
                    QWeakPointer<LIRemoteDataWriteListener> _listener) override;
    virtual void writeRequest(const QByteArray& _data) override;

private:
    virtual void customEvent(QEvent *_event) override;

    friend class LCQLocalSourceHiden;
};


#endif // LCQLOCALDATAWRITER_H
