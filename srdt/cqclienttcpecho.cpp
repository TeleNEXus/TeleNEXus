#include "cqclienttcpecho.h"
#include <QHostAddress>
#include <QDebug>
#include <QDataStream>
#include <string>
#include <iostream>
#include <QTimer>

CQClientTcpEcho::CQClientTcpEcho(QObject *parent) : QObject(parent),
                                                    mSocket(this),
                                                    mTimer(this),
                                                    mDataSize(0),
                                                    mStateConnectSeq(ST_BEGIN),
                                                    mFlagActive(false)
{

//    connect(&mSocket, &QTcpSocket::readyRead, this, &CQClientTcpEcho::readData);
//    connect(&mSocket, &QTcpSocket::disconnected, this, &CQClientTcpEcho::disconnectFromServer);
//    connect(&mSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
//            this, &CQClientTcpEcho::error);

//    connect(&mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

CQClientTcpEcho::~CQClientTcpEcho()
{

}

// public slots.
void CQClientTcpEcho::sendEcho(const QString& _sin)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QByteArray blockRead;
    QDataStream inread(&blockRead, QIODevice::ReadWrite);

    inread.setVersion(QDataStream::Qt_5_9);

    out.device()->seek(0);

    //out.writeRawData(_sin.toStdString().data(), _sin.toStdString().length());
    out << _sin.toStdString().data();
    mSocket.write(block);

    mDataSize = block.size();
}

void CQClientTcpEcho::start()
{
    if(mFlagActive) return;
    mFlagActive = true;
    QTimer::singleShot(1, this, [=](){ connectSeq();});
}

void CQClientTcpEcho::disconnectFromServer()
{
    mSocket.disconnectFromHost();
}

// rpvate slots.
void CQClientTcpEcho::readData()
{
    QByteArray block;
    QString str;
    QDataStream in(&mSocket);
    in.setVersion(QDataStream::Qt_5_9);


    std::cout << "Socket bytes available: " << std::to_string(mSocket.bytesAvailable()) << std::endl;


    in >> block;

    std::cout << "block size: " << std::to_string(block.size()) << std::endl;

//    for(quint64 i = 0; i < 5; i++)
//        {
//            std::cout << block.at(i);
//        }
//    std::cout << std::endl;

    emit receveEcho(QString(block));


//    char chBuff[200];

//    quint64 read_datasize;

//    read_datasize = mSocket.read(chBuff, mDataSize);

//    qDebug() << "Read block size: " << read_datasize;

//    std::cout << "Read chBuff --------------------------->";
//    for(quint64 i = 0; i < read_datasize; i++)
//    {
//        std::cout << chBuff[i];
//    }
//    std::cout << std::endl;


//    emit receveEcho(QString(chBuff));
}

void CQClientTcpEcho::connectionClosedByServer()
{
    qDebug() << "MESSAGE: connectionClosedByServer";
}

void CQClientTcpEcho::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "ERROR: " << socketError;
}

void CQClientTcpEcho::stop()
{
    if(mFlagActive)
    {
        disconnect(&mTimer, &QTimer::timeout, this, &CQClientTcpEcho::connectSeq);
        mTimer.stop();
        mSocket.disconnectFromHost();
        mStateConnectSeq = ST_BEGIN;
        qDebug() << "Disconnect";
        mFlagActive = false;
    }
}

void CQClientTcpEcho::connectSeq()
{
    switch (mStateConnectSeq)
    {
    case ST_BEGIN:
        qDebug() << "Begin Connecting...";
        connect(&mTimer, &QTimer::timeout, this, &CQClientTcpEcho::connectSeq);
        mStateConnectSeq = ST_CONNECTING;
        mTimer.start(250);

    case ST_CONNECTING:
        mSocket.connectToHost(QHostAddress("192.168.0.2"), 1001);
        mStateConnectSeq = ST_MONITOR;

    case ST_MONITOR:

        //Анализ состояния сокета.
        switch (mSocket.state())
        {

        case QAbstractSocket::UnconnectedState:
            qDebug() << "UnconnectedState";
            mSocket.disconnectFromHost();
            qDebug() << "Reconnecting...";
            mStateConnectSeq = ST_CONNECTING;
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << "HostLookupState";
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << "ConnectingState";
            break;
        case QAbstractSocket::ConnectedState:
//            qDebug() << "ConnectedState";
            break;
        case QAbstractSocket::BoundState:
            qDebug() << "BoundState";
            break;
        case QAbstractSocket::ClosingState:
            qDebug() << "ClosingState";
            break;
        case QAbstractSocket::ListeningState:
            qDebug() << "ListeningState";
            break;

        default:
            qDebug() << "Socket state: UNKNOWN";
            break;
        }
        break;
    }


}
