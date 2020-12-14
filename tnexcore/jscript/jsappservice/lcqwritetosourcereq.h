#ifndef LCQWRITETOSOURCE_H_
#define LCQWRITETOSOURCE_H_

#include "lqextendevent.h"

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

#include "LIRemoteDataWriteListener.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class QThread;

class LCQWriteToSource  : public QObject
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CWriteListener
  /* class CWriteListener : public LIRemoteDataWriteListener */
  /* { */
  /* private: */
  /*   LCQWriteToSource* mpRequest; */
  /* public: */
  /*   CWriteListener() =  delete; */
  /*   CWriteListener(LCQWriteToSource* _req); */

  /*   virtual ~CWriteListener(){} */
  /*   virtual void dataIsWrite(LERemoteDataStatus _status); */
  /* }; */

  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
  __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQWriteToSource* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventBase
  class CEventWrite: public CEventBase 
  {
  public:
      CEventWrite();
      virtual void handle(LCQWriteToSource* _sender);
  };

  QString     mSourceId;
  QString     mDataId;

  const QByteArray& edWriteData;

  int mWriteDataSize;

  /* QSharedPointer<CWriteListener>        mspDataListener; */
  QSharedPointer<LIRemoteDataWriter>    mspDataWriter;

  QMutex          mMutexEvent;
  QWaitCondition  mWaitCond;

  LCQWriteToSource(
      const QString&      _sourceId, 
      const QString&      _dataId,
      const QByteArray&   _writeData
      );

public:
  virtual ~LCQWriteToSource();

  static QSharedPointer<LCQWriteToSource> create(
      const QString&      _sourceId,
      const QString&      _dataId,
      const QByteArray&   _writeData,
      QThread* _thread);

  int writeData();

private:
  virtual void customEvent(QEvent*) override; 
};
#endif
