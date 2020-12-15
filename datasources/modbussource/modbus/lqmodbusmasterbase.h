/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LQMODBUSMASTERBASE_H
#define LQMODBUSMASTERBASE_H

#include <QModbusClient>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

class LQModbusMasterBase : public QObject
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
    explicit LQModbusMasterBase(QObject* _parent = nullptr);
    virtual ~LQModbusMasterBase();

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

#endif // LQMODBUSMASTERBASE_H
