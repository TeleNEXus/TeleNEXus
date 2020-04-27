#ifndef CQSOCKETCLIENT1_H
#define CQSOCKETCLIENT1_H

#include <QObject>
#include <QTcpSocket>

class CQSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit CQSocketClient(QObject *parent = nullptr);

public slots:
    void run();
    void finish();
signals:
    void finished();
private:
    volatile bool   mFlagFinish;


};

#endif // CQSOCKETCLIENT1_H
