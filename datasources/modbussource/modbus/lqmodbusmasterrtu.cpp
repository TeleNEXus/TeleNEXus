#include "lqmodbusmasterrtu.h"
#include <QDebug>
#include <QThread>

namespace modbus {

LQModbusMasterRtu::LQModbusMasterRtu(QObject* _parent) :
            LQModbusMasterBase(_parent),
            mBaudRate (QSerialPort::BaudRate::UnknownBaud),
            mParity (QSerialPort::Parity::UnknownParity),
            mDataBits (QSerialPort::DataBits::UnknownDataBits),
            mStopBits (QSerialPort::StopBits::UnknownStopBits),
            mTimeout(500),
            mpMaster(nullptr),
            mpThread(nullptr)
{
}

//----------------------------------------------------------------------------------------------------------------------
LQModbusMasterRtu::~LQModbusMasterRtu()
{
    mpThread->quit();
    mpThread->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
void LQModbusMasterRtu::connectToPort(QString                 _portName,
                                       QSerialPort::BaudRate   _baudRate,
                                       QSerialPort::Parity     _parity,
                                       QSerialPort::DataBits   _dataBits,
                                       QSerialPort::StopBits   _stopBits,
                                       int                     _timeout)
{
    mPortName   = _portName;
    mParity     = _parity;
    mBaudRate   = _baudRate;
    mDataBits   = _dataBits;
    mStopBits   = _stopBits;
    mTimeout    = _timeout;
    connectRequest();
}

//----------------------------------------------------------------------------------------------------------------------
void LQModbusMasterRtu::disconnectFromPort()
{
    //TODO: add code.
}

//----------------------------------------------------------------------------------------------------------------------
static void deleterLater(QObject* _obj)
{
    _obj->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LQModbusMasterRtu> LQModbusMasterRtu::create()
{
    LQModbusMasterRtu* master = new LQModbusMasterRtu(nullptr);
    master->mpThread = new QThread;
    master->moveToThread(master->mpThread);
    master->mpThread->start();
    return QSharedPointer<LQModbusMasterRtu>(master, deleterLater);
}

//----------------------------------------------------------------------------------------------------------------------
QModbusClient* LQModbusMasterRtu::createMaster()
{
    if(mpMaster != nullptr)
    {
        mpMaster->disconnectDevice();
        mpMaster->deleteLater();
    }

    mpMaster = new QModbusRtuSerialMaster;

    mpMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, mPortName);
    mpMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, mBaudRate);
    mpMaster->setConnectionParameter(QModbusDevice::SerialParityParameter,   mParity);
    mpMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, mDataBits);
    mpMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, mStopBits);
    mpMaster->setTimeout(mTimeout);

    return mpMaster;
}

QModbusClient* LQModbusMasterRtu::getMaster()
{
    return mpMaster;
}

}//namespace
