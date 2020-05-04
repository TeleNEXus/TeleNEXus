#include "lqmodbusmastertcp.h"
#include <QDebug>
#include <QThread>

namespace modbus {

//----------------------------------------------------------------------------------------------------------------------
LQModbusMasterTcp::LQModbusMasterTcp(QObject *_parent) :  LQModbusMasterBase(_parent),
                                                            mPort(502),
                                                            mpMaster(nullptr),
                                                            mpThread(nullptr)
{

}

//----------------------------------------------------------------------------------------------------------------------
LQModbusMasterTcp::~LQModbusMasterTcp()
{
    mpThread->quit();
    mpThread->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
static void deleterLater(QObject* _obj)
{
    _obj->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LQModbusMasterTcp> LQModbusMasterTcp::create()
{
    LQModbusMasterTcp* master = new LQModbusMasterTcp(nullptr);
    master->mpThread = new QThread;
    master->moveToThread(master->mpThread);
    master->mpThread->start();
    return QSharedPointer<LQModbusMasterTcp>(master, deleterLater);
}

//----------------------------------------------------------------------------------------------------------------------
void LQModbusMasterTcp::connectToHost(const QUrl& _url)
{
    mPort = _url.port();
    mHostName = _url.host();
    connectRequest();
}

//----------------------------------------------------------------------------------------------------------------------
void LQModbusMasterTcp::disconnectFromHost()
{
    //TODO: Add Code
}

QModbusClient* LQModbusMasterTcp::createMaster()
{
    if(mpMaster != nullptr)
    {
//        mpMaster->disconnectDevice();
        mpMaster->deleteLater();
    }

    mpMaster = new QModbusTcpClient();

    mpMaster->setConnectionParameter(QModbusDevice::NetworkPortParameter, mPort);
    mpMaster->setConnectionParameter(QModbusDevice::NetworkAddressParameter, mHostName);
    mpMaster->setTimeout(500);
    qDebug() << "LCQModbusMasterTcp::createMaster()";
    return mpMaster;
}

QModbusClient* LQModbusMasterTcp::getMaster()
{
    return mpMaster;
}

}//namespace
