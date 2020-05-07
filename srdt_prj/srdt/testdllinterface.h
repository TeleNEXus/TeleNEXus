#ifndef TESTDLLINTERFACE_H
#define TESTDLLINTERFACE_H

#include <QString>
#include <QDebug>

class LCTestDllInterface
{
public:
    LCTestDllInterface(){qDebug() << "LCTestDllInterface";}
    virtual ~LCTestDllInterface(){qDebug() << "~LCTestDllInterface";}
    virtual QString getString() = 0;
};

#endif // TESTDLLINTERFACE_H
