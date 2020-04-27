#include "cqclientecho2.h"
#include <QHostAddress>
#include <QDebug>
#include <QDataStream>
#include <QEventLoop>
#include <QTimer>
#include <QTcpSocket>
#include <QThread>

struct SLocalData
{
    QTcpSocket  *mpSocket;
    QTimer      *mpTimer;
    QEventLoop  *mpEchoEventLoop;
    QString     mEchoString;
    volatile bool mFlagDisconnect;
    bool        mFlagResponseTimeout;
    int         mTimeWaitResponseMs;
    int         mTimeReconnectAfterErrorMs;
};

#define THISLD ((SLocalData*)mpLocalData)

CQClientEcho2::CQClientEcho2(QObject *parent) : QObject(parent)
{
    mpLocalData = new SLocalData;
    THISLD->mpSocket = nullptr;
    THISLD->mpTimer  = new QTimer(this);
    THISLD->mpTimer->setSingleShot(true);
    THISLD->mpEchoEventLoop = new QEventLoop(this);
    THISLD->mFlagDisconnect = false;
    THISLD->mFlagResponseTimeout = false;
    THISLD->mTimeWaitResponseMs = 2500;
    THISLD->mTimeReconnectAfterErrorMs = 1000;
}

CQClientEcho2::~CQClientEcho2()
{
    if(THISLD->mpSocket) THISLD->mpSocket->deleteLater();
    THISLD->mpTimer->stop();
    THISLD->mpTimer->deleteLater();
    THISLD->mpEchoEventLoop->deleteLater();
    delete THISLD;
}

void CQClientEcho2::setTimeWaitResponseMs(int _time)
{
    if(_time < 100) _time = 100;
    THISLD->mTimeWaitResponseMs = _time;
}

void CQClientEcho2::setTimeReconnectAfterErrorMs(int _time)
{
    if(_time < 100) _time = 100;
    THISLD->mTimeReconnectAfterErrorMs = _time;
}

//void CQClientEcho2::disconnectFromHost()
//{
//    QTcpSocket* socket;
//    socket = THISLD->mpSocket;


//}

quint16 __counter;
void CQClientEcho2::echo(const QString& _instr, QString& _outstr)
{

//    mMutex.lock();
//    __counter++;
//    mMutex.unlock();

    if(THISLD->mpSocket == nullptr)
    {
        _outstr = "No connection";
        return;
    }

    QDataStream out(THISLD->mpSocket);

    QMetaObject::Connection conn1 =
    connect(THISLD->mpTimer, &QTimer::timeout, [=](){
        qDebug() << "---------------------------------------------DEBUG: Timeout connection";
//        THISLD->mpSocket->disconnectFromHost();
        THISLD->mFlagResponseTimeout = true;
        if(THISLD->mpEchoEventLoop->isRunning()) THISLD->mpEchoEventLoop->exit();
    });

    if(THISLD->mpSocket->state() == QAbstractSocket::SocketState::ConnectedState)
    {
        out << _instr;
        THISLD->mpTimer->start(THISLD->mTimeWaitResponseMs);
        THISLD->mpEchoEventLoop->exec();
        THISLD->mpTimer->stop();
        if(THISLD->mFlagResponseTimeout)
        {
            THISLD->mFlagResponseTimeout = false;
            _outstr = "Connection timeout!";
        }
        else
        {
            _outstr = THISLD->mEchoString;
        }
    }
    else
    {
        _outstr = "No connection";
    }

    disconnect(conn1);
}

void CQClientEcho2::echoSend(const QString& _str)
{
    QString echostr;
    echo(_str, echostr);
    emit echoReceive(echostr);
}

void CQClientEcho2::connectToHost(const QHostAddress& _hostaddr, quint16 _port)
{
    if(THISLD->mpSocket)
    {
        if(THISLD->mpEchoEventLoop->isRunning()) THISLD->mpEchoEventLoop->exit();
        THISLD->mpSocket->deleteLater();
    }

    THISLD->mpSocket = new QTcpSocket(this);
//    THISLD->mpSocket = new QTcpSocket;
//    THISLD->mpSocket->moveToThread(QThread::currentThread());

    connect(THISLD->mpSocket, &QTcpSocket::connected, [](){ qDebug() << "Connected to Host";});

    connect(THISLD->mpSocket, &QTcpSocket::disconnected,
        [=](){
            qDebug() << "---------------------------------------------Disconnected from Host";
            THISLD->mpSocket->connectToHost(_hostaddr, _port);
            if(THISLD->mpEchoEventLoop->isRunning()) THISLD->mpEchoEventLoop->exit();
        });

    connect(THISLD->mpSocket, &QTcpSocket::stateChanged,
        [](QAbstractSocket::SocketState socketState){
            qDebug() << "---------------------------------------------Socket state changed: " << socketState;
        });

    connect(THISLD->mpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        [=](QAbstractSocket::SocketError socketError){
            qDebug() << "---------------------------------------------Socket error: " << socketError;
            THISLD->mpSocket->abort();
            THISLD->mpSocket->deleteLater();
            THISLD->mpSocket = nullptr;
            QTimer::singleShot(THISLD->mTimeReconnectAfterErrorMs, this, [=](){connectToHost(_hostaddr, _port);});
            if(THISLD->mpEchoEventLoop->isRunning()) THISLD->mpEchoEventLoop->exit();
        });

    connect(THISLD->mpSocket, &QTcpSocket::readyRead,
        [=](){
            QDataStream in(THISLD->mpSocket);
            qDebug() << "---------------------------------------------Redy read data size:" << THISLD->mpSocket->bytesAvailable();
            if(THISLD->mpSocket->bytesAvailable() > 0)
            in >> THISLD->mEchoString;
            if(THISLD->mpEchoEventLoop->isRunning()) THISLD->mpEchoEventLoop->exit();
        });


    THISLD->mpSocket->connectToHost(_hostaddr, _port);
}

CQClientEchoProcess::CQClientEchoProcess(const QString& name, QObject *parent) :
    QThread(parent), mFlagStop(false), mName(name)
{
    mEventLoop.moveToThread(this);
}

void CQClientEchoProcess::run()
{
//    CQClientEcho2 *ce = new CQClientEcho2;
//    QHostAddress haddr("192.168.0.2");
//    QString str_tx;
//    QString str_rx;
//    QTimer t1;
//    quint32 test_counter = 0;

//    qDebug() << "Host: " << haddr.toString();
//    ce->connectToHost(haddr, 1001);

//    QMetaObject::Connection con = QObject::connect(&t1, &QTimer::timeout,
//            [&](){
//                    QString str_tx;
//                    QString str_rx;
//                    str_tx = "New Message! #:";
//                    str_tx += QString::number(test_counter);
//                    qDebug() << "Transmited data:   " << str_tx;
//                    ce->echo(str_tx, str_rx);
//                    qDebug() << "Received data:     " << str_rx;
//                    test_counter++;
//        });
//        t1.start(500);
//        exec();
//        qDebug() << "Exit loop";
//        t1.stop();

    int counter = 0;
    QString strout, strecho;
    while(!mFlagStop)
    {
        strout = mName + " Echo message #";
        strout += QString::number(counter);
        qDebug() << "Echo send      :" << strout;
        emit echoSend(strout);
        counter++;
        mEventLoop.exec();
        QThread::msleep(500);
    }
}

void CQClientEchoProcess::echoReceive(const QString& str)
{
    qDebug() << "Echo response  :" << str;
    mEventLoop.exit();
}

void CQClientEchoProcess::quit()
{
    mFlagStop = true;
    QThread::quit();
}
