/***********************************************************************************************************************
 * FILE NAME:       lcmodbus.cpp
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:
 * ********************************************************************************************************************/
#include "lcmodbus.h"
#include "lmodbusdefs.h"

using namespace modbus;

LCModbus::SMasterPduStatus LCModbus::masterReadPduError(TUint8 _pduBuff[], TUint8 _fcode)
{
    const TUint8& error_code = _pduBuff[0];
    if( ( (error_code - _fcode) != MODBUS_EXCEPTION_FLAG) ||
        ( (error_code - MODBUS_EXCEPTION_FLAG) != _fcode ) )
    {
        return {EMasterPduError::MASTER_PDU_WRONG_MESSAGE, MBEC_NO_EXCEPTION, 0};
    }
    return {EMasterPduError::MASTER_PDU_MODBUS_EXCEPTION, _pduBuff[1], 2};
}

TUint16 LCModbus::masterWritePduReadHoldingRegisters(TUint8 _databuff[], TUint16 _startAddr, TUint16 _regQuant)
{
    if(_regQuant == 0) return 0;
    _databuff[0] = MBFC_READ_HOLDING_REGISTERS;
    MB_REG_TO_BYTES(_startAddr, &_databuff[1]);
    MB_REG_TO_BYTES(_regQuant, &_databuff[3]);
    return 5;
}

LCModbus::SMasterPduStatus
    LCModbus::masterReadPduReadHoldingRegisters(TUint8 _pduBuff[], TUint16 _regsBuff[], TUint16 _regQuant)
{
    if(_pduBuff[0] != MBFC_READ_HOLDING_REGISTERS)
    {
        //Обработка ошибок.
        return masterReadPduError(_pduBuff, MBFC_READ_HOLDING_REGISTERS);
    }

    TUint16 read_reg_quant = (TUint16)(_pduBuff[1] >> 1);
    if(read_reg_quant != _regQuant)
        return {EMasterPduError::MASTER_PDU_WRONG_MESSAGE, MBEC_NO_EXCEPTION, read_reg_quant};

    copyBytesToRegs(&_pduBuff[2], _regsBuff, _regQuant);
    return {EMasterPduError::MASTER_PDU_NO_ERROR, MBEC_NO_EXCEPTION, _regQuant};
}

TUint16 LCModbus::masterWritePduWriteSingleRegister(  TUint8 _pduBuff[], TUint16 _addr, TUint16 _reg)
{
    _pduBuff[0] = MBFC_WRITE_SINGLE_REGISTER;
    MB_REG_TO_BYTES(_addr, &_pduBuff[1]);
    MB_REG_TO_BYTES(_reg, &_pduBuff[3]);
    return 5;
}

LCModbus::SMasterPduStatus LCModbus::masterReadPduWriteSingleRegister(  TUint8 _pduBuff[], TUint16 _addr, TUint16 _reg)
{
    if(_pduBuff[0] != MBFC_WRITE_SINGLE_REGISTER)
    {
        //Обработка ошибок.
        return masterReadPduError(_pduBuff, MBFC_WRITE_SINGLE_REGISTER);
    }

    TUint16 ra, rr;

    MB_BYTES_TO_REG(&_pduBuff[1], ra);
    MB_BYTES_TO_REG(&_pduBuff[3], rr);

    if( ((ra ^ _addr)|(rr ^ _reg)) != 0)
    {
        return {EMasterPduError::MASTER_PDU_WRONG_MESSAGE, MBEC_NO_EXCEPTION, 1};
    }

    return {EMasterPduError::MASTER_PDU_NO_ERROR, MBEC_NO_EXCEPTION, 5};
}

