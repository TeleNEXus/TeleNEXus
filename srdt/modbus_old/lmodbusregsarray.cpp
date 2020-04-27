#include "lmodbusregsarray.h"

#include "lexcept.h"

namespace modbus
{

LCModbusRegsArray::LCModbusRegsArray(TUint16 _length)  noexcept : mpDataArray(nullptr)
{
    mLength = (_length > 0) ? (_length) : (1);
    mpDataArray = new TUint16[mLength];
}

LCModbusRegsArray::~LCModbusRegsArray()  noexcept
{
    if(mLength == 0) return;
    delete [] mpDataArray;
}

TUint16 LCModbusRegsArray::copy(const LCModbusRegsArray& _array, TUint16 _startIndex, TUint16 _length) noexcept
{
    if((_length == 0) || (_startIndex >= _array.mLength)) return 0;

    if(_length + _startIndex >= _array.mLength) _length = _array.mLength - _startIndex;

    if(_length > mLength) _length = mLength;

    for(TUint16 i = 0; i < _length; i++)
    {
        mpDataArray[i] = _array.mpDataArray[_startIndex + i];
    }
    return _length;
}

TUint16 LCModbusRegsArray::copy(const TUint16* _array, TUint16 _startIndex, TUint16 _length) noexcept
{
    if(_length == 0) return 0;

    if(_length > mLength) _length = mLength;

    for(TUint16 i = 0; i < _length; i++)
    {
        mpDataArray[i] = _array[_startIndex + i];
    }
    return _length;
}

bool LCModbusRegsArray::isPresent(TUint16 _i)
{
    return (_i >= mLength) ? (false):(true);
}

TUint16& LCModbusRegsArray::operator[](TUint16 _i) noexcept
{
    if(_i >= mLength) return mpDataArray[mLength - 1];
    return mpDataArray[_i];
}

}
