#include "extwidglib.h"

//Extwidglib::Extwidglib()
//{
//}

//QString Extwidglib::getName()
//{
//    return "Extwidglib::getName()";
//}

QString getTestDllInterfaceString(LCTestDllInterface* _t)
{
    return _t->getString();
}

class LCTestDll : public LCTestDllInterface
{
public:
    LCTestDll(){qDebug() << "LCTestDll";}
    virtual ~LCTestDll(){qDebug() << "~LCTestDll";}
    virtual QString getString()
    {
        return QString("class LCTestDll : public LCTestDllInterface");
    }
};

QSharedPointer<LCTestDllInterface> EXTWIDGLIB_EXPORT getTestDllInterface()
{
    return QSharedPointer<LCTestDllInterface>(new LCTestDll());
}





