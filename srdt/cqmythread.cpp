#include "cqmythread.h"

CQMyThread::CQMyThread(QObject *parent) : QThread(parent)
{

}
volatile int i;
void CQMyThread::run()
{
    exec();
    i++;
}
