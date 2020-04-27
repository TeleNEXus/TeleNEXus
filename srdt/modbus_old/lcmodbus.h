/***********************************************************************************************************************
 * FILE NAME:       lcmodbus.h
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:
 * ********************************************************************************************************************/
#ifndef LCMODBUS_H
#define LCMODBUS_H

#include "ltypedef.h"

namespace modbus {

using namespace lstd;


class LCModbus
{
public:

    enum class EMasterOperationStatus
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

    struct STcpMbap
    {
        TUint16 transactionId;
        TUint16 length;
        TUint16 unitId;
        STcpMbap(   TUint16 _transactionId,
                    TUint16 _length,
                    TUint16 _unitId) :  transactionId(_transactionId),
                                        length(_length),
                                        unitId(_unitId){}
    };

    enum class EMasterPduError
    {
        MASTER_PDU_NO_ERROR,
        MASTER_PDU_WRONG_MESSAGE,
        MASTER_PDU_MODBUS_EXCEPTION
    };

    struct SMasterPduStatus
    {
        EMasterPduError error;
        TUint8          exceptionCode;
        TUint16         dataLength;
    };

public:
    static LCModbus::SMasterPduStatus masterReadPduError(TUint8 _pduBuff[], TUint8 _fcode);

    static TUint16 masterWritePduReadHoldingRegisters(TUint8 _pdubuff[], TUint16 _startAddr, TUint16 _regQuant);

    static SMasterPduStatus masterReadPduReadHoldingRegisters(  TUint8 _pduBuff[],
                                                                TUint16 _regsBuff[],
                                                                TUint16 _regQuant);

    static TUint16 masterWritePduWriteSingleRegister(  TUint8 _pduBuff[], TUint16 _addr, TUint16 _reg);
    static SMasterPduStatus masterReadPduWriteSingleRegister(  TUint8 _pduBuff[], TUint16 _addr, TUint16 _reg);

    static TUint16 masterWritePduWriteMultipleRegisters(  TUint8 _pdubuff[],
                                                          TUint16 _startAddr,
                                                          TUint16 _regQuant,
                                                          const TUint16 _regsBuff[]);
    static SMasterPduStatus masterReadPduWriteMultipleRegisters(  TUint8 _pduBuff[],
                                                                  TUint16 _startAddr,
                                                                  TUint16 _regQuant);

    static TUint16 writeTcpMbap(   TUint16 _transactionId,
                                    TUint16 _length,
                                    TUint8  _unitId,
                                    TUint8  _adubuff[]);

    static bool checkTcpMbap( TUint16  _transactionId,
                              TUint16  _length,
                              TUint8   _unitId,
                              const TUint8 _adubuff[]);

    static void copyBytesToRegs(const TUint8 _bytes[], TUint16 _regs[], TUint16 _regQuant);
    static void copyRegsToBytes(const TUint16 _regs[], TUint8 _bytes[], TUint16 _regQuant);
private:
    LCModbus();
    LCModbus(const LCModbus&);
};

}
#endif // LCMODBUS_H
