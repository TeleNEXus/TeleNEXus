#include "lcqremotedatareader.h"
#include <QDebug>
//=================================================================================================LCQRemoteDataListener
LCQRemoteDataReader::LCQRemoteDataReader(QObject* _parent) :   QObject(_parent)
{
}

LCQRemoteDataReader::
    LCQRemoteDataReader(  const QString& _dataName,
                            QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                            QObject* _parent) : QObject(_parent),
                                                mDataName(_dataName),
                                                mDataSource(_dataSource)
{

}

LCQRemoteDataReader::~LCQRemoteDataReader()
{
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------setDataName
void LCQRemoteDataReader::setDataName(const QString& _dataName)
{
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        sp->disconnectReader(this);
    }
    mDataName = _dataName;
}

//---------------------------------------------------------------------------------------------------------setDataSource
void LCQRemoteDataReader::setDataSource(QWeakPointer<LCQRemoteDataSourceBase> _dataSource)
{
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        sp->disconnectReader(this);
    }
    mDataSource = _dataSource;
}

//-----------------------------------------------------------------------------------------------------------readRequest
void LCQRemoteDataReader::readRequest()
{
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp->read(mDataName, this);
}

//-------------------------------------------------------------------------------------------------------connectToSource
void LCQRemoteDataReader::connectToSource()
{
    qDebug() << "LCQRemoteDataReader::connectToSource:" << mDataName;
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp->connectReader(mDataName, this);
}

//--------------------------------------------------------------------------------------------------disconnectFromSource
void LCQRemoteDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    QSharedPointer<LCQRemoteDataSourceBase> sp = mDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(this);
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQRemoteDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != LCQRemoteDataSourceBase::CQEventDataIsRead::msExtendedEventType) return;
    LCQRemoteDataSourceBase::CQEventDataIsRead *e = dynamic_cast<LCQRemoteDataSourceBase::CQEventDataIsRead*>(_event);
    if(e == nullptr) return;
    emit dataIsRead(e->mData, e->mStatus);
}
