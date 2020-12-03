#ifndef LCQJSAPPSERVICE_H_
#define LCQJSAPPSERVICE_H_

#include "lqextendevent.h"

#include <QObject>
#include <QSharedPointer>
#include <QEvent>
#include <QMutex>
#include <QWaitCondition>

class QThread;

class LCQJSAppService final : public QObject 
{
  Q_OBJECT
private:
  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQJSAppService* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventReadData
  class CEventReadData : public CEventBase 
  {
  private:
    const QString& edDataId;
    QMutex& edMutex;
  public:
      CEventReadData() = delete;
      CEventReadData(const QString& _dataId, QMutex& _mutex);
      virtual void handle(LCQJSAppService* _sender);
  };

private:
  QThread* mpThread;
  /* QWaitCondition mWaitCond; */
  /* QMutex mMutexEvent; */
  /* QMutex mMutexThread; */

  LCQJSAppService();
  ~LCQJSAppService();
  LCQJSAppService(const LCQJSAppService&) = delete;
  LCQJSAppService& operator=(const LCQJSAppService&) = delete;

public:

  static QSharedPointer<LCQJSAppService> getService();
  QString readSourceData(const QString& _dataId);

private:
  /* virtual void customEvent(QEvent*) override; */
  static void serviceDeleter(LCQJSAppService* _inst);


};
#endif
