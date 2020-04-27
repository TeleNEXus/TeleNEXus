#include "lcqmodbusmasterrtu.h"
#include <QDebug>
#include <QThread>

namespace modbus {

LCQModbusMasterRtu::LCQModbusMasterRtu(QObject* _parent) :
            LCModbusMasterBase(_parent),
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
LCQModbusMasterRtu::~LCQModbusMasterRtu()
{
    mpThread->quit();
    mpThread->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterRtu::connectToPort(QString                 _portName,
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
void LCQModbusMasterRtu::disconnectFromPort()
{
    //TODO: add code.
}

//----------------------------------------------------------------------------------------------------------------------
static void deleterLater(QObject* _obj)
{
    _obj->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LCQModbusMasterRtu> LCQModbusMasterRtu::create()
{
    LCQModbusMasterRtu* master = new LCQModbusMasterRtu(nullptr);
    master->mpThread = new QThread;
    master->moveToThread(master->mpThread);
    master->mpThread->start();
    return QSharedPointer<LCQModbusMasterRtu>(master, deleterLater);
}

//----------------------------------------------------------------------------------------------------------------------
QModbusClient* LCQModbusMasterRtu::createMaster()
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

    qDebug() << "LCQModbusMasterRtu::createMaster()";
    return mpMaster;
}

QModbusClient* LCQModbusMasterRtu::getMaster()
{
    return mpMaster;
}

}//namespace
