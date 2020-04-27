#ifndef LCMODBUSMASTERBASE_H
#define LCMODBUSMASTERBASE_H

#include <QModbusClient>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

namespace modbus
{

class LCModbusMasterBase : public QObject
{
    Q_OBJECT
public:

    struct SReply
    {
        enum class EStatus
        {
            UNDEF,
            OK,
            WRONG_REQ,
            WRONG_RESP,
            RESP_TIMEOUT,
            MODBUS_EXCEPTION,
            NO_CONNECTION,
            CONNECTION_ERROR,
            UNREC_ERROR
        };
        EStatus status;
        quint8  modbusExceptionCode;
        SReply() : status(EStatus::UNDEF), modbusExceptionCode(0){}
        SReply(EStatus _status, quint8 _except_code) : status(_status), modbusExceptionCode(_except_code){}
    };

private:

    QMutex          mMutexThread;
    QMutex          mMutexEvent;
    QWaitCondition  mWaitCond;
    QTimer          *mpTimer;

public:
    explicit LCModbusMasterBase(QObject* _parent = nullptr);
    virtual ~LCModbusMasterBase();

    virtual SReply readInputRegisters(quint8 _devId,
                                                  quint16 _addr, quint16 _regQuant, quint16 _regs[]) final;
    virtual SReply readHoldingRegisters(quint8 _devId,
                                                  quint16 _addr, quint16 _regQuant, quint16 _regs[]) final;

    virtual SReply writeSingleRegister(quint8 _devId, quint16 _addr, quint16 _reg) final;

    virtual SReply writeMultipleRegisters(quint8 _devId,
                                                    quint16 _addr, quint16 _regQuan, const quint16 _regs[]) final;

    virtual SReply readCoils(quint8 _devId, quint16 _startAddr, quint16 _coilsQuant, quint8 _bits[]) final;

    virtual SReply writeSingleCoils(quint8 _devId, quint16 _addr, quint8 _bit) final;

    virtual SReply writeMultipleCoils(quint8 _devId,
                                                    quint16 _addr, quint16 _regQuan, const quint8 _bits[]) final;

    virtual SReply readDiscreteInputs(quint8 _devId, quint16 _startAddr, quint16 _inputsQuant, quint8 _bits[]) final;

protected:
    void connectRequest();
    virtual QModbusClient* createMaster() = 0;
    virtual QModbusClient* getMaster() = 0;

private:
    virtual void customEvent(QEvent *event) final;
};

}// namespace modbus

#endif // LCMODBUSMASTERBASE_H
