#ifndef LCMODBUSMASTERSIMULATOR_H
#define LCMODBUSMASTERSIMULATOR_H

#include "lcmodbusmasterbase.h"
#include "lqextendevent.h"
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QTimer>
#include <QHostAddress>
#include <QTcpSocket>

namespace modbus {

class LCQModbusMasterSimulator : public QObject, public LCModbusMasterBase
{
        Q_OBJECT
    private:
        QByteArray mDataBlock;
        QMutex mMutex;
private:
    explicit LCQModbusMasterSimulator(QObject *parent = nullptr);
public:
    LCQModbusMasterSimulator(const LCQModbusMasterSimulator&) = delete;
    LCQModbusMasterSimulator& operator=(const LCQModbusMasterSimulator&) = delete;
    virtual ~LCQModbusMasterSimulator();
    static QSharedPointer<LCQModbusMasterSimulator> create(QObject *_parent = nullptr);
//--------------------------------------------------------------------------------------------------------------override
    virtual EOperationStatus readHoldingRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) override;

    virtual EOperationStatus writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg) override;

    virtual EOperationStatus writeMultipleRegisters(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuant, const TUint16 _regs[]) override;


        TUint16 getRegister(TUint16 _addr);
        void setRegister(TUint16 _addr, TUint16 _val);

    signals:
        void dataChange(const QByteArray& _data);
    private:
        QTimer mTimer;
        TUint16 mCounter;
};

}//namespace modbus

#endif // LCMODBUSMASTERSIMULATOR_H
