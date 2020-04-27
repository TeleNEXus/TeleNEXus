/***********************************************************************************************************************
 * FILE NAME:       lmodbusregsarray.h
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:           Класс массива регистров MODBUS
 * ********************************************************************************************************************/
#ifndef LMODBUSREGSARRAY_H
#define LMODBUSREGSARRAY_H

#include "ltypedef.h"

namespace modbus
{
using namespace lstd;

class LCModbusRegsArray
{
public:
    LCModbusRegsArray(TUint16 _length)  noexcept;
    virtual ~LCModbusRegsArray() noexcept;
    TUint16 getLength(void) noexcept {return mLength;}
    /*
     * Копирует _length значений из массива _array, начиная с _startIndex.
     * Возвращает количество скопированных элементов.
     */
    TUint16 copy(const LCModbusRegsArray& _array, TUint16 _startIndex, TUint16 _length) noexcept;
    /*
     * Перегрузка для массива TUint16.
     */
    TUint16 copy(const TUint16* _array, TUint16 _startIndex, TUint16 _length) noexcept;
    bool isPresent(TUint16 _i);
    TUint16& operator[](TUint16 _i) noexcept;

private:
    LCModbusRegsArray();
private:
    TUint16*    mpDataArray;
    TUint16     mLength;
};

}//namespace modbus

#endif // CMODBUSREGSARRAY_H
