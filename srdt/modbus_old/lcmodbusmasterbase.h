#ifndef LCMODBUSMASTERBASE_H
#define LCMODBUSMASTERBASE_H

#include "lcmodbus.h"

namespace modbus
{

class LCModbusMasterBase
{
public:
    using EOperationStatus = LCModbus::EMasterOperationStatus;

public:
    LCModbusMasterBase();
    virtual EOperationStatus readHoldingRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) = 0;

    virtual EOperationStatus writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg) = 0;

    virtual EOperationStatus writeMultipleRegisters(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuan, const TUint16 _regs[]) = 0;
};

}// namespace modbus

#endif // LCMODBUSMASTERBASE_H
