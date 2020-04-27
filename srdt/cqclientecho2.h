#ifndef CQCLIENTECHO2_H
#define CQCLIENTECHO2_H

#include <QObject>
#include <QHostAddress>
#include <QThread>
#include <QMutex>
class CQClientEcho2 : public QObject
{
    Q_OBJECT
public:
    explicit CQClientEcho2(QObject *parent = nullptr);

    virtual ~CQClientEcho2();

    void setTimeWaitResponseMs(int _time);
    void setTimeReconnectAfterErrorMs(int _time);
//    void disconnectFromHost();

    void echo(const QString& _instr, QString& _outstr);
    void echoSend(const QString& _str);

public slots:
    void connectToHost(const QHostAddress& _hostaddr, quint16 _port);
signals:
    void echoReceive(const QString& _str);
private:
    void* mpLocalData;
        QMutex mMutex;
};

#include <QEventLoop>

class CQClientEchoProcess : public QThread
{
    Q_OBJECT
public:
    explicit CQClientEchoProcess(const QString& name, QObject *parent = nullptr);

public slots:
    void run() override;
    void echoReceive(const QString& str);
    virtual void quit();
signals:
    void echoSend(const QString& _strout);
private:
    volatile bool mFlagStop;
    QEventLoop mEventLoop;
    QString mName;
};

#endif // CQCLIENTECHO2_H
