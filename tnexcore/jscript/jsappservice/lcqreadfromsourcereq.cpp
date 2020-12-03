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
LCQReadFromSourceReq::CEventRead::CEventRead(const QString& _dataId,
    QString& _retData) :
  mDataId(_dataId),
  mRetData(_retData)
{
}

//------------------------------------------------------------------------------
void LCQReadFromSourceReq::CEventRead::handle(LCQReadFromSourceReq* _sender)
{
  Q_UNUSED(_sender)
  mRetData = QString(
      "LCQReadFromSourceReq::CEventRead::handle data Id = ").arg(mDataId);
}

//==============================================================================
QSharedPointer<LCQReadFromSourceReq> LCQReadFromSourceReq::create()
{
  return QSharedPointer<LCQReadFromSourceReq>(new LCQReadFromSourceReq);
}

LCQReadFromSourceReq::LCQReadFromSourceReq() :
    /* const QString& _dataId, */
    /* QThread* _thread): */
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
LCQReadFromSourceReq::~LCQReadFromSourceReq()
{
}

//------------------------------------------------------------------------------
QString LCQReadFromSourceReq::getData(const QString& _dataId)
{
  /* moveToThread(_thread); */
  QString ret_data;
  mMutexEvent.lock();
  QCoreApplication::sendEvent(this, new CEventRead(_dataId, ret_data));
  mWaitCond.wait(&mMutexEvent);
  mMutexEvent.unlock();
  return ret_data;
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

