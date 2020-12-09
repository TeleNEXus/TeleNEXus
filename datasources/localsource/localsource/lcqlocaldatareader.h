#ifndef LCQLOCALDATAREADER_H_
#define LCQLOCALDATAREADER_H_

#include "lqextendevent.h"
#include "lcqlocalsourcehiden.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"

class LCQLocalDataReader final: public QObject, public LIRemoteDataReader
{
  Q_OBJECT

public:
  //----------------------------------------------------------------------------CQEventDataRead
  class CQEventDataIsRead : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION

  public:
      QSharedPointer<QByteArray> mspData;
      LERemoteDataStatus mStatus;

      explicit CQEventDataIsRead(
          const QByteArray& _data,
          LERemoteDataStatus _status);

      explicit CQEventDataIsRead(LERemoteDataStatus _status);
  };

private:

  QString mDataName;
  QWeakPointer<LIRemoteDataReadListener> mwpReadListener;
  QWeakPointer<LCQLocalSourceHiden> mwpDataSource;
  QWeakPointer<LCQLocalDataReader> mwpThis;

  explicit LCQLocalDataReader() = delete;

  LCQLocalDataReader(
      const QString& _dataName, 
      QSharedPointer<LIRemoteDataReadListener> _readListener,
      QSharedPointer<LCQLocalSourceHiden> _dataSource);

public:
  virtual ~LCQLocalDataReader();

  static QSharedPointer<LCQLocalDataReader> 
    create(
        const QString& _dataName, 
        QSharedPointer<LIRemoteDataReadListener> _readListener,
        QSharedPointer<LCQLocalSourceHiden> _dataSource);

  virtual void readRequest() override;
  virtual void connectToSource() override;
  virtual void disconnectFromSource() override;

public:

  void notifyListener(const QByteArray& _data);
  void notifyListener(LERemoteDataStatus _status);
  QString getDataName(){ return mDataName; }

private:
  virtual void customEvent(QEvent* _event) override;
};

#endif // LCQLOCALDATAREADER_H_