//----------------------------------------------------------------------------------masterWritePduWriteMultipleRegisters
TUint16 LCModbus::masterWritePduWriteMultipleRegisters(  TUint8 _pdubuff[],
                                                         TUint16 _startAddr,
                                                         TUint16 _regQuant,
                                                         const TUint16 _regsBuff[])
{
    //Function code
    _pdubuff[0] = MBFC_WRITE_MULTIPLE_REGISTERS;
    //Starting address
    MB_REG_TO_BYTES(_startAddr, &_pdubuff[1]);
    //Quantity of Registers
    MB_REG_TO_BYTES(_regQuant, &_pdubuff[3]);
    //Byte count
    _pdubuff[5] = _regQuant * 2;
    copyRegsToBytes(_regsBuff, &_pdubuff[6], _regQuant);
    return 6 + _pdubuff[5];
}

//-----------------------------------------------------------------------------------masterReadPduWriteMultipleRegisters
LCModbus::SMasterPduStatus LCModbus::masterReadPduWriteMultipleRegisters(  TUint8 _pduBuff[],
                                                                           TUint16 _startAddr,
                                                                           TUint16 _regQuant)
{
    if(_pduBuff[0] != MBFC_WRITE_MULTIPLE_REGISTERS)
    {
        //Обработка ошибок.
        return masterReadPduError(_pduBuff, MBFC_WRITE_MULTIPLE_REGISTERS);
    }

    TUint16 start_addr, reg_quant;

    MB_BYTES_TO_REG(&_pduBuff[1], start_addr);
    MB_BYTES_TO_REG(&_pduBuff[3], reg_quant);

    if( ((start_addr ^ _startAddr)|(reg_quant ^ _regQuant)) != 0)
    {
        return {EMasterPduError::MASTER_PDU_WRONG_MESSAGE, MBEC_NO_EXCEPTION, reg_quant};
    }

    return {EMasterPduError::MASTER_PDU_NO_ERROR, MBEC_NO_EXCEPTION, 5};
}

//----------------------------------------------------------------------------------------------------------writeTcpMbap
TUint16 LCModbus::writeTcpMbap(TUint16  _transactionId,
                               TUint16  _length,
                               TUint8   _unitId,
                               TUint8   _adubuff[])
{
    MB_REG_TO_BYTES(_transactionId, &_adubuff[0]);
    _adubuff[2] = 0;
    _adubuff[3] = 0;
    MB_REG_TO_BYTES(_length,        &_adubuff[4]);
    _adubuff[6] = _unitId;
    return 7;
}

//----------------------------------------------------------------------------------------------------------checkTcpMbap
bool LCModbus::checkTcpMbap( TUint16 _transactionId,
                             TUint16 _length,
                             TUint8  _unitId,
                             const TUint8  _adubuff[])
{
    TUint16 res, t_id, len;

    MB_BYTES_TO_REG(&_adubuff[0], t_id);
    MB_BYTES_TO_REG(&_adubuff[4], len);

    res =   t_id ^ _transactionId;
    res |=  len  ^ _length;
    res |=  _adubuff[2] & _adubuff[3];
    res |=  _adubuff[6] ^ _unitId;

    return (res == 0) ? (true) : (false);
}

//-------------------------------------------------------------------------------------------------------copyBytesToRegs
void LCModbus::copyBytesToRegs(const TUint8 _bytes[], TUint16 _regs[], TUint16 _regQuant)
{
    for(TUint16 i = 0, j = 0; i < _regQuant; i++, j += 2)
    {
        MB_BYTES_TO_REG(&_bytes[j], _regs[i]);
    }
}

//-------------------------------------------------------------------------------------------------------copyRegsToBytes
void LCModbus::copyRegsToBytes(const TUint16 _regs[], TUint8 _bytes[], TUint16 _regQuant)
{
    for(TUint16 i = 0, j = 0; i < _regQuant; i++, j += 2)
    {
        MB_REG_TO_BYTES(_regs[i], &_bytes[j]);
    }
}


//======================================================================================================================
LCModbus::LCModbus()
{

}

LCModbus::LCModbus(const LCModbus&)
{

}
