#ifndef LCQREADFROMSOURCEREQ_H_
#define LCQREADFROMSOURCEREQ_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataWriteListener.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class QThread;

class LCQWtiteToSource  : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CWriteListener
  class CWriteListener : public LIRemoteDataWriteListener
  {
  private:
    LCQWtiteToSource* mpRequest;
  public:
    CWriteListener() =  delete;
    CWriteListener(LCQWtiteToSource* _req);

    virtual ~CWriteListener(){}
    virtual void dataIsWrite(LERemoteDataStatus _status);
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQWtiteToSource* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventWrite: public CEventBase 
  {
  public:
      CEventWrite();
      virtual void handle(LCQWtiteToSource* _sender);
  };

  QString     mSourceId;
  QString     mDataId;

  const QByteArray& edWriteData;

  int mWriteDataSize;

  QSharedPointer<CWriteListener>        mspDataListener;
  QSharedPointer<LIRemoteDataWriter>    mspDataWriter;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LCQWtiteToSource(
      const QString&      _sourceId, 
      const QString&      _dataId,
      const QByteArray&   _writeData
      );

public:
  virtual ~LCQWtiteToSource();

  static QSharedPointer<LCQWtiteToSource> create(
      const QString&      _sourceId,
      const QString&      _dataId,
      const QByteArray&   _writeData,
      QThread* _thread);

  int writeData();

private:
  virtual void customEvent(QEvent*) override; 
};
#endif
