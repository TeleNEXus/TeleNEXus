#include "lcqlocalsourcehiden.h"

//==============================================================================
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQLocalSourceHiden::CEventBase);

LCQLocalSourceHiden::CEventBase::CEventBase() :
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================
LCQLocalSourceHiden::CEventConnectReader::CEventConnectReader(
    LCQLocalSourceHiden* _sender,
    QSharedPointer<LCQLocalDataReader> _sp_reader) :
  mpSender(_sender),
  mspDataReader(_sp_reader)
{}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::CEventConnectReader::handle()
{
}

//==============================================================================
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
  e->handle();
}
