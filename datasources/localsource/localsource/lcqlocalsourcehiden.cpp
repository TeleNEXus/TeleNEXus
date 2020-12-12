#include "lcqlocalsourcehiden.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include <QCoreApplication>
#include <QDebug>

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
  _sender->mDataMap.readData(mspDataReader);
}

//==============================================================================CEventWrite
LCQLocalSourceHiden::CEventWrite::CEventWrite(
    QSharedPointer<LCQLocalDataWriter> _sp_writer,
    const QByteArray& _data
    ) :
  mspDataWriter(_sp_writer),
  mData(_data)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventWrite::handle(
    LCQLocalSourceHiden* _sender)
{
  _sender->mDataMap.writeData(mspDataWriter, mData);
}

//==============================================================================
void pointerDeleter(QObject* _obj)
{
  _obj->deleteLater();
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
QSharedPointer<LCQLocalSourceHiden> LCQLocalSourceHiden::create()
{
  auto sp = QSharedPointer<LCQLocalSourceHiden>(
      new LCQLocalSourceHiden(),
      pointerDeleter);
  sp->mwpThis= sp;
  return sp;
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
QSharedPointer<LCQLocalDataReader> LCQLocalSourceHiden::createReader(
    const QString& _dataName, 
    QSharedPointer<LIRemoteDataReadListener> _listener)
{
  return LCQLocalDataReader::create(_dataName, _listener, mwpThis);
}

//------------------------------------------------------------------------------
QSharedPointer<LCQLocalDataWriter> LCQLocalSourceHiden::createWriter(
    const QString& _dataName, 
    QSharedPointer<LIRemoteDataWriteListener> _listener)
{
  return LCQLocalDataWriter::create(_dataName, _listener, mwpThis);
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
  QCoreApplication::postEvent(this, new CEventConnectReader(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::disconnectReader(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QCoreApplication::postEvent(this, new CEventDisconnectReader(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::read(
    QSharedPointer<LCQLocalDataReader> _sp_reader)
{
  QCoreApplication::postEvent(this, new CEventRead(_sp_reader));
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::write(
    const QByteArray& _data, 
    QSharedPointer<LCQLocalDataWriter> _sp_writer)
{
  QCoreApplication::postEvent(this, new CEventWrite(_sp_writer, _data));
}



