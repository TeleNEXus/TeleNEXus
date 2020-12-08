#include "lcqlocaldatawriter.h"
#include "lcqlocalsourcehiden.h"
#include "LIRemoteDataWriteListener.h"


//==============================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalDataWriter::CQEventDataIsWrite);

LCQLocalDataWriter::CQEventDataIsWrite::CQEventDataIsWrite(
    LERemoteDataStatus _status) : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
  mStatus(_status)
{
}

//==============================================================================
LCQLocalDataWriter::LCQLocalDataWriter(
    QWeakPointer<LCQLocalSourceHiden> _dataSource) : 
  QObject(nullptr),
  mspDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::setDataName(const QString& _dataName)
{
  mDataName = _dataName;
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::
setDataWriteListener(QWeakPointer<LIRemoteDataWriteListener> _listener)
{
  mwpWriteListener = _listener.lock();
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::writeRequest(const QByteArray& _data)
{
  auto sp = mspDataSource.lock();
  if(sp.isNull())
  {
    auto listener = mwpWriteListener.lock();

    if(!listener.isNull()) listener->dataIsWrite(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp->write(_data, this);
  }
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::customEvent(QEvent *_event)
{
  if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
  {

    auto listener = mwpWriteListener.lock();

    if(!listener.isNull())
    {
      CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
      if(e == nullptr)
      {
        listener->dataIsWrite(LERemoteDataStatus::DS_WRONG);
      }
      else
      {
        listener->dataIsWrite(e->mStatus);
      }
    }
  }
}

