#ifndef LCMODBUSDATAMAP_H_
#define LCMODBUSDATAMAP_H_

#include <QMap>
#include <QSharedPointer>
#include <QLinkedList>

#include "LIRemoteDataSource.h"
#include "lqmodbusmasterbase.h"

class LQModbusDataWriter;
class LQModbusDataReader;

class CDataMap
{
private:

  //----------------------------------------------------------------------------CDataMapItemBase
  class CDataMapItemBase
  {
  protected:
    CDataMap& mDataMap;
    quint16 mAddr;
    quint16 mSize;
  public:
    CDataMapItemBase() = delete;
    CDataMapItemBase(CDataMap& _dataMap): mDataMap(_dataMap){}
    virtual ~CDataMapItemBase(){}
    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void read(LQModbusDataWriter* _reader) = 0;
    virtual void write(const QByteArray& _data, 
        LQModbusDataWriter* _writer) = 0;
    virtual void update() = 0;
  };

  //----------------------------------------------------------------------------CDataMapItemRegsBase
  class CDataMapItemRegsBase : public CDataMapItemBase
  {
  private:
    quint16 mAddr;
    quint16 mSize;
  public:
    CDataMapItemRegsBase() = delete;
    explicit CDataMapItemRegsBase(
        quint16 _addr, 
        quint16 _size, 
        CDataMap& _dataMap);
    virtual ~CDataMapItemRegsBase();

    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(LQModbusDataWriter* _reader) override;
  };

  //----------------------------------------------------------------------------CDataMapItemHoldingRegs
  class CDataMapItemHoldingRegs : public CDataMapItemRegsBase
  {
  public:
    CDataMapItemHoldingRegs() = delete;
    explicit CDataMapItemHoldingRegs(
        quint16 _addr, quint16 _size, CDataMap& _dataMap);
    virtual ~CDataMapItemHoldingRegs();
    virtual void write(const QByteArray& _data, LQModbusDataWriter* _writer) override;
    virtual void update() override;
  };

  //----------------------------------------------------------------------------CDataMapItemInputRegs
  class CDataMapItemInputRegs : public CDataMapItemRegsBase
  {
  public:
    CDataMapItemInputRegs() = delete;
    explicit CDataMapItemInputRegs(
        quint16 _addr, quint16 _size, CDataMap& _dataMap);
    virtual ~CDataMapItemInputRegs();
    virtual void write(const QByteArray& _data, 
        LQModbusDataWriter* _writer) override;
    virtual void update() override;
  };

  //----------------------------------------------------------------------------CDataMapItemBitsBase
  class CDataMapItemBitsBase : public CDataMapItemBase
  {
  public:
    CDataMapItemBitsBase() = delete;
    explicit CDataMapItemBitsBase(
        quint16 _addr, quint16 _size, CDataMap& _dataMap);
    virtual ~CDataMapItemBitsBase();

    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(LQModbusDataWriter* _reader) override;
  };

  //----------------------------------------------------------------------------CDataMapItemCoils
  class CDataMapItemCoils : public CDataMapItemBitsBase
  {
  public:
    CDataMapItemCoils() = delete;
    explicit CDataMapItemCoils(
        quint16 _addr, quint16 _size, CDataMap& _dataMap);
    virtual ~CDataMapItemCoils();
    virtual void write(const QByteArray& _data, 
        LQModbusDataWriter* _writer) override;
    virtual void update() override;
  };

  //----------------------------------------------------------------------------CDataMapItemDiscreteInputs
  class CDataMapItemDiscreteInputs : public CDataMapItemBitsBase
  {
  public:
    CDataMapItemDiscreteInputs() = delete;
    explicit CDataMapItemDiscreteInputs(
        quint16 _addr, quint16 _size, CDataMap& _dataMap);
    virtual ~CDataMapItemDiscreteInputs();
    virtual void write(const QByteArray& _data, 
        LQModbusDataWriter* _writer) override;
    virtual void update() override;
  };

  QMap<QString, CDataMapItemBase*> mMapItems;
  QSharedPointer<modbus::LQModbusMasterBase> mspMaster;

public:

  CDataMap(quint8 _devId, QSharedPointer<modbus::LQModbusMasterBase> _master);
  ~CDataMap();

  void update();

  void addItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size);
  void addItemInputRegs(const QString& _name, quint16 _addr, quint16 _size);
  void addItemDiscreteInputs(const QString& _name, quint16 _addr, 
      quint16 _size);
  void addItemCoils(const QString& _name, quint16 _addr, quint16 _size);

  bool write(LQModbusDataWriter* _writer, const QByteArray& _data);
  bool read(const QString& _name, QObject* _reader);
  bool connectReader(const QString& _name, QObject* reader);
  bool disconnectReader(QObject* reader);
};

/* }; */
#endif //LCMODBUSDATAMAP_H_
