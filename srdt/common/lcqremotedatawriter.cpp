#include "lcqremotedatawriter.h"
#include <QSharedDataPointer>
#include <QDebug>

//===================================================================================================LCQRemoteDataWriter
LCQRemoteDataWriter::LCQRemoteDataWriter(QObject *_parent) : QObject(_parent)
{

}

LCQRemoteDataWriter::LCQRemoteDataWriter(   const QString& _dataName,
                                            QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                                            QObject* _parent) : QObject(_parent),
                                                                mDataName(_dataName),
                                                                mDataSource(_dataSource)
{

}

//-----------------------------------------------------------------------------------------------------------setDataName
void LCQRemoteDataWriter::setDataName(const QString& _dataName)
{
    mDataName = _dataName;
}

//---------------------------------------------------------------------------------------------------------setDataSource
void LCQRemoteDataWriter::setDataSource(QWeakPointer<LCQRemoteDataSourceBase> _dataSource)
{
    mDataSource = _dataSource;
}

//----------------------------------------------------------------------------------------------------------writeRequest
void LCQRemoteDataWriter::writeRequest(const QByteArray& _data)
{
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(sp.isNull()) emit dataIsWrite(LCQRemoteDataSourceBase::EDataStatus::DS_WRONG);
    else sp->write(mDataName, _data, this);
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQRemoteDataWriter::customEvent(QEvent *_event)
{
    if(_event->type() == LCQRemoteDataSourceBase::CQEventDataIsWrite::msExtendedEventType)
    {
        LCQRemoteDataSourceBase::CQEventDataIsWrite *e =
                dynamic_cast<LCQRemoteDataSourceBase::CQEventDataIsWrite*>(_event);
        if(e == nullptr) emit dataIsWrite(LCQRemoteDataSourceBase::EDataStatus::DS_WRONG);
        else emit dataIsWrite(e->mStatus);
    }
}
