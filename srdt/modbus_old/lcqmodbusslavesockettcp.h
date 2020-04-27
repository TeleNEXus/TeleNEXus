#ifndef LCQMODBUSSLAVESOCKETTCP_H
#define LCQMODBUSSLAVESOCKETTCP_H

#include <QTcpSocket>
#include "ltypedef.h"

class LCQModbusSlaveSocketTcp : public QTcpSocket
{
    Q_OBJECT
public:
    explicit LCQModbusSlaveSocketTcp(QObject *parent = nullptr);

signals:
private:
    lstd::TUint8 mByteBuff[300];
};

#endif // LCQMODBUSSLAVESOCKETTCP_H
