#ifndef LCQREADFROMSOURCEREQ_H_
#define LCQREADFROMSOURCEREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>


class QThread;

class LCQReadFromSourceReq final : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQReadFromSourceReq* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventRead: public CEventBase 
  {
  private:
    const QString& mDataId;
  public:
      CEventRead() = delete;
      CEventRead(const QString& _dataId);
      virtual void handle(LCQReadFromSourceReq* _sender);
  };

  QMutex mMutexEvent;
  QWaitCondition mWaitCond;
  QString mRetData;

public:
  LCQReadFromSourceReq() = delete;
  LCQReadFromSourceReq(QObject*) = delete;

  LCQReadFromSourceReq(
      const QString& _dataId, 
      QThread* _thread, 
      QObject* _parent = nullptr);

  virtual ~LCQReadFromSourceReq();
  QString getData();
  virtual void customEvent(QEvent*); 
};
#endif
