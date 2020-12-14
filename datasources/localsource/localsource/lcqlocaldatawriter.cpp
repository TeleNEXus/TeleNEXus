#include "lcqlocaldatawriter.h"
#include "lcqlocalsourcehiden.h"
#include "LIRemoteDataWriteListener.h"
#include <QCoreApplication>

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
    const QString& _dataName,
    LTWriteListener _writeListener,
    /* QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
    QSharedPointer<LCQLocalSourceHiden> _dataSource):
  QObject(nullptr),
  mDataName(_dataName),
  /* mspWriteListener(_writeListener), */
  mListener(_writeListener),
  mwpDataSource(_dataSource)
{
}

//------------------------------------------------------------------------------
LCQLocalDataWriter::~LCQLocalDataWriter()
{
}

//==============================================================================
static void pointerDeleter(QObject* _obj)
{
  _obj->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataWriter> LCQLocalDataWriter::create(
    const QString& _dataName,
    LTWriteListener _writeListener,
    /* QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
    QSharedPointer<LCQLocalSourceHiden> _dataSource)
{
  auto sp_writer = 
    QSharedPointer<LCQLocalDataWriter>(
        new LCQLocalDataWriter(_dataName, _writeListener, _dataSource), 
        pointerDeleter);
  sp_writer->mwpThis = sp_writer;
  return sp_writer;
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::writeRequest(const QByteArray& _data)
{
  auto sp = mwpDataSource.lock();
  if(sp.isNull())
  {
    /* if(!mspWriteListener.isNull()) */ 
    /*   mspWriteListener->dataIsWrite(LERemoteDataStatus::DS_WRONG); */
    mListener(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp->write(_data, mwpThis);
  }
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsWrite(_status));
}

//------------------------------------------------------------------------------
void LCQLocalDataWriter::customEvent(QEvent *_event)
{
  if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
  {
    /* if(!mspWriteListener.isNull()) */
    /* { */
      CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
      if(e == nullptr)
      {
        /* mspWriteListener->dataIsWrite(LERemoteDataStatus::DS_WRONG); */
        mListener(LERemoteDataStatus::DS_WRONG);
      }
      else
      {
        mListener(e->mStatus);
        /* mspWriteListener->dataIsWrite(e->mStatus); */
      }
    /* } */
  }
}

