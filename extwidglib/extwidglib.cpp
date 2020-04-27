#include "extwidglib.h"

//Extwidglib::Extwidglib()
//{
//}

//QString Extwidglib::getName()
//{
//    return "Extwidglib::getName()";
//}

QString getTestDllInterface(LCTestDllInterface* _t)
{
    return _t->getString();
}

QString getTestDllclass(LCTestDllClass* _c)
{
    return _c->getString();
}



