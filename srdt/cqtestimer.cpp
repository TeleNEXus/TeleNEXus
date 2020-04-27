#include "cqtestimer.h"
#include <QDebug>
#include <iostream>
CQTestTimer::CQTestTimer(QObject *parent) : QObject(parent), mTimer(this), mStateSeq(0), mFlagStopSeq(false)
{


}

CQTestTimer::~CQTestTimer()
{
    mTimer.stop();
}


void CQTestTimer::moveToThread(QThread *thread)
{
    if(thread != nullptr) thread = nullptr;
    qDebug() << "CQTestTimer::moveToThread is disabled";
}

//
void CQTestTimer::startSeq(QThread* _thread)
{
    if(mTimer.isActive()) return;

    if(_thread != nullptr)
    {
        ((QObject*)this)->moveToThread(_thread);

        connect(_thread, &QThread::finished, [=](){
            qDebug() << "Stop Timer";
            mTimer.stop();});
    }
    connect(&mTimer, &QTimer::timeout, this, &CQTestTimer::seq);
    QTimer::singleShot(250, this, [=](){std::cout << "Start Timer"; mTimer.start(500);});
}

void CQTestTimer::stopSeq()
{
    mFlagStopSeq = true;
}

volatile quint64 __gTestCounter = 0;
void CQTestTimer::seq()
{
    quint16 t;
    switch(mStateSeq)
    {
    case 0:
        std::cout << "Sequence state: 0" << std::endl;

        for(quint64 i = 0; i <500000000; i++)
        {
            if(mFlagStopSeq) break;
            __gTestCounter++;
            t = __gTestCounter/100;
            t = __gTestCounter*100;
            t = __gTestCounter/100;
            t = __gTestCounter*100;
            t = __gTestCounter/100;
            t = __gTestCounter*100;
            t = __gTestCounter/100;
            t = __gTestCounter*100;
            t = __gTestCounter/100;
            t = __gTestCounter*100;
        }
        mStateSeq = 1;
        break;
    case 1:
        std::cout << "Sequence state: 1" << std::endl;
        mStateSeq = 0;
        break;
    }

    if(mFlagStopSeq)
    {
        mTimer.stop();
        mFlagStopSeq = false;
    }
}
