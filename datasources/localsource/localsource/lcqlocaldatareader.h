#ifndef LCQLOCALDATAREADER_H_
#define LCQLOCALDATAREADER_H_

#include "lqextendevent.h"
#include "lcqlocalsourcehiden.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"


class LCQLocalDataReader final: public QObject, public LIRemoteDataReader
{
  Q_OBJECT
private:

  //----------------------------------------------------------------------------CQEventDataRead
  class CQEventDataIsRead : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION

  public:
      QSharedPointer<QByteArray> mspData;
      LERemoteDataStatus mStatus;

      explicit CQEventDataIsRead(
          QSharedPointer<QByteArray> _data, 
          LERemoteDataStatus _status);

      explicit CQEventDataIsRead(LERemoteDataStatus _status);
  };

  QString mDataName;
  QWeakPointer<LCQLocalSourceHiden> mwpDataSource;
  QWeakPointer<LIRemoteDataReadListener> mwpReadListener;

public:

  virtual ~LCQLocalDataReader();
  virtual void setDataName(const QString& _dataName) override;
  virtual void setDataReadListener(
      QWeakPointer<LIRemoteDataReadListener> _listener) override;
  virtual void readRequest() override;
  virtual void connectToSource() override;
  virtual void disconnectFromSource() override;

private:
  explicit LCQLocalDataReader(QWeakPointer<LCQLocalSourceHiden> _dataSource);

  virtual void customEvent(QEvent* _event) override;

};

#endif // LCQLOCALDATAREADER_H_
