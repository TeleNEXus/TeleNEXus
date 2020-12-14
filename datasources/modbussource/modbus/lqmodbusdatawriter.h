#ifndef LQMODBUSDATAWRITER_H
#define LQMODBUSDATAWRITER_H

#include "lqextendevent.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataSource.h"

class LQModbusDataSource;
class LQModbusDataWriter : public QObject, public LIRemoteDataWriter
{

  Q_OBJECT;

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

  LTWriteListener mListener;
  /* QWeakPointer<LIRemoteDataWriteListener> mwpWriteListener; */
  QWeakPointer<LQModbusDataSource> mwpDataSource;
  QWeakPointer<LQModbusDataWriter> mwpThis;

private:
  explicit LQModbusDataWriter() = delete;

  /* explicit LQModbusDataWriter( */
  /*     const QString& _dataName, */
  /*     QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
  /*     QSharedPointer<LQModbusDataSource> _dataSource); */

  explicit LQModbusDataWriter(
      const QString& _dataName,
      LTWriteListener _listener,
      QSharedPointer<LQModbusDataSource> _dataSource);
public:

  virtual ~LQModbusDataWriter();

  static QSharedPointer<LQModbusDataWriter> create(
      const QString& _dataName,
      LTWriteListener _listener,
      QSharedPointer<LQModbusDataSource> _dataSource);

  /* static QSharedPointer<LQModbusDataWriter> create( */
  /*     const QString& _dataName, */
  /*     QSharedPointer<LIRemoteDataWriteListener> _writeListener, */
  /*     QSharedPointer<LQModbusDataSource> _dataSource); */


  virtual void writeRequest(const QByteArray& _data) override;
  QString getDataName(){ return mDataName; }
  void notifyListener(LERemoteDataStatus _status);

private:
  virtual void customEvent(QEvent *_event) override;
};

#endif // LQMODBUSDATAWRITER_H
