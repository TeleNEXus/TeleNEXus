#ifndef LCLOCALSOURCEHIDEN_H_
#define LCLOCALSOURCEHIDEN_H_

#include <QObject>
#include "lcdataitemmap.h"
#include "lqextendevent.h"

class LCQLocalDataReader;
class LCQLocalDataWriter;
class LIRemoteDataReader;
class LIRemoteDataWriter;

class LCQLocalSourceHiden final : public QObject 
{

private:
  //----------------------------------------------------------------------------CEventBase
  class CEventBase : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CEventBase();
      virtual void handle(LCQLocalSourceHiden* _sender) = 0;
  };

  //----------------------------------------------------------------------------CEventConnetcReader
  class CEventConnectReader : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventConnectReader() = delete;
    CEventConnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventDisconnectReader
  class CEventDisconnectReader : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventDisconnectReader() = delete;
    CEventDisconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventRead
  class CEventRead : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataReader> mspDataReader;

  public:
    CEventRead() = delete;
    CEventRead(QSharedPointer<LCQLocalDataReader> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  //----------------------------------------------------------------------------CEventWrite
  class CEventWrite : public CEventBase 
  {
  private:
    QSharedPointer<LCQLocalDataWriter> mspDataWriter;

  public:
    CEventWrite() = delete;
    CEventWrite(QSharedPointer<LCQLocalDataWriter> _sp_reader);
    virtual void handle(LCQLocalSourceHiden* _sender) override;
  };

  LCDataItemMap mDataMap;
public:

  explicit LCQLocalSourceHiden();

  virtual ~LCQLocalSourceHiden();

  void addItem(const QString& _id, const QByteArray& _data); 
  void addItem(const QString& _id, const QBitArray& _data); 

private:

  void connectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void disconnectReader(QSharedPointer<LCQLocalDataReader> _sp_reader);
  void read(QSharedPointer<LCQLocalDataReader> _ps_reader);
  void write(
      const QByteArray& _data, 
      QSharedPointer<LCQLocalDataWriter> _sp_writer);

  virtual void customEvent(QEvent* _event) override;

  friend class LCQLocalDataReader;
  friend class LCQLocalDataWriter;
};

#endif //LCLOCALSOURCEHIDEN_H_
