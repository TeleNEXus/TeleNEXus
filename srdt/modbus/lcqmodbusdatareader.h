#ifndef LCQMODBUSDATAREADER_H
#define LCQMODBUSDATAREADER_H

#include "lqextendevent.h"
#include "LCRemoteDataSourceInterface.h"
#include "LCRemoteDataReaderInterface.h"

namespace modbus {

class LCQModbusDataReader : public QObject, public LCRemoteDataReaderInterface
{
    Q_OBJECT
private:

    //---------------------------------------------------------------------------------------------------CQEventDataRead
    class CQEventDataIsRead : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        QSharedPointer<QByteArray> mData;
        ERemoteDataStatus mStatus;
        explicit CQEventDataIsRead(const QByteArray& _data, ERemoteDataStatus _status);
        explicit CQEventDataIsRead(ERemoteDataStatus _status);
    };

    QString mDataName;
    QWeakPointer<LCRemoteDataSourceInterface> mDataSource;
    QWeakPointer<LCRemoteDataReadListnerInterface> mpReadListener;

public:

    LCQModbusDataReader(  QObject* _parent = nullptr);
    virtual ~LCQModbusDataReader();
    virtual void setDataName(const QString& _dataName) override;
    virtual void setDataSource(QWeakPointer<LCRemoteDataSourceInterface> _source) override;
    virtual void setDataReadListener(QWeakPointer<LCRemoteDataReadListnerInterface> _listener) override;
    virtual void readRequest() override;
    virtual void connectToSource() override;
    virtual void disconnectFromSource() override;
private:
    virtual void customEvent(QEvent* _event) override;

    friend class LCQModbusDataSource;
};

} //namespace
#endif // LCQMODBUSDATAREADER_H
