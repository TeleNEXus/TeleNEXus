#ifndef LCQMODBUSDATAWRITER_H
#define LCQMODBUSDATAWRITER_H

#include "LCRemoteDataWriterInterface.h"
#include "lcqmodbusdatasource.h"

namespace modbus
{

class LCQModbusDataWriter : public QObject, public LCRemoteDataWriterInterface
{
    Q_OBJECT
private:
    QString mDataName;
    QWeakPointer<LCQModbusDataSource> mDataSource;
    QWeakPointer<LCRemoteDataWriteListnerInterface> mwpWriteListener;
public:

    explicit LCQModbusDataWriter(QObject* _parent = nullptr);

    virtual void setDataName(const QString& _dataName) override;
    virtual void setDataWriteListener(QWeakPointer<LCRemoteDataWriteListnerInterface> _listener) override;
    virtual void writeRequest(const QByteArray& _data) override;
    virtual void customEvent(QEvent *_event) override;

    friend class LCQModbusDataSource;
};

}

#endif // LCQMODBUSDATAWRITER_H
