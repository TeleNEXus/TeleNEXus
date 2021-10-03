/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LCMODBUSDATAMAP_H_
#define LCMODBUSDATAMAP_H_

#include "lqmodbusmasterbase.h"
#include "lqmodbusdatawriter.h"
#include "lqmodbusdatareader.h"
#include "lcmemoryreadset.h"

#include <QString>
#include <QLinkedList>
#include <QMap>



class LCModbusDataMap
{
private:

  using EReadStatus = LIRemoteDataReader::EReadStatus;
  using EWriteStatus= LIRemoteDataWriter::EWriteStatus;

  class CDataMapItemRegsBase;
  class CDataMapItemBitsBase;

  class CAddressedDataMapItem;


  class CIAddressedDataController
  {
  public:
    CIAddressedDataController(){}

    virtual ~CIAddressedDataController(){}

    virtual void addReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData> _dataItem) = 0;
    virtual void deleteReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData> _dataItem) = 0;

    virtual void read(
        quint16 _addr, 
        quint16 _size, 
        QSharedPointer<LQModbusDataReader> _reader) = 0;

    virtual void write(
        quint16 _addr, 
        quint16 _size,
        const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) = 0;
  };

  //----------------------------------------------------------------------------CControllerRegistersBase
  class CControllerRegistersBase : public CIAddressedDataController
  {

  protected:
    const quint8& mDevId;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    LCMemoryReadSet mMemoryReadSet;

  public:
    CControllerRegistersBase(
        const quint8& _devId,
        QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerRegistersBase();

    virtual void addReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData>  _dataItem) override;

    virtual void deleteReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData> _dataItem) override;

    virtual void read(
        quint16 _addr, 
        quint16 _size, 
        QSharedPointer<LQModbusDataReader> _reader) override;

    virtual void write(
        quint16 _addr, 
        quint16 _size,
        const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;

    void update();

  protected:
    virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
    virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
  };

  //----------------------------------------------------------------------------CControllerHoldingRegisters
  class CControllerHoldingRegisters : public CControllerRegistersBase
  {
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

  //----------------------------------------------------------------------------CControllerInputRegisters
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

  //----------------------------------------------------------------------------CControllerBitsBase
  class CControllerBitsBase : public CIAddressedDataController

  {
  protected:
    const quint8& mDevId;
  private:
    QSharedPointer<LQModbusMasterBase> mspMaster;
  public:
    CControllerBitsBase(
        const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerBitsBase();

    virtual void addReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData> _dataItem) override;
    virtual void deleteReadDataItem(
        QSharedPointer<LCMemoryReadSet::CIData> _dataItem) override;

    virtual void read(
        quint16 _addr, 
        quint16 _size, 
        QSharedPointer<LQModbusDataReader> _reader) override;

    virtual void write(
        quint16 _addr,
        quint16 _size,
        const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;

    void update();

  protected:
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;

    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
  };

  //----------------------------------------------------------------------------CControllerDiscreteInputs
  class CControllerCoils : public CControllerBitsBase
  {
  public:
    CControllerCoils(
        const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerCoils();
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
  };

  //----------------------------------------------------------------------------CControllerDiscreteInputs
  class CControllerDiscreteInputs : public CControllerBitsBase
  {
  public:

  public:
    CControllerDiscreteInputs(
        const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
    ~CControllerDiscreteInputs();
    virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
        quint16 _addr, quint16 _size, quint8 _bits[]) override;
  };




  //----------------------------------------------------------------------------CIDataMapItem
  class CIDataMapItem
  {
  public:
    CIDataMapItem(){}
    virtual ~CIDataMapItem(){}
  public:
    virtual bool hasNoReaders(void) const = 0;
    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) = 0;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) = 0;
  };




  //----------------------------------------------------------------------------CAddressedDataMapItem
  class CAddressedDataMapItem : CIDataMapItem
  {
  protected:
    class CReadSetData : public LCMemoryReadSet::CIData
    {
    protected:
      qint16 mAddress;
      qint16 mSize;
      std::function<void(EReadStatus, const QByteArray)> mNotifier;
    public:
      CReadSetData() = delete;
      CReadSetData(
          quint16 _addr, 
          quint32 _size, 
          std::function<void(EReadStatus, const QByteArray&)> _notifier):
        mAddress(_addr),
        mSize(_size),
        mNotifier(_notifier){}

      virtual int getAddress() const override { return mAddress; }
      virtual int getSize() const override { return mSize; }
      virtual void setData(const QByteArray& _data, int status) override;
    };

    QSharedPointer<LCMemoryReadSet::CIData> mspReadSetData;
    QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
    CIAddressedDataController& mController;

  public:
    CAddressedDataMapItem() = delete;
    CAddressedDataMapItem(
        quint16 _addr, 
        quint16 _size, 
        CIAddressedDataController& _controller);
    virtual ~CAddressedDataMapItem(){}
  private:

    void notifyReaders(
        EReadStatus _status, 
        const QByteArray& _data = QByteArray());

  public:

    virtual bool hasNoReaders(void) const override 
    {
      return mReadersList.isEmpty();
    }

    virtual void connectReader(
        QWeakPointer<LQModbusDataReader> _reader) override;

    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) 
      override; 

    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;

    virtual void read(QSharedPointer<LQModbusDataReader> _reader) override;
  };

private:
  CControllerHoldingRegisters mControllerHoldingRegs;
  CControllerInputRegisters   mControllerInputRegs;

  CControllerCoils            mControllerCoils;
  CControllerDiscreteInputs   mControllerDiscreteInputs;

  QMap<QString, QSharedPointer<CIDataMapItem>> mMapItems;

public:
  LCModbusDataMap() = delete;
  LCModbusDataMap(const quint8& _devId, 
      QSharedPointer<LQModbusMasterBase> _master);
  virtual ~LCModbusDataMap(){}

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

#endif /* LCMODBUSDATAMAP_H_ */
