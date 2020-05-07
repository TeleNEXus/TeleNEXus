#include "lctestdllclass.h"

LCTestDllClass::LCTestDllClass()
{

}

LCTestDllClass::~LCTestDllClass()
{

}

QString LCTestDllClass::getString()
{
    CEvent event;
    return "LCTestDllClass::getString() var = " + QString::number(event.getVar());
}


int LCTestDllClass::CEvent::var = 10;

LCTestDllClass::CEvent::CEvent()
{

}

int LCTestDllClass::CEvent::getVar()
{
    return var;
}
