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
  private:
    const QString& mDataId;
    QString& mRetData;
  public:
      CEventRead() = delete;
      CEventRead(const QString& _dataId, QString& _retData);
      virtual void handle(LCQReadFromSourceReq* _sender);
  };

  /* QThread *mpThread; */
  /* QString mDataId; */
  QMutex mMutexEvent;
  QWaitCondition mWaitCond;
  /* QString mRetData; */


  /* LCQReadFromSourceReq(QObject*) = delete; */

  LCQReadFromSourceReq(
      /* const QString& _dataId, */ 
      /* QThread* _thread */
      );

public:
  virtual ~LCQReadFromSourceReq();
  static QSharedPointer<LCQReadFromSourceReq> create();
  QString getData(const QString& _dataId);
private:
  virtual void customEvent(QEvent*) override; 
};
#endif
