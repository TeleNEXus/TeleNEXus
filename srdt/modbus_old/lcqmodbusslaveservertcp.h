#ifndef LQMODBUSSLAVESERVERTCP_H
#define LQMODBUSSLAVESERVERTCP_H

#include <QTcpServer>

class LQModbusSlaveServerTcp : public QTcpServer
{
    Q_OBJECT
public:
    explicit LQModbusSlaveServerTcp(QObject *parent = nullptr);
protected:
    virtual void incomingConnection(qintptr handle) override;
};

#endif // LQMODBUSSLAVESERVERTCP_H
