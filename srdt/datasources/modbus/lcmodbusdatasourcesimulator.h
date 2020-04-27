#ifndef LCQMODBUSDATASOURCESIMULATOR_H
#define LCQMODBUSDATASOURCESIMULATOR_H

#include "lcqremotedatasourcebase.h"
#include "lcmodbus.h"

namespace modbus {

class LCModbusDataSourceSimulator : public LCQRemoteDataSourceBase
{
public:
    LCModbusDataSourceSimulator();
    virtual void connectReader(const QString& _dataName, QObject* _reader)override
    {
        Q_UNUSED(_dataName);
        Q_UNUSED(_reader);
    }
    virtual void disconnectReader(QObject* _reader)override{Q_UNUSED(_reader);}
    virtual void read(const QString& _dataName, QObject* _reader)override
    {
        Q_UNUSED(_dataName);
        Q_UNUSED(_reader);
    }
    virtual void write(const QString& _dataName, const QByteArray& _data, QObject* _writer = nullptr)override
    {
        Q_UNUSED(_dataName);
        Q_UNUSED(_data);
        Q_UNUSED(_writer);
    }

    void addDataItemHoldingRegs(const QString& _dataName, TUint16 _addr, TUint16 _length)
    {
        Q_UNUSED(_dataName);
        Q_UNUSED(_addr);
        Q_UNUSED(_length);
    }
    void deleteDataItemOne(const QString& _dataName){Q_UNUSED(_dataName);}
    void deleteDataItemAll(){}


};

}

#endif // LCQMODBUSDATASOURCESIMULATOR_H
