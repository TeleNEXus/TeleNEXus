#ifndef LCQREADFROMSOURCEREQ_H_
#define LCQREADFROMSOURCEREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataReader.h"
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataSource.h"

class QThread;

class LCQReadFromSourceReq  : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CReadListener
  class CReadListener : public LIRemoteDataReadListener
  {
  private:
    LCQReadFromSourceReq* mpRequest;
  public:
    CReadListener() =  delete;
    CReadListener(LCQReadFromSourceReq* _req);

    virtual ~CReadListener(){}
    virtual void dataIsRead(
        QSharedPointer<QByteArray> _data, 
        LERemoteDataStatus _status) override;
  };

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

  QString     mSourceId;
  QString     mDataId;
  QByteArray  mRetData;

  QSharedPointer<CReadListener>       mspDataListener;
  QSharedPointer<LIRemoteDataReader>  mspDataReader;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LCQReadFromSourceReq(const QString& _sourceId, const QString& _dataId);

public:
  virtual ~LCQReadFromSourceReq();

  static QSharedPointer<LCQReadFromSourceReq> create(
      const QString& _sourceId,
      const QString& _dataId,
      QThread* _thread);

  QByteArray readData();
private:
  virtual void customEvent(QEvent*) override; 
};
#endif
