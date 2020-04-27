#include "lcqmodbusmastertcp.h"
#include <QDebug>
#include <QThread>

namespace modbus {

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp::LCQModbusMasterTcp(QObject *_parent) :  LCModbusMasterBase(_parent),
                                                            mPort(502),
                                                            mpMaster(nullptr),
                                                            mpThread(nullptr)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp::~LCQModbusMasterTcp()
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
QSharedPointer<LCQModbusMasterTcp> LCQModbusMasterTcp::create()
{
    LCQModbusMasterTcp* master = new LCQModbusMasterTcp(nullptr);
    master->mpThread = new QThread;
    master->moveToThread(master->mpThread);
    master->mpThread->start();
    return QSharedPointer<LCQModbusMasterTcp>(master, deleterLater);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp::connectToHost(const QUrl& _url)
{
    mPort = _url.port();
    mHostName = _url.host();
    connectRequest();
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp::disconnectFromHost()
{
    //TODO: Add Code
}

QModbusClient* LCQModbusMasterTcp::createMaster()
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

QModbusClient* LCQModbusMasterTcp::getMaster()
{
    return mpMaster;
}

}//namespace
