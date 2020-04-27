#include "cqclientechoblocking1.h"
#include "QDebug"

CQClientEchoBlocking1::CQClientEchoBlocking1(QObject *parent) : QThread(parent),
    mThreadClient(nullptr)
{

}

void CQClientEchoBlocking1::run()
{
    quint16 counter = 0;
    forever
    {

        mMutex1.lock();
        for(int i = 0; i < 10; i++)
        {
            qDebug() << mInputString << "Pass :" << counter;
            counter++;
            QThread::msleep(50);
        }

        mEchoString = mInputString +" " + QString::number(counter);
        mWaitCond.wakeOne();
        mWaitCond.wait(&mMutex1);
        mMutex1.unlock();
    }
}

void CQClientEchoBlocking1::echo(const QString& strin, QString& echostr)
{

    QMutexLocker l(&mMutex1);
//    mMutex1.lock();
//    qDebug() << "echo1";
    qDebug() << strin << "start";
    mInputString  = strin;
    if(!isRunning())
    {
        start();
    }
    else
    {
        mWaitCond.wakeOne();
        mWaitCond.wait(&mMutex1);
    }
    qDebug() << mEchoString << "stop";
//    qDebug() << strin << "stop";


//    qDebug() << mEchoString;
//    mMutex1.unlock();

//    mMutex2.lock();
//    qDebug() << "echo2";

//    qDebug() << "echo3";
//    mWaitCond.wait(&mMutex2);
//    qDebug() << "echo4";

//    mMutex.unlock();
//    mMutex1.unlock();
}
