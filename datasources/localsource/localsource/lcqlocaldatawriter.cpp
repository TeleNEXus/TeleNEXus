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
LCQLocalDataWriter::LCQLocalDataWriter():
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataWriter> LCQLocalDataWriter::create(
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp_writer = QSharedPointer<LCQLocalDataWriter>(new LCQLocalDataWriter());
  sp_writer->mwpDataSource = _dataSource;
  sp_writer->mwpThis = sp_writer;
  return sp_writer;
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
  auto sp = mwpDataSource.lock();
  if(sp.isNull())
  {
    auto listener = mwpWriteListener.lock();

    if(!listener.isNull()) listener->dataIsWrite(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp->write(_data, mwpThis);
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

