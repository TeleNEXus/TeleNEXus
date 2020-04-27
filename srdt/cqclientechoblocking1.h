#ifndef CQCLIENTECHOBLOCKING1_H
#define CQCLIENTECHOBLOCKING1_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
class CQClientEchoBlocking1 : public QThread
{
    Q_OBJECT
public:
    explicit CQClientEchoBlocking1(QObject *parent = nullptr);
    virtual void run() override;
    void echo(const QString& strin, QString& echostr);
signals:
private:
    QWaitCondition  mWaitCond;
    QMutex          mMutex1;
    QMutex          mMutex2;
    QThread         *mThreadClient;
    QString         mInputString;
    QString         mEchoString;
};

#endif // CQCLIENTECHOBLOCKING1_H
