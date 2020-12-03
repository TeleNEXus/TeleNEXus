#ifndef LCQREADFROMSOURCEREQ_H_
#define LCQREADFROMSOURCEREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>


class QThread;

class LCQReadFromSourceReq  : public QObject
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
  public:
      CEventRead();
      virtual void handle(LCQReadFromSourceReq* _sender);
  };

  /* QThread *mpThread; */
  /* QMutex mMutexThread; */
  QMutex mMutexEvent;
  QWaitCondition mWaitCond;
  QString mDataId;
  QString mRetData;


  /* LCQReadFromSourceReq(QObject*) = delete; */

  LCQReadFromSourceReq(
      const QString& _dataId
      /* QThread* _thread */
      );

public:
  virtual ~LCQReadFromSourceReq();
  static QSharedPointer<LCQReadFromSourceReq> create(const QString& _dataId);
  QString getData();
private:
  virtual void customEvent(QEvent*) override; 
};
#endif
