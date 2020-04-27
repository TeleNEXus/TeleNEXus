#ifndef CQCLIENTTCPECHO_H
#define CQCLIENTTCPECHO_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QThread>

class CQClientTcpEcho : public QObject
{
    Q_OBJECT
public:
    explicit CQClientTcpEcho(QObject *parent = nullptr);

    virtual ~CQClientTcpEcho();
public slots:
    void sendEcho(const QString& _sout);
    void start();
    void disconnectFromServer();
public slots:
    void readData();
    void connectionClosedByServer();
    void error(QAbstractSocket::SocketError);
    void stop();

private slots:
    void connectSeq();

signals:
    void receveEcho(const QString& _sin);

private:
    enum EStateConnectSeq
    {
        ST_BEGIN,
        ST_CONNECTING,
        ST_MONITOR

    };

    QTcpSocket mSocket;
    QTimer mTimer;
    volatile short mDataSize;
    EStateConnectSeq mStateConnectSeq;

    volatile bool mFlagActive;

};

#endif // CQCLIENTTCPECHO_H
