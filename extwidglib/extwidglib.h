#ifndef EXTWIDGLIB_H
#define EXTWIDGLIB_H

#include "extwidglib_global.h"

#include <QString>

#include <QSharedPointer>

#include "../srdt/testdllinterface.h"
#include "../srdt/lctestdllclass.h"



//class EXTWIDGLIB_EXPORT Extwidglib
//{
//public:
//    Extwidglib();
//    QString getName();
//};

extern "C" QString EXTWIDGLIB_EXPORT getTestDllInterfaceString(LCTestDllInterface*);
extern "C" QSharedPointer<LCTestDllInterface> EXTWIDGLIB_EXPORT getTestDllInterface();

#endif // EXTWIDGLIB_H
