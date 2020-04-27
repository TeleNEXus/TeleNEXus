#ifndef EXTWIDGLIB_H
#define EXTWIDGLIB_H

#include "extwidglib_global.h"

#include <QString>

#include "../srdt/testdllinterface.h"
#include "../srdt/lctestdllclass.h"



//class EXTWIDGLIB_EXPORT Extwidglib
//{
//public:
//    Extwidglib();
//    QString getName();
//};

extern "C" QString EXTWIDGLIB_EXPORT getTestDllInterface(LCTestDllInterface*);
extern "C" QString EXTWIDGLIB_EXPORT getTestDllclass(LCTestDllClass*);

#endif // EXTWIDGLIB_H
