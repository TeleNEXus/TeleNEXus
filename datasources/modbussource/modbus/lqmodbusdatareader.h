#ifndef LQMODBUSDATAREADER_H
#define LQMODBUSDATAREADER_H

#include "lqextendevent.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"

namespace modbus {

class LQModbusDataSource;

class LQModbusDataReader : public QObject, public LIRemoteDataReader
{
    Q_OBJECT
private:

    //---------------------------------------------------------------------------------------------------CQEventDataRead
    class CQEventDataIsRead : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        QSharedPointer<QByteArray> mData;
        LERemoteDataStatus mStatus;
        explicit CQEventDataIsRead(const QByteArray& _data, LERemoteDataStatus _status);
        explicit CQEventDataIsRead(LERemoteDataStatus _status);
    };

    QString mDataName;
    QWeakPointer<LIRemoteDataReadListener> mwpReadListener;
    QWeakPointer<LIRemoteDataSource> mDataSource;

public:

    virtual ~LQModbusDataReader();
    /* virtual void setDataName(const QString& _dataName) override; */
    /* virtual void setDataReadListener(QWeakPointer<LIRemoteDataReadListener> _listener) override; */
    virtual void readRequest() override;
    virtual void connectToSource() override;
    virtual void disconnectFromSource() override;
private:
    explicit LQModbusDataReader() = delete;

    explicit LQModbusDataReader(  
        const QString& _dataName,
        QWeakPointer<LIRemoteDataReadListener> _readListener,
        QWeakPointer<LQModbusDataSource> _dataSource);
 
    virtual void customEvent(QEvent* _event) override;
    friend class LQModbusDataSource;
};

} //namespace
#endif // LQMODBUSDATAREADER_H
