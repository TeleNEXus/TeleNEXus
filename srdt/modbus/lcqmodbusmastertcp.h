#ifndef LCQMODBUSMASTERTCP2_H
#define LCQMODBUSMASTERTCP2_H

#include "lcmodbusmasterbase.h"
#include <QModbusTcpClient>
#include <QUrl>

class QThread;

namespace modbus {

class LCQModbusMasterTcp : public LCModbusMasterBase
{
private:
    quint16             mPort;
    QString             mHostName;
    QModbusTcpClient*   mpMaster;
    QThread*            mpThread;

private:
    LCQModbusMasterTcp() = delete;
    LCQModbusMasterTcp(const LCQModbusMasterTcp&) = delete;
    LCQModbusMasterTcp& operator=(const LCQModbusMasterTcp&) = delete;
    explicit LCQModbusMasterTcp(QObject *_parent = nullptr);
    virtual ~LCQModbusMasterTcp();

public:
    static QSharedPointer<LCQModbusMasterTcp> create();

public slots:
    void connectToHost(const QUrl& _url);
    void disconnectFromHost();

private:
//--------------------------------------------------------------------------------------------------------------override
    virtual QModbusClient* createMaster() override;
    virtual QModbusClient* getMaster() override;
};

}//namespace

#endif // LCQMODBUSMASTERTCP2_H
