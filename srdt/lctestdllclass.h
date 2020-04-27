#ifndef LCTESTDLLCLASS_H
#define LCTESTDLLCLASS_H

#include <QString>

class LCTestDllClass
{
public:

    class CEvent
    {

    private:
        static int var;
    public:
        CEvent();
        virtual int getVar();
    };

    LCTestDllClass();
    ~LCTestDllClass();
    virtual QString getString();
};

#endif // LCTESTDLLCLASS_H
