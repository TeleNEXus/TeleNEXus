#ifndef LQMODBUSDATASOURCE_H
#define LQMODBUSDATASOURCE_H

#include <QThread>
#include <QList>
#include <QLinkedList>
#include <QMap>
#include <QTimerEvent>

#include "lqextendevent.h"
#include "lqmodbusmasterbase.h"
#include "LIRemoteDataSource.h"

namespace modbus
{
class LQModbusDataWriter;
class LQModbusDataReader;

//==============================================================================LCQModbusDataController
class LQModbusDataSource : public QObject, public LIRemoteDataSource
{
  Q_OBJECT;
private:
  //----------------------------------------------------------------------------CQEventBase
  class CQEventBase : public QEvent
  {
    __LQ_EXTENDED_QEVENT_DECLARATION
  public:
      CQEventBase();
      virtual void handle(LQModbusDataSource* _sender) = 0;
  };

  //------------------------------------------------------------------------CQEventStart
  class CQEventStart : public CQEventBase
  {
  private:
    int mUpdateInterval;
  public:
    CQEventStart(int _updateInterval);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //------------------------------------------------------------------------CQEventStop
  class CQEventStop : public CQEventBase
  {
  public:
    CQEventStop();
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //------------------------------------------------------------------------CQEventReqRead
  class CQEventReqRead : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqRead(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //--------------------------------------------------------------------------CQEventReqWrite
  class CQEventReqWrite : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataWriter> mspWriter;
    QByteArray  mData;
  public:
    CQEventReqWrite(
        QSharedPointer<LQModbusDataWriter> _sp_writer,
        const QByteArray& _data);

    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //------------------------------------------------------------------------CQEventReqConnectReader
  class CQEventReqConnectReader : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqConnectReader(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //------------------------------------------------------------------------CQEventReqDisconnectReader
  class CQEventReqDisconnectReader : public CQEventBase
  {
  private:
    QSharedPointer<LQModbusDataReader> mspReader;
  public:
    CQEventReqDisconnectReader(QSharedPointer<LQModbusDataReader> _reader);
    virtual void handle(LQModbusDataSource* _sender) override;
  };

  //------------------------------------------------------------------------CControllerRegistersBase
  class CControllerRegistersBase
  {
  public:
    class CDataItem
    {
    public:
      quint16 mAddr;
      quint16 mSize;
      LERemoteDataStatus mStatus;
      quint16 *mpData;
      QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
    public:
      CDataItem() = delete;

      explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
      mSize(_size),
      mStatus(LERemoteDataStatus::DS_UNDEF)
      {
        mpData = new quint16[_size];
      }
      ~CDataItem(){delete [] mpData;}
      void notifyReaders();
    };
  protected:
    const quint8& mDevId;
  private:
    quint16* mRegBuff;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    QLinkedList<CDataItem*> mReadDataList;

  public:
    CControllerRegistersBase(
        const quint8& _devId,
        QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerRegistersBase();
    void addReadDataItem(CDataItem* _dataItem);
    void deleteReadDataItem(CDataItem* _dataItem);
    void read(quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader);
    void write(quint16 _addr, quint16 _size, const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer);
    void update();
  protected:
    virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
    virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
  };

  //------------------------------------------------------------------------CControllerHoldingRegisters
  class CControllerHoldingRegisters : public CControllerRegistersBase
  {
  public:

  public:
    CControllerHoldingRegisters(
        const quint8& _devId,
        QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerHoldingRegisters();
    virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) override;
    virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) override;
  };

  //------------------------------------------------------------------------CControllerInputRegisters
  class CControllerInputRegisters : public CControllerRegistersBase
  {
  public:

  public:
    CControllerInputRegisters(
        const quint8& _devId,
        QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerInputRegisters();
    virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) override;
    virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) override;
  };

  //-----------------------------------------------------------------------------------------------CControllerBitsBase
  class CControllerBitsBase
  {
  public:
    class CDataItem
    {
    public:
      quint16 mAddr;
      quint16 mSize;
      LERemoteDataStatus mStatus;
      quint8 *mpData;
      QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
    public:
      CDataItem() = delete;

      explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
      mSize(_size),
      mStatus(LERemoteDataStatus::DS_UNDEF)
      {
        mpData = new quint8[_size];
      }
      ~CDataItem(){delete [] mpData;}
      void notifyReaders();
    };
  protected:
    const quint8& mDevId;
  private:
    quint8* mBitsBuff;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    QLinkedList<CDataItem*> mReadDataList;

  public:
    CControllerBitsBase(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerBitsBase();
    void addReadDataItem(CDataItem* _dataItem);
    void deleteReadDataItem(CDataItem* _dataItem);
    void read(quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader);
    void write(
        quint16 _addr,
        quint16 _size,
        const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer);
    void update();
  protected:
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
  };


  //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
  class CControllerDiscreteInputs : public CControllerBitsBase
  {
  public:

  public:
    CControllerDiscreteInputs(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerDiscreteInputs();
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
  };

  //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
  class CControllerCoils : public CControllerBitsBase
  {
  public:

  public:
    CControllerCoils(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerCoils();
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
  };


  //--------------------------------------------------------------------------------------------------CDataMapItemBase
  class CDataMapItemBase
  {
  public:
    CDataMapItemBase(){}
    virtual ~CDataMapItemBase(){}
    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) = 0;

    virtual void read(QSharedPointer<LQModbusDataReader> _reader) = 0;
  };

  //----------------------------------------------------------------------------------------------CDataMapItemRegsBase
  class CDataMapItemRegsBase : public CDataMapItemBase
  {
  protected:
    CControllerRegistersBase::CDataItem mDataItem;
    CControllerRegistersBase& mController;
  public:
    explicit CDataMapItemRegsBase(quint16 _addr, quint16 _size, CControllerRegistersBase& _controller);
    virtual ~CDataMapItemRegsBase();

    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) override;
  };

  //-------------------------------------------------------------------------------------------CDataMapItemHoldingRegs
  class CDataMapItemHoldingRegs : public CDataMapItemRegsBase
  {
  public:
    explicit CDataMapItemHoldingRegs(quint16 _addr, quint16 _size, CControllerHoldingRegisters& _controller);
    virtual ~CDataMapItemHoldingRegs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //---------------------------------------------------------------------------------------------CDataMapItemInputRegs
  class CDataMapItemInputRegs : public CDataMapItemRegsBase
  {
  public:
    explicit CDataMapItemInputRegs(quint16 _addr, quint16 _size, CControllerInputRegisters& _controller);
    virtual ~CDataMapItemInputRegs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------------------------CDataMapItemBitsBase
  class CDataMapItemBitsBase : public CDataMapItemBase
  {
  protected:
    CControllerBitsBase::CDataItem mDataItem;
    CControllerBitsBase& mController;
  public:
    explicit CDataMapItemBitsBase(quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemBitsBase();

    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) override;
  };

  //-------------------------------------------------------------------------------------------------CDataMapItemCoils
  class CDataMapItemCoils : public CDataMapItemBitsBase
  {
  public:
    explicit CDataMapItemCoils(
        quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemCoils();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------------------CDataMapItemDiscreteInputs
  class CDataMapItemDiscreteInputs : public CDataMapItemBitsBase
  {
  public:
    explicit CDataMapItemDiscreteInputs(quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemDiscreteInputs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMap
  class CDataMap
  {
  private:

    CControllerHoldingRegisters mControllerHoldingRegs;
    CControllerInputRegisters   mControllerInputRegs;

    CControllerCoils mControllerCoils;
    CControllerDiscreteInputs mControllerDiscreteInputs;

    QMap<QString, CDataMapItemBase*> mMapItems;

  public:
    CDataMap(const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
    ~CDataMap();

    void update();

    void addItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size);
    void addItemInputRegs(const QString& _name, quint16 _addr, quint16 _size);
    void addItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size);
    void addItemCoils(const QString& _name, quint16 _addr, quint16 _size);

    void write(QSharedPointer<LQModbusDataWriter> _writer, const QByteArray& _data);
    void read(QSharedPointer<LQModbusDataReader> _reader);
    void connectReader(QSharedPointer<LQModbusDataReader> _reader);
    void disconnectReader(QSharedPointer<LQModbusDataReader> _reader);
  };

  quint8 mDevId;
  QSharedPointer<LQModbusMasterBase> mModbusMaster;
  int mUpdateInterval;
  QTimer mTimer;

  CDataMap mDataMap;
  QSharedPointer<QThread> mspThread;
  QWeakPointer<LQModbusDataSource> mwpThis;
private:
  explicit LQModbusDataSource(quint8 _devId,
      QSharedPointer<LQModbusMasterBase> _modbusMaster);

  LQModbusDataSource(const LQModbusDataSource&) = delete;
  LQModbusDataSource& operator=(const LQModbusDataSource&) = delete;
public:
  virtual ~LQModbusDataSource();
  static QSharedPointer<LQModbusDataSource> create(
      quint8 _devId,
      QSharedPointer<LQModbusMasterBase> _modbusMaster);
  void addDataItemHoldingRegs(    const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemInputRegs(      const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemDiscreteInputs( const QString& _name, quint16 _addr, quint16 _size);
  void addDataItemCoils(          const QString& _name, quint16 _addr, quint16 _size);
  void start(int _updateIntervalMs = 500);
  void start(QSharedPointer<QThread> _thread, int _updateIntervalMs = 500);
  void stop();


private:
  void connectReader(QSharedPointer<LQModbusDataReader> _reader);
  void disconnectReader(QSharedPointer<LQModbusDataReader> _reader);
  void read(QSharedPointer<LQModbusDataReader> _reader);
  void write(QSharedPointer<LQModbusDataWriter> _writer,
      const QByteArray& _data);
  virtual void customEvent(QEvent* _event) override;

public:

  //----------------------------------------------------------------------------------------------------------override
  virtual QSharedPointer<LIRemoteDataReader> createReader(
      const QString& _dataName,
      QWeakPointer<LIRemoteDataReadListener> _readListener) override;

  virtual QSharedPointer<LIRemoteDataWriter> createWriter(
      const QString& _dataName,
      QWeakPointer<LIRemoteDataWriteListener> _writeListener) override;

  friend class LQModbusDataReader;
  friend class LQModbusDataWriter;
};

}
#endif // LQMODBUSDATASOURCE_H
