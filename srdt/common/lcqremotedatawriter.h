#ifndef LCQREMOTEDATAWRITER_H
#define LCQREMOTEDATAWRITER_H

#include <QObject>
#include <QWeakPointer>
#include "lcqremotedatasourcebase.h"

class LCQRemoteDataWriter : public QObject
{
    Q_OBJECT
private:
    QString mDataName;
    QWeakPointer<LCQRemoteDataSourceBase> mDataSource;
public:
    explicit LCQRemoteDataWriter(QObject *_parent = nullptr);
    explicit LCQRemoteDataWriter(   const QString& _dataName,
                                    QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                                    QObject* _parent = nullptr);
    void setDataName(const QString& _dataName);
    void setDataSource(QWeakPointer<LCQRemoteDataSourceBase> _dataSource);
signals:
    void dataIsWrite(LCQRemoteDataSourceBase::EDataStatus _status);
public slots:
    void writeRequest(const QByteArray& _data);
private:
    virtual void customEvent(QEvent *_event) override;
};

#endif // LCQREMOTEDATAWRITER_H
