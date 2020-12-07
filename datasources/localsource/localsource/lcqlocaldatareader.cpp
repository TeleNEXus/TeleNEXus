#include "lcqlocaldatareader.h"
#include "LIRemoteDataReadListener.h"

#include <QDebug>

//==============================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataReader::CQEventDataIsRead);

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead(
        const QByteArray& _data,
        LERemoteDataStatus _status) : 
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray(_data)),
    mStatus(_status)
{
}

LCQLocalDataReader::CQEventDataIsRead::CQEventDataIsRead( 
        LERemoteDataStatus _status) :  
    QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
    mspData(new QByteArray),
    mStatus(_status)
{
}

//==============================================================================LCQRemoteDataListener
LCQLocalDataReader::LCQLocalDataReader( 
    QWeakPointer<LCQLocalSourceHiden> _dataSource) :   
  QObject(nullptr),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LCQLocalDataReader::~LCQLocalDataReader()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->disconnectReader(this);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setDataName(const QString& _dataName)
{
    auto sp = mwpDataSource.lock();
    if(!sp.isNull()) sp->disconnectReader(this);
    mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::setDataReadListener(
        QWeakPointer<LIRemoteDataReadListener> _listener)
{
    auto sp = _listener.lock();
    if(!sp.isNull()) mwpReadListener = sp;
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::readRequest()
{
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp->read(this);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::connectToSource()
{
    qDebug() << "LCQLocalDataReader::connectToSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->connectReader(this);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::disconnectFromSource()
{
    qDebug() << "disconnectFromSource:" << mDataName;
    auto sp = mwpDataSource.lock();
    if(sp.isNull()) return;
    sp.data()->disconnectReader(this);
}

//------------------------------------------------------------------------------
void LCQLocalDataReader::customEvent(QEvent* _event)
{
    if(_event->type() != CQEventDataIsRead::msExtendedEventType) return;

    CQEventDataIsRead *e = dynamic_cast<CQEventDataIsRead*>(_event);

    if(e == nullptr) return;

    auto listener = mwpReadListener.lock();

    if(!listener.isNull())
    {
        listener->dataIsRead(e->mspData, e->mStatus);
    }
}

