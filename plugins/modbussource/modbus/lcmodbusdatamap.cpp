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
#include "lcmodbusdatamap.h"
#include "lmodbusdefs.h"

//==============================================================================CRegistersDataItem
void LCModbusDataMap::CRegistersDataItem::notifyReaders()
{
  for(auto it = mReadersList.begin(); it != mReadersList.end(); it++)
  {
    auto sp = it->lock();
    if(sp.isNull())
    {
      mReadersList.erase(it);
      continue;
    }
    sp->notifyListener(QByteArray((char*)(mpData), mSize * 2), mStatus);
  }
}

//==============================================================================CBitsDataItem
void LCModbusDataMap::CBitsDataItem::notifyReaders()
{
  for(auto it = mReadersList.begin(); it != mReadersList.end(); it++)
  {
    auto sp = it->lock();
    if(sp.isNull())
    {
      mReadersList.erase(it);
      continue;
    }
    sp->notifyListener(QByteArray( (char*)(mpData), mSize), mStatus);
  }
}

//==============================================================================CControllerRegistersBase
LCModbusDataMap::CControllerRegistersBase::CControllerRegistersBase(
    const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master) :
  mDevId(_devId),
  mspMaster(_master)
{
  mRegBuff = new quint16[MODBUS_MAX_READ_REGISTER_COUNT + 1];
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerRegistersBase::
~CControllerRegistersBase()
{
  delete [] mRegBuff;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::addReadDataItem(
    CRegistersDataItem* _dataItem)
{
  if(mReadDataList.contains(_dataItem)) return;
  mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::deleteReadDataItem(
    CRegistersDataItem* _dataItem)
{
  mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  EReadStatus status = EReadStatus::Wrong;

  if(!mspMaster.isNull())
  {
    if(readRegs(mspMaster.data(), _addr, _size, mRegBuff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = EReadStatus::Valid;
    }
  }
  _reader->notifyListener(QByteArray( (char*)mRegBuff, _size * 2), status);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  EWriteStatus status = EWriteStatus::Failure;

  if(_data.size() % 2 == 0)
  {
    quint16 length = _data.size() >> 1;
    if(length == _size)
    {
      if(!mspMaster.isNull())
      {
        if(writeRegs(
              mspMaster.data(),
              _addr,
              length,
              ((quint16*)_data.constData())).status ==
            LQModbusMasterBase::SReply::EStatus::OK)
          status = EWriteStatus::Success;
      }
    }
  }
  _writer->notifyListener(status);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::update()
{
  //    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();
  //TODO: Проработать опережающий ответ для всех
  //слушателей при нескольких таймаутах.
  //
  for(auto it = mReadDataList.begin();
      it != mReadDataList.end();
      it++)
  {
    (*it)->mStatus = EReadStatus::Wrong;
    if(readRegs(
          mspMaster.data(),
          (*it)->mAddr,
          (*it)->mSize,
          (*it)->mpData).status == LQModbusMasterBase::SReply::EStatus::OK)
    {
      (*it)->mStatus = EReadStatus::Valid;
    }
    (*it)->notifyReaders();
    if((*it)->mReadersList.isEmpty()) mReadDataList.erase(it);
  }
}

//==============================================================================CControllerHoldingRegisters
LCModbusDataMap::CControllerHoldingRegisters::CControllerHoldingRegisters(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  CControllerRegistersBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerHoldingRegisters::~CControllerHoldingRegisters()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerHoldingRegisters:: readRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  return master->readHoldingRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerHoldingRegisters::writeRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  if(_size == 1)
  {
    return master->writeSingleRegister( mDevId, _addr, _regs[0]);
  }
  return master->writeMultipleRegisters( mDevId, _addr, _size, _regs);
}

//==============================================================================CControllerInputRegisters
LCModbusDataMap::CControllerInputRegisters::CControllerInputRegisters(
    const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerRegistersBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerInputRegisters::
~CControllerInputRegisters()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerInputRegisters::readRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  return master->readInputRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerInputRegisters::writeRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  Q_UNUSED(master);
  Q_UNUSED(_addr);
  Q_UNUSED(_size);
  Q_UNUSED(_regs);
  return LQModbusMasterBase::SReply(
      LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}

//==============================================================================CControllerRegistersBase
LCModbusDataMap::CControllerBitsBase::CControllerBitsBase(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  mDevId(_devId),
  mspMaster(_master)
{
  mBitsBuff = new quint8[MODBUS_MAX_READ_COIL_COUNT + 1];
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerBitsBase::
~CControllerBitsBase()
{
  delete [] mBitsBuff;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::addReadDataItem(
    CBitsDataItem* _dataItem)
{
  if(mReadDataList.contains(_dataItem)) return;
  mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::deleteReadDataItem(
    CBitsDataItem* _dataItem)
{
  mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  EReadStatus status = EReadStatus::Wrong;

  if(!mspMaster.isNull())
  {
    if(readBits(mspMaster.data(), _addr, _size, mBitsBuff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = EReadStatus::Valid;
    }
  }
  _reader->notifyListener(QByteArray((char*)mBitsBuff, _size), status);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  EWriteStatus status = EWriteStatus::Failure;

  if(_data.size() == _size)
  {
    if(!mspMaster.isNull())
    {
      if(writeBits(
            mspMaster.data(),
            _addr,
            _size,
            ((quint8*)_data.constData())).status ==
          LQModbusMasterBase::SReply::EStatus::OK)
        status = EWriteStatus::Success;
    }
  }
  _writer->notifyListener(status);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::update()
{
  if(mspMaster.isNull()) return;
  {
    //TODO: Проработать опережающий ответ
    //для всех слушателей при нескольких таймаутах.
    for(auto it = mReadDataList.begin(); it != mReadDataList.end(); it++)
    {
      (*it)->mStatus = EReadStatus::Wrong;
      if(readBits(
            mspMaster.data(),
            (*it)->mAddr,
            (*it)->mSize,
            (*it)->mpData).status ==
          LQModbusMasterBase::SReply::EStatus::OK)
      {
        (*it)->mStatus = EReadStatus::Valid;
      }
      (*it)->notifyReaders();
      if((*it)->mReadersList.isEmpty()) mReadDataList.erase(it);
    }
  }
}

//==============================================================================CControllerDiscreteInputs
LCModbusDataMap::CControllerCoils::
CControllerCoils(const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerBitsBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerCoils::
~CControllerCoils()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerCoils::readBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

  return master->readCoils(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerCoils::writeBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
  if(_size == 1)
  {
    return master->writeSingleCoils( mDevId, _addr, _bits[0]);
  }
  return master->writeMultipleCoils( mDevId, _addr, _size, _bits);
}

//==============================================================================CControllerDiscreteInputs
LCModbusDataMap::CControllerDiscreteInputs::
CControllerDiscreteInputs(const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerBitsBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerDiscreteInputs::
~CControllerDiscreteInputs()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerDiscreteInputs::readBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
  return master->readDiscreteInputs(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerDiscreteInputs::writeBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
  Q_UNUSED(master);
  Q_UNUSED(_addr);
  Q_UNUSED(_size);
  Q_UNUSED(_bits);
  return LQModbusMasterBase::SReply(
      LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}

//==============================================================================CDataMapItemRegsBase
LCModbusDataMap::CDataMapItemRegsBase::CDataMapItemRegsBase(
    quint16 _addr,
    quint16 _size,
    CControllerRegistersBase& _controller):
  mDataItem(_addr, _size),
  mController(_controller)
{

}

//------------------------------------------------------------------------------
LCModbusDataMap::CDataMapItemRegsBase::~CDataMapItemRegsBase()
{
  mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemRegsBase::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
  mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemRegsBase::disconnectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  mDataItem.mReadersList.removeAll(_reader);
  if(mDataItem.mReadersList.isEmpty())
    mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemRegsBase::read(
    QSharedPointer<LQModbusDataReader> _reader)
{
  mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==============================================================================CDataMapItemHoldingRegs
LCModbusDataMap::CDataMapItemHoldingRegs::CDataMapItemHoldingRegs(
    quint16 _addr,
    quint16 _size,
    CControllerHoldingRegisters& _controller):
  CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CDataMapItemHoldingRegs::~CDataMapItemHoldingRegs()
{
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemHoldingRegs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//==============================================================================CDataMapItemInputRegs
LCModbusDataMap::CDataMapItemInputRegs::CDataMapItemInputRegs(
    quint16 _addr,
    quint16 _size,
    CControllerInputRegisters& _controller):
  CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CDataMapItemInputRegs::~CDataMapItemInputRegs()
{
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemInputRegs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(EWriteStatus::Failure);
}

//==============================================================================CDataMapItemBitsBase
LCModbusDataMap::
CDataMapItemBitsBase::
CDataMapItemBitsBase(quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller): mDataItem(_addr, _size),
  mController(_controller)
{

}

//------------------------------------------------------------------------------
LCModbusDataMap::
CDataMapItemBitsBase::
~CDataMapItemBitsBase()
{
  mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemBitsBase::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
  mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void LCModbusDataMap::
CDataMapItemBitsBase::disconnectReader(QWeakPointer<LQModbusDataReader> _reader)
{
  mDataItem.mReadersList.removeAll(_reader);
  if(mDataItem.mReadersList.isEmpty())
  {
    mController.deleteReadDataItem(&mDataItem);
  }
}

//------------------------------------------------------------------------------
void LCModbusDataMap::
CDataMapItemBitsBase::read(QSharedPointer<LQModbusDataReader> _reader)
{
  mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==============================================================================CDataMapItemCoils
LCModbusDataMap::CDataMapItemCoils::CDataMapItemCoils(
    quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CDataMapItemCoils::~CDataMapItemCoils()
{
}

//------------------------------------------------------------------------------
void LCModbusDataMap::
CDataMapItemCoils::write(
    const QByteArray& _data, QSharedPointer<LQModbusDataWriter> _writer)
{
  mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//==============================================================================CDataMapItemInputRegs
LCModbusDataMap::CDataMapItemDiscreteInputs::CDataMapItemDiscreteInputs(
    quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::CDataMapItemDiscreteInputs::~CDataMapItemDiscreteInputs()
{
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CDataMapItemDiscreteInputs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(EWriteStatus::Failure);
}

//==============================================================================CModbusDataMap
LCModbusDataMap::LCModbusDataMap(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  mControllerHoldingRegs(_devId, _master),
  mControllerInputRegs(_devId, _master),
  mControllerCoils(_devId, _master),
  mControllerDiscreteInputs(_devId, _master)
{
}

//------------------------------------------------------------------------------
LCModbusDataMap::~LCModbusDataMap()
{
  for(auto it = mMapItems.begin(); it != mMapItems.end(); it++)
  {
    delete *it;
  }
  mMapItems.clear();
}

//------------------------------------------------------------------------------
void LCModbusDataMap::update()
{
  mControllerHoldingRegs.update();
  mControllerInputRegs.update();
  mControllerCoils.update();
  mControllerDiscreteInputs.update();
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemHoldingRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  auto it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name,
      new CDataMapItemHoldingRegs(_addr, _size, mControllerHoldingRegs));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemInputRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  auto it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, new CDataMapItemInputRegs(_addr, _size, mControllerInputRegs));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemDiscreteInputs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  auto it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name,
      new CDataMapItemDiscreteInputs(_addr, _size, mControllerDiscreteInputs));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemCoils(
    const QString& _name, quint16 _addr, quint16 _size)
{
  auto it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, new CDataMapItemCoils(_addr, _size, mControllerCoils));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::write(
    QSharedPointer<LQModbusDataWriter> _writer, const QByteArray& _data)
{
  auto it = mMapItems.find(_writer->getDataName());
  if(it == mMapItems.end())
  {
    _writer->notifyListener(EWriteStatus::Failure);
  }
  else
  {
    (*it)->write(_data, _writer);
  }
}

//------------------------------------------------------------------------------
void LCModbusDataMap::read(
    QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end())
  {
    _reader->notifyListener(EReadStatus::Undef);
  }
  else
  {
    (*it)->read(_reader);
  }
}

//------------------------------------------------------------------------------
void LCModbusDataMap::connectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end()) return;
  (*it)->connectReader(_reader);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::disconnectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end()) return;
  (*it)->disconnectReader(_reader);
}