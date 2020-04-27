#include <QDebug>
#include "lcqmodbusslavesockettcp.h"
#include <QDataStream>
#include "lcmodbus.h"
#include <QByteArray>
#include <QHostAddress>
using namespace modbus;

LCQModbusSlaveSocketTcp::LCQModbusSlaveSocketTcp(QObject *parent) : QTcpSocket(parent)
{
    qDebug() << "New connection";
    connect(this, &LCQModbusSlaveSocketTcp::readyRead,
            [=](){
                QDataStream dstr(this);
                int l = 0;
                QByteArray ba;
                l = dstr.readRawData((char*)mByteBuff, 256);
                ba.setRawData((char*)mByteBuff, l);
                qDebug() << "Read data" << QString(ba.toHex());
            });

    connect(this, &LCQModbusSlaveSocketTcp::disconnected,
            [=](){qDebug() << "Disconnected"; this->deleteLater();});

    connect(this, &LCQModbusSlaveSocketTcp::stateChanged,
            [=](){
                qDebug() <<"--------------------"<< this->state();
            });
}
