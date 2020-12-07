#include "lcqlocaldatareader.h"
#include "LIRemoteDataReadListener.h"

#include <QDebug>
//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataReader::CQEventDataIsRead);

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead(
        const QByteArray& _data,
        LERemoteDataStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray(_data)),
    mStatus(_status)
{
}

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead( 
        LERemoteDataStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================LCQRemoteDataListener
LCQLocalDataReader::LCQLocalDataReader(
        QWeakPointer<LCQLocalSourceHiden> _dataSource, 
        QObject* _parent) :   
    QObject(_parent),
    mDataSource(_dataSource)
{
}

LCQLocalDataReader::~LCQLocalDataReader()
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQLocalSourceHiden*)sp.data())->disconnectReader(this);
}

//------------------------------------------------------------------------------setDataName
void LCQLocalDataReader::setDataName(const QString& _dataName)
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(!sp.isNull())
    {
        ((LCQLocalSourceHiden*)sp.data())->disconnectReader(this);
    }
    mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setDataReadListener(
        QWeakPointer<LIRemoteDataReadListener> _listener)
{
    QSharedPointer<LIRemoteDataReadListener> sp = _listener.lock();
    if(!sp.isNull()) mpReadListener = sp;
}

//------------------------------------------------------------------------------readRequest
void LCQLocalDataReader::readRequest()
{
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQLocalSourceHiden*)sp.data())->read(mDataName, this);
}

//------------------------------------------------------------------------------connectToSource
void LCQLocalDataReader::connectToSource()
{
    qDebug() << "LCQLocalDataReader::connectToSource:" << mDataName;
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQLocalSourceHiden*)sp.data())->connectReader(mDataName, this);
}

//------------------------------------------------------------------------------disconnectFromSource
void LCQLocalDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    QSharedPointer<LIRemoteDataSource> sp = mDataSource.lock();
    if(sp.isNull()) return;
    ((LCQLocalSourceHiden*)sp.data())->disconnectReader(this);
}

//------------------------------------------------------------------------------customEvent
void LCQLocalDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    QSharedPointer<LIRemoteDataReadListener> listener = mpReadListener.lock();
    if(!listener.isNull())
    {
        listener->dataIsRead(e->mData, e->mStatus);
    }
}

