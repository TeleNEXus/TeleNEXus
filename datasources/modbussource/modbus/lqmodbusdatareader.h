#ifndef LQMODBUSDATAREADER_H
#define LQMODBUSDATAREADER_H

#include "lqextendevent.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"

#include <functional>
class LQModbusDataSource;

class LQModbusDataReader : public QObject, public LIRemoteDataReader
{
  Q_OBJECT;
private:

  //----------------------------------------------------------------------------CQEventDataRead
  class CQEventDataIsRead : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      QSharedPointer<QByteArray> mData;
      LERemoteDataStatus mStatus;
      explicit CQEventDataIsRead(const QByteArray& _data, 
          LERemoteDataStatus _status);
      explicit CQEventDataIsRead(LERemoteDataStatus _status);
  };

  QString mDataName;
  QWeakPointer<LIRemoteDataReadListener> mwpReadListener;
  QWeakPointer<LQModbusDataSource> mwpDataSource;
  QWeakPointer<LQModbusDataReader> mwpThis;
  /* std::function<void(QSharedPointer<QByteArray>, LERemoteDataStatus)> mListener; */

private:
  explicit LQModbusDataReader() = delete;

  explicit LQModbusDataReader(  
      const QString& _dataName,
      QWeakPointer<LIRemoteDataReadListener> _readListener,
      QWeakPointer<LQModbusDataSource> _dataSource);

public:
  virtual ~LQModbusDataReader();

  static QSharedPointer<LQModbusDataReader> 
    create(
        const QString& _dataName, 
        QWeakPointer<LIRemoteDataReadListener> _readListener,
        QWeakPointer<LQModbusDataSource> _dataSource);

  virtual void readRequest() override;
  virtual void connectToSource() override;
  virtual void disconnectFromSource() override;

  void notifyListener(const QByteArray& _data, LERemoteDataStatus _status);
  void notifyListener(LERemoteDataStatus _status);
  QString getDataName(){ return mDataName; }

private:
  virtual void customEvent(QEvent* _event) override;
};

#endif // LQMODBUSDATAREADER_H
