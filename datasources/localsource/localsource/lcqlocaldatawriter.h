#ifndef LCQLOCALDATAWRITER_H
#define LCQLOCALDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class LCQLocalSourceHiden;

class LCQLocalDataWriter final : public QObject, public LIRemoteDataWriter
{
  Q_OBJECT
private:
    //--------------------------------------------------------------------------
  class CQEventDataIsWrite : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      const LERemoteDataStatus mStatus;
      explicit CQEventDataIsWrite(LERemoteDataStatus _status);
  };

  QString mDataName;
  QWeakPointer<LIRemoteDataWriteListener> mwpWriteListener;
  QWeakPointer<LCQLocalSourceHiden> mwpDataSource;
  QWeakPointer<LCQLocalDataWriter> mwpThis;

private:
  LCQLocalDataWriter() = delete;
  LCQLocalDataWriter(
      const QString& _dataName,
      QSharedPointer<LIRemoteDataWriteListener> _writeListener,
      QSharedPointer<LCQLocalSourceHiden> _dataSource);

public:
  static QSharedPointer<LCQLocalDataWriter> create(
      const QString& _dataName,
      QSharedPointer<LIRemoteDataWriteListener> _writeListener,
      QSharedPointer<LCQLocalSourceHiden> _dataSource);

  virtual void writeRequest(const QByteArray& _data) override;

  QString getDataName(){ return mDataName; }
  void notifyListener(LERemoteDataStatus _status);

private:
  virtual void customEvent(QEvent *_event) override;
};


#endif // LCQLOCALDATAWRITER_H
