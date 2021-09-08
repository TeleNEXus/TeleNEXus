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

  class CDataMapItemRegsBase;
  class CDataMapItemBitsBase;

  //----------------------------------------------------------------------------CControllerRegistersBase
  class CControllerRegistersBase
  {
  protected:
    const quint8& mDevId;
  private:
    quint16* mRegBuff;
    QSharedPointer<LQModbusMasterBase> mspMaster;
    QLinkedList<CDataMapItemRegsBase*> mReadDataList;

  public:
    CControllerRegistersBase(
        const quint8& _devId,
        QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerRegistersBase();
    void addReadDataItem(CDataMapItemRegsBase* _dataItem);
    void deleteReadDataItem(CDataMapItemRegsBase* _dataItem);

    void read(
        quint16 _addr, 
        quint16 _size, 
        QSharedPointer<LQModbusDataReader> _reader);

    void write(
        quint16 _addr, 
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
    QLinkedList<CDataMapItemBitsBase*> mReadDataList;

  public:
    CControllerBitsBase(
        const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
    ~CControllerBitsBase();
    void addReadDataItem(CDataMapItemBitsBase* _dataItem);
    void deleteReadDataItem(CDataMapItemBitsBase* _dataItem);
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
  protected:
    quint16 mAddr;
    quint16 mSize;
    QLinkedList<QWeakPointer<LQModbusDataReader>> mReadersList;
  public:
    CDataMapItemBase() = delete;
    CDataMapItemBase(quint16 _addr, quint16 _size): 
      mAddr(_addr),
      mSize(_size)
    {}
    virtual ~CDataMapItemBase(){}

    virtual void connectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void disconnectReader(QWeakPointer<LQModbusDataReader> _reader) = 0;
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) = 0;
    virtual void read(QSharedPointer<LQModbusDataReader> _reader) = 0;

    void    notifyReaders(EReadStatus _status, 
        const QByteArray& _data = QByteArray());
    quint16 getAddress(void){return mAddr;}
    quint16 getSize(void){return mSize;}
    bool hasNoReaders(void){return mReadersList.isEmpty();}
  };

  //----------------------------------------------------------------------------CDataMapItemRegsBase
  class CDataMapItemRegsBase : public CDataMapItemBase
  {
  protected:
    CControllerRegistersBase& mController;
  public:
    CDataMapItemRegsBase() = delete;
    CDataMapItemRegsBase(
        quint16 _addr, 
        quint16 _size, 
        CControllerRegistersBase& _controller) :
      CDataMapItemBase(_addr, _size * 2),
      mController(_controller){}

    virtual ~CDataMapItemRegsBase(){}
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
    CDataMapItemHoldingRegs() = delete;
    CDataMapItemHoldingRegs(
        quint16 _addr, quint16 _size, CControllerHoldingRegisters& _controller);
    virtual ~CDataMapItemHoldingRegs();
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemInputRegs
  class CDataMapItemInputRegs : public CDataMapItemRegsBase
  {
  public:
    CDataMapItemInputRegs() = delete;
    CDataMapItemInputRegs(
        quint16 _addr, 
        quint16 _size, 
        CControllerInputRegisters& _controller):
      CDataMapItemRegsBase(_addr, _size, _controller)
    {
    } virtual ~CDataMapItemInputRegs(){}
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemBitsBase
  class CDataMapItemBitsBase : public CDataMapItemBase
  {
  protected:
    CControllerBitsBase& mController;
  public:
    CDataMapItemBitsBase() = delete;
    CDataMapItemBitsBase(
        quint16 _addr, 
        quint16 _size, 
        CControllerBitsBase& _controller): 
      CDataMapItemBase(_addr, _size),
      mController(_controller)
    {
    }

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
    CDataMapItemCoils() = delete;
    CDataMapItemCoils(
        quint16 _addr, 
        quint16 _size, 
        CControllerBitsBase& _controller):
      CDataMapItemBitsBase(_addr, _size, _controller){}

    virtual ~CDataMapItemCoils(){};
    virtual void write(const QByteArray& _data,
        QSharedPointer<LQModbusDataWriter> _writer) override;
  };

  //----------------------------------------------------------------------------CDataMapItemDiscreteInputs
  class CDataMapItemDiscreteInputs : public CDataMapItemBitsBase
  {
  public:
    CDataMapItemDiscreteInputs() = delete;
    CDataMapItemDiscreteInputs(
        quint16 _addr, 
        quint16 _size, 
        CControllerBitsBase& _controller):
      CDataMapItemBitsBase(_addr, _size, _controller){}
    virtual ~CDataMapItemDiscreteInputs(){};
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
  LCModbusDataMap() = delete;
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
