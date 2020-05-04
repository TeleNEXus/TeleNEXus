#ifndef LQMODBUSMASTERRTU_H
#define LQMODBUSMASTERRTU_H

#include "lqmodbusmasterbase.h"
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

class QThread;

namespace modbus {

class LQModbusMasterRtu : public LQModbusMasterBase
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
    LQModbusMasterRtu() = delete;
    LQModbusMasterRtu(const LQModbusMasterRtu&) = delete;
    LQModbusMasterRtu& operator=(const LQModbusMasterRtu&) = delete;
    explicit LQModbusMasterRtu(QObject* _parent = nullptr);
    virtual ~LQModbusMasterRtu();

public:
    static QSharedPointer<LQModbusMasterRtu> create();

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
#endif // LQMODBUSMASTERRTU_H
