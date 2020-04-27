#ifndef CQTESTIMER_H
#define CQTESTIMER_H

#include <QObject>
#include <QTimer>
#include <QThread>

class CQTestTimer : public QObject
{
    Q_OBJECT
public:
    explicit CQTestTimer(QObject *parent = nullptr);
    virtual ~CQTestTimer();
    virtual void moveToThread(QThread *thread);
public slots:
    void startSeq(QThread* _thread = nullptr);
    void stopSeq();
private slots:
    void seq();
signals:

private:
    QTimer mTimer;
    short mStateSeq;
    volatile bool mFlagStopSeq;
};

#endif // CQTESTIMER_H
