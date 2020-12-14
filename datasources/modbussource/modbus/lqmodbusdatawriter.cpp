#include "lqmodbusdatawriter.h"
#include "lqmodbusdatasource.h"
#include "LIRemoteDataWriteListener.h"

#include <QDebug>

#include <QCoreApplication>

//==============================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataWriter::CQEventDataIsWrite);

LQModbusDataWriter::
CQEventDataIsWrite::
CQEventDataIsWrite(LERemoteDataStatus _status) : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
  mStatus(_status)
{
}

//==============================================================================
LQModbusDataWriter::LQModbusDataWriter(
    const QString& _dataName,
    LTWriteListener _writeListener,
    QSharedPointer<LQModbusDataSource> _dataSource) : 
  QObject(nullptr),
  mDataName(_dataName),
  mListener(_writeListener),
  mwpDataSource(_dataSource)
{

}
/* LQModbusDataWriter::LQModbusDataWriter( */
/*     const QString& _dataName, */
/*     QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
/*     QSharedPointer<LQModbusDataSource> _dataSource) : */ 
/*   QObject(nullptr), */
/*   mDataName(_dataName), */
/*   mwpWriteListener(_writeListener), */
/*   mwpDataSource(_dataSource) */
/*   { */

/*   } */

//------------------------------------------------------------------------------
LQModbusDataWriter::~LQModbusDataWriter()
{
}

//==============================================================================create
static void doDeleteLater(LQModbusDataWriter* _writer)
{
  qDebug() << "LQModbusDataWriter  doDeleteLater";
  _writer->deleteLater();
}
//------------------------------------------------------------------------------
QSharedPointer<LQModbusDataWriter> LQModbusDataWriter::create(
    const QString& _dataName,
    LTWriteListener _writeListener,
    QSharedPointer<LQModbusDataSource> _dataSource)
{
  auto sp  = QSharedPointer<LQModbusDataWriter>(
      new LQModbusDataWriter(_dataName, _writeListener, _dataSource),
      doDeleteLater);
  sp->mwpThis = sp;
  return sp;
}
/* QSharedPointer<LQModbusDataWriter> LQModbusDataWriter::create( */
/*     const QString& _dataName, */
/*     QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
/*     QSharedPointer<LQModbusDataSource> _dataSource) */
/* { */
/*   auto sp  = QSharedPointer<LQModbusDataWriter>( */
/*       new LQModbusDataWriter(_dataName, _writeListener, _dataSource), */
/*       doDeleteLater); */
/*   sp->mwpThis = sp; */
/*   return sp; */
/* } */

//------------------------------------------------------------------------------
void LQModbusDataWriter::writeRequest(const QByteArray& _data)
{
  auto sp = mwpDataSource.lock();

  if(sp.isNull())
  {
    mListener(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    sp.data()->write(mwpThis.lock(), _data);
  }
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::notifyListener(LERemoteDataStatus _status)
{
  QCoreApplication::postEvent(this, new CQEventDataIsWrite(_status));
}

//------------------------------------------------------------------------------
void LQModbusDataWriter::customEvent(QEvent *_event)
{
  if(_event->type() == CQEventDataIsWrite::msExtendedEventType)
  {
    CQEventDataIsWrite *e = dynamic_cast<CQEventDataIsWrite*>(_event);
    if(e == nullptr)
    {
      mListener(LERemoteDataStatus::DS_WRONG);
    }
    else
    {
      mListener(e->mStatus);
    }
  }
}

