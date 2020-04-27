#ifndef TESTDLLINTERFACE_H
#define TESTDLLINTERFACE_H

#include <QString>

class LCTestDllInterface
{
public:
    LCTestDllInterface(){}
    virtual ~LCTestDllInterface(){}
    virtual QString getString() = 0;
};

#endif // TESTDLLINTERFACE_H
