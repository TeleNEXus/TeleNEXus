#include "lcqreadfromsourcereq.h"
#include <QCoreApplication>
#include <QDebug>

//==============================================================================CEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQReadFromSourceReq::CEventBase);

LCQReadFromSourceReq::CEventBase::CEventBase() : 
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CEventRead
LCQReadFromSourceReq::CEventRead::CEventRead(const QString& _dataId) :
  mDataId(_dataId)
{
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::CEventRead::handle(LCQReadFromSourceReq* _sender)
{
  _sender->mRetData = QString(
      "LCQReadFromSourceReq::CEventRead::handle data Id = ").arg(mDataId);
}

//==============================================================================
LCQReadFromSourceReq::LCQReadFromSourceReq(
    const QString& _dataId,
    QThread* _thread, QObject* _parent):
  QObject(_parent)
{
  moveToThread(_thread);
  mMutexEvent.lock();
  QCoreApplication::sendEvent(this, new CEventRead(_dataId));
  mWaitCond.wait(&mMutexEvent);
}

//------------------------------------------------------------------------------
LCQReadFromSourceReq::~LCQReadFromSourceReq()
{
}

//------------------------------------------------------------------------------
QString LCQReadFromSourceReq::getData()
{
  return mRetData;
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::customEvent(QEvent* _event)
{
  mMutexEvent.lock();
  mMutexEvent.unlock();

  if(_event->type() == CEventBase::msExtendedEventType)
  {
    CEventBase* e = dynamic_cast<CEventBase*>(_event);
    if(e == nullptr)
    {
      qDebug() << "LCQReadFromSourceReq::customEvent dynamic cast err";
      return;
    }
    qDebug() << "LCQReadFromSourceReq::customEvent";
    e->handle(this);
  }
}

