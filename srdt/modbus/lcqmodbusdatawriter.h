#ifndef LCQMODBUSDATAWRITER_H
#define LCQMODBUSDATAWRITER_H

#include "lqextendevent.h"
#include "LCRemoteDataWriterInterface.h"
#include "LCRemoteDataSourceInterface.h"

namespace modbus
{

class LCQModbusDataWriter : public QObject, public LCRemoteDataWriterInterface
{
    Q_OBJECT
private:
    //------------------------------------------------------------------------------------------------------------------
    class CQEventDataIsWrite : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        const ERemoteDataStatus mStatus;
        explicit CQEventDataIsWrite(ERemoteDataStatus _status);
    };

    QString mDataName;
    QWeakPointer<LCRemoteDataSourceInterface> mDataSource;
    QWeakPointer<LCRemoteDataWriteListnerInterface> mwpWriteListener;

public:
    explicit LCQModbusDataWriter(QObject* _parent = nullptr);
public:
    virtual void setDataName(const QString& _dataName) override;
    virtual void setDataSource(QWeakPointer<LCRemoteDataSourceInterface> _source) override;
    virtual void setDataWriteListener(QWeakPointer<LCRemoteDataWriteListnerInterface> _listener) override;
    virtual void writeRequest(const QByteArray& _data) override;

private:
    virtual void customEvent(QEvent *_event) override;

    friend class LCQModbusDataSource;
};

}

#endif // LCQMODBUSDATAWRITER_H
