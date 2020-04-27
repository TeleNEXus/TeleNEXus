#ifndef LCQREMOTEDATAREADER_H
#define LCQREMOTEDATAREADER_H

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include "lqextendevent.h"
#include "lcqremotedatasourcebase.h"

class LCQRemoteDataReader : public QObject
{
    Q_OBJECT
private:
    QString mDataName;
    QWeakPointer<LCQRemoteDataSourceBase> mDataSource;
public:
    LCQRemoteDataReader(  QObject* _parent = nullptr);
    LCQRemoteDataReader(  const QString& _dataName,
                            QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                            QObject* _parent = nullptr);
    virtual ~LCQRemoteDataReader();
    void setDataName(const QString& _dataName);
    void setDataSource(QWeakPointer<LCQRemoteDataSourceBase> _dataSource);
signals:
    void dataIsRead(QSharedPointer<QByteArray> _data,
                    LCQRemoteDataSourceBase::EDataStatus _status);
public slots:
    void readRequest();
    void connectToSource();
    void disconnectFromSource();
private:
    virtual void customEvent(QEvent* _event) override;
};

#endif // LCQREMOTEDATAREADER_H
