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

#include <QString>
#include <QLinkedList>
#include <QMap>



class LCModbusDataMap
{
private:

  using EReadStatus = LIRemoteDataReader::EReadStatus;
  using EWriteStatus= LIRemoteDataWriter::EWriteStatus;

  //-----------------------------------------------------------------CBitsDataItem
  class CBitsDataItem
  {
  public:
    quint16 mAddr;
    quint16 mSize;
    EReadStatus mStatus;
    quint8 *mpData;
    QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
  public:
    CBitsDataItem() = delete;

    explicit CBitsDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
    mSize(_size),
    mStatus(EReadStatus::Undef)
    {
      mpData = new quint8[_size];
    }
    ~CBitsDataItem(){delete [] mpData;}
    void notifyReaders();
  };

  //-----------------------------------------------------------------CRegistersDataItem
  class CRegistersDataItem 
  {
  public:
    quint16 mAddr;
    quint16 mSize;
    EReadStatus mStatus;
    quint16 *mpData;
    QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
  public:
    CRegistersDataItem() = delete;

    explicit CRegistersDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
    mSize(_size),
    mStatus(EReadStatus::Undef)
    {
      mpData = new quint16[_size];
    }
    ~CRegistersDataItem(){delete [] mpData;}
    void notifyReaders();
  };

  //----------------------------------------------------------------------------CControllerRegistersBase
  class CControllerRegistersBase
  {
  protected:
    const quint8& mDevId;
  private:
    quint16* mRegBuff;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    QLinkedList<CRegistersDataItem*> mReadDataList;

  public:
    CControllerRegistersBase(
        const quint8& _devId,
        QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerRegistersBase();
    void addReadDataItem(CRegistersDataItem* _dataItem);
    void deleteReadDataItem(CRegistersDataItem* _dataItem);

    void read(
        quint16 _addr, 
        quint16 _size, 
        QSharedPointer<LQModbusDataReader> _reader);

    void write(
        quint16 _addr, 
        quint16 _size, 
        const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer);

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
  class CControllerBitsBase
  {
  protected:
    const quint8& mDevId;
  private:
    quint8* mBitsBuff;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    QLinkedList<CBitsDataItem*> mReadDataList;

  public:
    CControllerBitsBase(
        const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerBitsBase();
    void addReadDataItem(CBitsDataItem* _dataItem);
    void deleteReadDataItem(CBitsDataItem* _dataItem);
    void read(
        quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader);
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

  //----------------------------------------------------------------------------CDataMapItemBase
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

  //----------------------------------------------------------------------------CDataMapItemRegsBase
  class CDataMapItemRegsBase : public CDataMapItemBase
  {
  protected:
    CRegistersDataItem mDataItem;
    CControllerRegistersBase& mController;
  public:
    explicit CDataMapItemRegsBase(
        quint16 _addr, quint16 _size, CControllerRegistersBase& _controller);
    virtual ~CDataMapItemRegsBase();

    virtual void connectReader(
        QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(
        QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) override;
  };

  //----------------------------------------------------------------------------CDataMapItemHoldingRegs
  class CDataMapItemHoldingRegs : public CDataMapItemRegsBase
  {
  public:
    explicit CDataMapItemHoldingRegs(
        quint16 _addr, quint16 _size, CControllerHoldingRegisters& _controller);
    virtual ~CDataMapItemHoldingRegs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemInputRegs
  class CDataMapItemInputRegs : public CDataMapItemRegsBase
  {
  public:
    explicit CDataMapItemInputRegs(
        quint16 _addr, quint16 _size, CControllerInputRegisters& _controller);
    virtual ~CDataMapItemInputRegs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemBitsBase
  class CDataMapItemBitsBase : public CDataMapItemBase
  {
  protected:
    CBitsDataItem mDataItem;
    CControllerBitsBase& mController;
  public:
    explicit CDataMapItemBitsBase(
        quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemBitsBase();

    virtual void connectReader(
        QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void disconnectReader(
        QWeakPointer<LQModbusDataReader> _reader) override;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) override;
  };

  //----------------------------------------------------------------------------CDataMapItemCoils
  class CDataMapItemCoils : public CDataMapItemBitsBase
  {
  public:
    explicit CDataMapItemCoils(
        quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemCoils();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemDiscreteInputs
  class CDataMapItemDiscreteInputs : public CDataMapItemBitsBase
  {
  public:
    explicit CDataMapItemDiscreteInputs(
        quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
    virtual ~CDataMapItemDiscreteInputs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };


private:
  CControllerHoldingRegisters mControllerHoldingRegs;
  CControllerInputRegisters   mControllerInputRegs;

  CControllerCoils            mControllerCoils;
  CControllerDiscreteInputs   mControllerDiscreteInputs;

  QMap<QString, CDataMapItemBase*> mMapItems;

public:
  LCModbusDataMap(const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
  ~LCModbusDataMap();

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
