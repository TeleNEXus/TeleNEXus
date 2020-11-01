#ifndef LQMODBUSDATAWRITER_H
#define LQMODBUSDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

namespace modbus
{
class LQModbusDataSource;
class LQModbusDataWriter : public QObject, public LIRemoteDataWriter
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
    QWeakPointer<LIRemoteDataSource> mDataSource;
    QWeakPointer<LIRemoteDataWriteListner> mwpWriteListener;

private:
    explicit LQModbusDataWriter(QObject* _parent = nullptr) = delete;

    explicit LQModbusDataWriter(QWeakPointer<LQModbusDataSource> _dataSource, 
            QObject* _parent = nullptr);
public:
    virtual void setDataName(const QString& _dataName) override;
    /* virtual void setDataSource( */
    /*                 QWeakPointer<LIRemoteDataSource> _source) override; */
    virtual void setDataWriteListener(
                    QWeakPointer<LIRemoteDataWriteListner> _listener) override;
    virtual void writeRequest(const QByteArray& _data) override;

private:
    virtual void customEvent(QEvent *_event) override;

    friend class LQModbusDataSource;
};

}

#endif // LQMODBUSDATAWRITER_H
