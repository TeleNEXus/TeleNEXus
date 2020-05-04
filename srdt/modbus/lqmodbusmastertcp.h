#ifndef LQMODBUSMASTERTCP_H
#define LQMODBUSMASTERTCP_H

#include "lqmodbusmasterbase.h"

#include <QModbusTcpClient>
#include <QUrl>

class QThread;

namespace modbus {

class LQModbusMasterTcp : public LQModbusMasterBase
{
private:
    quint16             mPort;
    QString             mHostName;
    QModbusTcpClient*   mpMaster;
    QThread*            mpThread;

private:
    LQModbusMasterTcp() = delete;
    LQModbusMasterTcp(const LQModbusMasterTcp&) = delete;
    LQModbusMasterTcp& operator=(const LQModbusMasterTcp&) = delete;
    explicit LQModbusMasterTcp(QObject *_parent = nullptr);
    virtual ~LQModbusMasterTcp();

public:
    static QSharedPointer<LQModbusMasterTcp> create();

public slots:
    void connectToHost(const QUrl& _url);
    void disconnectFromHost();

private:
//--------------------------------------------------------------------------------------------------------------override
    virtual QModbusClient* createMaster() override;
    virtual QModbusClient* getMaster() override;
};

}//namespace

#endif // LQMODBUSMASTERTCP_H
