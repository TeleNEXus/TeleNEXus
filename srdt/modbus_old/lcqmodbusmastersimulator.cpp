#include "lcqmodbusmastersimulator.h"
#include "lmodbusdefs.h"
#include <QMutexLocker>
#include <QCoreApplication>
#include <QDebug>
#include <QDataStream>

namespace modbus {


//==============================================================================================LCQModbusMasterSimulator
LCQModbusMasterSimulator::LCQModbusMasterSimulator(QObject *_parent) :  QObject(_parent),
                                                                        mDataBlock(128,0),
                                                                        mCounter(0)
{
        connect(&mTimer, &QTimer::timeout,
            [=](){
                QMutexLocker ml(&mMutex);
                mDataBlock[0] = mCounter & 0x00ff;
                mDataBlock[1] = mCounter >> 8;
                mCounter++;
                emit dataChange(mDataBlock);
            });
        mTimer.start(1000);
}

LCQModbusMasterSimulator::~LCQModbusMasterSimulator()
{
}

static void doDeleteLater(QObject* _obj)
{
    _obj->deleteLater();
}

QSharedPointer<LCQModbusMasterSimulator> LCQModbusMasterSimulator::create(QObject *_parent)
{
    return QSharedPointer<LCQModbusMasterSimulator>(new LCQModbusMasterSimulator(_parent), doDeleteLater);
}

LCQModbusMasterSimulator::EOperationStatus
            LCQModbusMasterSimulator::readHoldingRegisters( TUint8 _devId,
                                                            TUint16 _addr,
                                                            TUint16 _regQuant,
                                                            TUint16 _regs[])
{
    Q_UNUSED(_devId);
    QMutexLocker ml(&mMutex);

    if((_regQuant == 0) ||
            (_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)) return EOperationStatus::WRONG_REQ;
    if(_addr + _regQuant > (mDataBlock.size() >> 1))return EOperationStatus::MODBUS_EXCEPTION;

    for(int i = 0; i < _regQuant; i++)
    {
        _regs[i] = ((TUint16*)mDataBlock.constData())[i + _addr];
    }
    return EOperationStatus::OK;
}

LCQModbusMasterSimulator::EOperationStatus
            LCQModbusMasterSimulator::writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg)
{
    Q_UNUSED(_devId);
    QMutexLocker ml(&mMutex);

    if(_addr >= (mDataBlock.size() >> 1))return EOperationStatus::MODBUS_EXCEPTION;
    mDataBlock[_addr*2] = (TUint8)(_reg & 0x00ff);
    mDataBlock[_addr*2 + 1] = (TUint8)(_reg >> 8);
    emit dataChange(mDataBlock);
    return EOperationStatus::OK;
}

LCQModbusMasterSimulator::EOperationStatus
            LCQModbusMasterSimulator::
                writeMultipleRegisters(TUint8 _devId, TUint16 _addr, TUint16 _regQuant, const TUint16 _regs[])
{
    Q_UNUSED(_devId);
    QMutexLocker ml(&mMutex);
    if((_regQuant == 0) ||
            (_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)) return EOperationStatus::WRONG_REQ;
    if(_addr + _regQuant > (mDataBlock.size() >> 1))return EOperationStatus::MODBUS_EXCEPTION;

    for(int i = 0; i < _regQuant; i++)
    {
        ((TUint16*)mDataBlock.constData())[i + _addr] = _regs[i];
    }
    emit dataChange(mDataBlock);
    return EOperationStatus::OK;
}

TUint16 LCQModbusMasterSimulator::getRegister(TUint16 _addr)
{
    if(_addr >= (mDataBlock.size() >> 1))return 0;
    return ((TUint16*)mDataBlock.constData())[_addr];
}

void LCQModbusMasterSimulator::setRegister(TUint16 _addr, TUint16 _val)
{
    if(_addr >= (mDataBlock.size() >> 1)) return;
    mDataBlock[_addr*2]     = _val & 0x00ff;
    mDataBlock[_addr*2 + 1] = _val >> 8;
    emit dataChange(mDataBlock);
}

}//namespace modbus
