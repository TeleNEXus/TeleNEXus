/***********************************************************************************************************************
 * FILE NAME:       ltypedef.h
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:           Заголовочный файл определения стандартных типов данных.
 * ********************************************************************************************************************/
#ifndef LTYPEDEF_H
#define LTYPEDEF_H

#include <stdint.h>

namespace lstd
{

typedef uint8_t     TUint8;
typedef int8_t      TInt8;
typedef TUint8      U8;
typedef TInt8       S8;

typedef uint16_t    TUint16;
typedef int16_t     TInt16;
typedef TUint16     U16;
typedef TInt16      S16;

typedef uint32_t    TUint32;
typedef int32_t     TInt32;
typedef TUint32     U32;
typedef TInt32      S32;

typedef uint64_t    TUint64;
typedef int64_t     TInt64;

typedef float       TFloat32;
typedef TFloat32    F32;

typedef double      TFloat64;
typedef TFloat64    F64;

typedef TUint64      TSize;

}

#endif // LTYPEDEF_H
