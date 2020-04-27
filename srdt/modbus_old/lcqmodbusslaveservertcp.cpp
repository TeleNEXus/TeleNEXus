#include "lcqmodbusslaveservertcp.h"
#include "lcqmodbusslavesockettcp.h"
#include <QDebug>
#include <QHostInfo>
LQModbusSlaveServerTcp::LQModbusSlaveServerTcp(QObject *parent) : QTcpServer(parent)
{

}

void LQModbusSlaveServerTcp::incomingConnection(qintptr handle)
{
    LCQModbusSlaveSocketTcp *sock;
    QHostInfo hi(handle);

    qDebug() << "Host name" << hi.hostName();
    qDebug() << "Server addr" << this->serverAddress();

    sock = new LCQModbusSlaveSocketTcp(this);
    sock->setSocketDescriptor(handle);
}
