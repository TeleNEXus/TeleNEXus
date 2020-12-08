#include "lcqlocalsourcehiden.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include <QCoreApplication>

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalSourceHiden::CEventBase);

LCQLocalSourceHiden::CEventBase::CEventBase() :
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventConnectReader
LCQLocalSourceHiden::CEventConnectReader::CEventConnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventConnectReader::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.connectReader(mspDataReader);
}

//==============================================================================CEventDisconnectReader
LCQLocalSourceHiden::CEventDisconnectReader::CEventDisconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventDisconnectReader::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.disconnectReader(mspDataReader);
}

//==============================================================================CEventRead
LCQLocalSourceHiden::CEventRead::CEventRead(
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventRead::handle(
    LCQLocalSourceHiden* _sender)
{
  QByteArray data = _sender->mDataMap.getData(mspDataReader->getDataName());

  if(data.isNull())
  {
    mspDataReader->notifyListener(LERemoteDataStatus::DS_UNDEF);
  }

  mspDataReader->notifyListener(data);
}

//==============================================================================CEventWrite
LCQLocalSourceHiden::CEventWrite::CEventWrite(
    QSharedPointer<LCQLocalDataWriter> _sp_writer) :
  mspDataWriter(_sp_writer)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventWrite::handle(
    LCQLocalSourceHiden* _sender)
{
  int ds = _sender->mDataMap.setData(mspDataWriter->getDataName());
}

//==============================================================================LCQLocalSourceHiden
LCQLocalSourceHiden::LCQLocalSourceHiden() :
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
LCQLocalSourceHiden::~LCQLocalSourceHiden()
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addItem(const QString& _id, const QByteArray& _data)
{
  mDataMap.addItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addItem(const QString& _id, const QBitArray& _data)
{
  mDataMap.addItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::customEvent(QEvent* _event)
{
  if(_event->type() != CEventBase::msExtendedEventType) return;
  CEventBase* e = dynamic_cast<CEventBase*>(_event);
  if(e == nullptr) return;
  e->handle(this);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::connectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  CEventConnectReader* event = new CEventConnectReader(_sp_reader);
  QCoreApplication::postEvent(this, event);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::read(
    QSharedPointer<LCQLocalDataReader> _ps_reader)
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::write(
    const QByteArray& _data, 
    QSharedPointer<LCQLocalDataWriter> _sp_writer)
{
}



