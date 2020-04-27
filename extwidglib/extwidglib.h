#ifndef EXTWIDGLIB_H
#define EXTWIDGLIB_H

#include "extwidglib_global.h"

#include <QString>

#include "../srdt/testdllinterface.h"



//class EXTWIDGLIB_EXPORT Extwidglib
//{
//public:
//    Extwidglib();
//    QString getName();
//};

extern "C" QString EXTWIDGLIB_EXPORT getExportString(LCTestDllInterface*);

#endif // EXTWIDGLIB_H
