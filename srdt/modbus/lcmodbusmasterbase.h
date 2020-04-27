#ifndef LCMODBUSMASTERBASE_H
#define LCMODBUSMASTERBASE_H

#include "ltypedef.h"
#include <QModbusClient>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

namespace modbus
{

using namespace lstd;

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
        TUint8  modbusExceptionCode;
        SReply() : status(EStatus::UNDEF), modbusExceptionCode(0){}
        SReply(EStatus _status, TUint8 _except_code) : status(_status), modbusExceptionCode(_except_code){}
    };

private:

    QMutex          mMutexThread;
    QMutex          mMutexEvent;
    QWaitCondition  mWaitCond;
    QTimer          *mpTimer;

public:
    explicit LCModbusMasterBase(QObject* _parent = nullptr);
    virtual ~LCModbusMasterBase();

    virtual SReply readInputRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) final;
    virtual SReply readHoldingRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) final;

    virtual SReply writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg) final;

    virtual SReply writeMultipleRegisters(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuan, const TUint16 _regs[]) final;

    virtual SReply readCoils(TUint8 _devId, TUint16 _startAddr, TUint16 _coilsQuant, TUint8 _bits[]) final;

    virtual SReply writeSingleCoils(TUint8 _devId, TUint16 _addr, TUint8 _bit) final;

    virtual SReply writeMultipleCoils(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuan, const TUint8 _bits[]) final;

    virtual SReply readDiscreteInputs(TUint8 _devId, TUint16 _startAddr, TUint16 _inputsQuant, TUint8 _bits[]) final;

protected:
    void connectRequest();
    virtual QModbusClient* createMaster() = 0;
    virtual QModbusClient* getMaster() = 0;

private:
    virtual void customEvent(QEvent *event) final;
};

}// namespace modbus

#endif // LCMODBUSMASTERBASE_H
