#ifndef LCQMODBUSMASTERRTU_H
#define LCQMODBUSMASTERRTU_H

#include "lcmodbusmasterbase.h"
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

class QThread;

namespace modbus {

class LCQModbusMasterRtu : public LCModbusMasterBase
{
    Q_OBJECT

private:
    QString mPortName;
    QSerialPort::BaudRate mBaudRate;
    QSerialPort::Parity mParity;
    QSerialPort::DataBits mDataBits;
    QSerialPort::StopBits mStopBits;
    int mTimeout;
    QModbusRtuSerialMaster* mpMaster;
    QThread* mpThread;

private:
    LCQModbusMasterRtu() = delete;
    LCQModbusMasterRtu(const LCQModbusMasterRtu&) = delete;
    LCQModbusMasterRtu& operator=(const LCQModbusMasterRtu&) = delete;
    explicit LCQModbusMasterRtu(QObject* _parent = nullptr);
    virtual ~LCQModbusMasterRtu();

public:
    static QSharedPointer<LCQModbusMasterRtu> create();

public slots:
    void connectToPort( QString                 _portName,
                        QSerialPort::BaudRate   _baudRate,
                        QSerialPort::Parity     _parity,
                        QSerialPort::DataBits   _dataBits,
                        QSerialPort::StopBits   _stopBits,
                        int                     _timeout = 500);
    void disconnectFromPort();

protected:
    virtual QModbusClient* createMaster() override;
    virtual QModbusClient* getMaster() override;
};

}//namespace
#endif // LCQMODBUSMASTERRTU_H
