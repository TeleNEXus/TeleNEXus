#ifndef CQMYTHREAD_H
#define CQMYTHREAD_H

#include <QThread>


class CQMyThread : public QThread
{
    Q_OBJECT
public:
    explicit CQMyThread(QObject *parent = nullptr);
protected:
    void run() override;
signals:

};

#endif // CQMYTHREAD_H
