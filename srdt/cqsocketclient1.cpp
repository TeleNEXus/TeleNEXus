#include "cqsocketclient1.h"
#include <QDebug>
#include <QHostAddress>
#include <QThread>
#include <QDataStream>
#include <iostream>


CQSocketClient::CQSocketClient(QObject *parent) : QObject(parent), mFlagFinish(false)
{

}

void CQSocketClient::run()
{
    QTcpSocket      mSocket;
    char chBuff[100];
    int testcounter = 0;
    std::string strBuff;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QByteArray blockRead;
    QDataStream inread(&blockRead, QIODevice::ReadWrite);
    inread.setVersion(QDataStream::Qt_5_9);

    mSocket.connectToHost(QHostAddress("192.168.0.2"), 1001, QIODevice::ReadWrite);

    if(mSocket.waitForConnected(2500))
    {
        qDebug() << "Connected to Socket Addr: " << mSocket.peerAddress().toString() << "; port: " << QString::number(mSocket.peerPort());
    }

    while(!mFlagFinish)
    {
        out.device()->seek(0);
        strBuff = std::string("Test ") + std::to_string(testcounter) ;
        out.writeRawData(strBuff.data(), strBuff.length());

        qDebug() << "Write Block size: " << block.size();
        mSocket.write(block);

        if(mSocket.waitForBytesWritten(3000))
        {
            qDebug() << "mSocket.waitForBytesWritten Ok";
        }

        quint64 read_datasize = 0;
        read_datasize = mSocket.read(chBuff, block.size());
        if (mSocket.waitForReadyRead(1000))
        {
            qDebug() << "mSocket.waitForReadyRead Ok";
        }

        qDebug() << "Read block size: " << read_datasize;

        std::cout << "Read chBuff --------------------------->";
        for(quint64 i = 0; i < read_datasize; i++)
        {
            std::cout << chBuff[i];
        }
        std::cout << std::endl;
        testcounter++;
        QThread::msleep(250);
    }

    qDebug() << QString("End socket client process");
    emit finished();
}

void CQSocketClient::finish()
{
    mFlagFinish = true;
}
