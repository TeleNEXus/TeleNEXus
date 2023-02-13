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
#include <QDebug>


static void doInParts(
    quint16 _startAddr, 
    quint16 _size, 
    quint16 _partSize, 
    std::function<bool(quint16 _currAddr, quint16 _count, quint16 _shiftData)> _doer);
//==============================================================================CControllerRegistersBase
LCModbusDataMap::CControllerRegistersBase::CControllerRegistersBase(
    const quint8& _devId, 
    QWeakPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  mDevId(_devId),
  mwpMaster(_master),
  mMemoryReadSet(
      [this](int _addr, int _size, QByteArray& _data)
      {
        const int bytes = _size * 2;
        char data_buff[bytes];
        _data.resize(bytes);

        auto sp = mwpMaster.lock();

        if(sp.isNull()) 
        {
          return static_cast<int>(EReadStatus::Wrong);
        }

        if(readRegs(sp.data(), _addr, _size, (quint16*)data_buff).status !=
            LQModbusMasterBase::SReply::EStatus::OK)
        {
          return static_cast<int>(EReadStatus::Wrong);
        }

        for(int i = 0; i < bytes; i++)
        {
          _data[i] = data_buff[i];
        }

        return static_cast<int>(EReadStatus::Valid);
      }, 2)
{
  if(_maxBytesPerReq % 2 == 0) 
  {
    mMaxRegsPerReq = _maxBytesPerReq / 2;
  }
  else
  {
    mMaxRegsPerReq = (_maxBytesPerReq + 1) / 2;
  }
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerRegistersBase::
~CControllerRegistersBase(){}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::inssertReadDataItem(
    QSharedPointer<LCMemoryReadSet::CIData> _dataItem)
{
  mMemoryReadSet.insert(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::removeReadDataItem(
    QSharedPointer<LCMemoryReadSet::CIData> _dataItem)
{
  mMemoryReadSet.remove(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  EReadStatus status = EReadStatus::Wrong;
  auto sp = mwpMaster.lock();

  quint16 buff[_size];

  if(!sp.isNull())
  {
    if(readRegs(sp.data(), _addr, _size, buff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = EReadStatus::Valid;
    }
  }

  _reader->notifyListener(QByteArray( (char*)buff, _size * 2), status);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  auto ret_wrong =
    [&_writer]()
    {
      _writer->notifyListener(EWriteStatus::Failure);
    };

  auto sp = mwpMaster.lock();

  if(sp.isNull()) return ret_wrong();

  if(_data.size() != _size * 2) return ret_wrong();

  if(writeRegs(
        sp.data(),
        _addr,
        _size,
        ((quint16*)_data.constData())).status !=
      LQModbusMasterBase::SReply::EStatus::OK) return ret_wrong();

  _writer->notifyListener(EWriteStatus::Success);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerRegistersBase::update()
{
  mMemoryReadSet.update();
}

//==============================================================================CControllerHoldingRegisters
LCModbusDataMap::CControllerHoldingRegisters::CControllerHoldingRegisters(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  CControllerRegistersBase(_devId, _master, _maxBytesPerReq)
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
  LQModbusMasterBase::SReply reply;

  auto reader = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->readHoldingRegisters(mDevId, _currAddr, _count, &_regs[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxRegsPerReq, reader);

  return reply;
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

  LQModbusMasterBase::SReply reply;
  auto writer = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->writeMultipleRegisters(mDevId, _currAddr, _count, &_regs[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxRegsPerReq, writer);
  return reply;
}

//==============================================================================CControllerInputRegisters
LCModbusDataMap::CControllerInputRegisters::CControllerInputRegisters(
    const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  CControllerRegistersBase(_devId, _master, _maxBytesPerReq)
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
  LQModbusMasterBase::SReply reply;

  auto reader = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->readInputRegisters(mDevId, _currAddr, _count, &_regs[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxRegsPerReq, reader);

  return reply;
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LCModbusDataMap::CControllerInputRegisters::
writeRegs(
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
    QWeakPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  mDevId(_devId),
  mwpMaster(_master),
  mMemoryReadSet(
      [this](int _addr, int _size, QByteArray& _data)
      {
        quint8 data_buff[_size];
        auto sp = mwpMaster.lock();

        if(sp.isNull()) 
        {
          return static_cast<int>(EReadStatus::Wrong);
        }

        if(readBits(sp.data(), _addr, _size, data_buff).status !=
            LQModbusMasterBase::SReply::EStatus::OK)
        {
          return static_cast<int>(EReadStatus::Wrong);
        }

        for(int i = 0; i < _size; i++)
        {
          _data[i] = data_buff[i];
        }

        return static_cast<int>(EReadStatus::Valid);
      })
{
  mMaxBitsPerReq = _maxBytesPerReq * 8;
}

//------------------------------------------------------------------------------
LCModbusDataMap::CControllerBitsBase::
~CControllerBitsBase()
{
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::inssertReadDataItem(
    QSharedPointer<LCMemoryReadSet::CIData> _dataItem)
{
  mMemoryReadSet.insert(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::removeReadDataItem(
    QSharedPointer<LCMemoryReadSet::CIData> _dataItem)
{
  mMemoryReadSet.remove(_dataItem);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  auto sp = mwpMaster.lock();

  if(!sp.isNull())
  {
    quint8 buff[_size];
    EReadStatus status = EReadStatus::Wrong;
    if(readBits(sp.data(), _addr, _size, buff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = EReadStatus::Valid;
    }
    _reader->notifyListener(QByteArray((char*)buff, _size), status);
  }
  _reader->notifyListener(EReadStatus::Wrong);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{

  auto ret_wrong =
    [&_writer]()
    {
      _writer->notifyListener(EWriteStatus::Failure);
    };

  if(_data.size() != _size) return ret_wrong();

  auto sp = mwpMaster.lock();

  if(sp.isNull()) return ret_wrong();

  if(writeBits(
        sp.data(),
        _addr,
        _data.size(),
        ((quint8*)_data.constData())).status !=
      LQModbusMasterBase::SReply::EStatus::OK) return ret_wrong();

  _writer->notifyListener(EWriteStatus::Success);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CControllerBitsBase::update()
{
  mMemoryReadSet.update();
}

//==============================================================================CControllerDiscreteInputs
LCModbusDataMap::CControllerCoils::
CControllerCoils(
    const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  CControllerBitsBase(_devId, _master, _maxBytesPerReq)
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

  LQModbusMasterBase::SReply reply;

  auto reader = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->readCoils(mDevId, _currAddr, _count, &_bits[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxBitsPerReq, reader);

  return reply;
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
  LQModbusMasterBase::SReply reply;
  auto writer = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->writeMultipleCoils(mDevId, _currAddr, _count, &_bits[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxBitsPerReq, writer);
  return reply;
}

//==============================================================================CControllerDiscreteInputs
LCModbusDataMap::CControllerDiscreteInputs::
CControllerDiscreteInputs(const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  CControllerBitsBase(_devId, _master, _maxBytesPerReq)
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
  LQModbusMasterBase::SReply reply;

  auto reader = 
    [&](quint16 _currAddr, quint16 _count, quint16 _shiftData)
    {
      reply = master->readDiscreteInputs(mDevId, _currAddr, _count, &_bits[_shiftData]);
      if(reply.status != LQModbusMasterBase::SReply::EStatus::OK) return false;
      return true;
    };

  doInParts(_addr, _size, mMaxBitsPerReq, reader);

  return reply;
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

//==============================================================================CReadSetData
void LCModbusDataMap::CAddressedDataMapItem::CReadSetData::setData(
    const QByteArray& _data, 
    int status)
{
  mNotifier(static_cast<EReadStatus>(status), _data);
}

//==============================================================================CAddressedDataMapItem
LCModbusDataMap::
CAddressedDataMapItem::
CAddressedDataMapItem(
    quint16 _addr, 
    quint16 _size, 
    CIAddressedDataController& _controller) : 
  mController(_controller)
{

  auto notifier = 
    [this](EReadStatus _status, const QByteArray& _data)
    {
      notifyReaders(_status, _data);
    };

  mspReadSetData = 
    QSharedPointer<LCMemoryReadSet::CIData>(
        new CReadSetData(_addr, _size, notifier));
}

void LCModbusDataMap::CAddressedDataMapItem::notifyReaders(
    EReadStatus _status, 
    const QByteArray& _data)
{
  for(auto it = mReadersList.begin(); it != mReadersList.end(); it++)
  {
    auto sp = it->lock();
    if(sp.isNull())
    {
      it = mReadersList.erase(it);
      it--;
      continue;
    }
    sp->notifyListener(_data, _status);
  }
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CAddressedDataMapItem::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  if(mReadersList.empty()) mController.inssertReadDataItem(mspReadSetData);
  mReadersList.push_back(_reader);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CAddressedDataMapItem::disconnectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  mReadersList.remove(_reader);
  if(mReadersList.empty())
  {
    mController.removeReadDataItem(mspReadSetData);
  }
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CAddressedDataMapItem::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  mController.write(
      mspReadSetData->getAddress(), 
      mspReadSetData->getSize(), 
      _data, _writer);
}

//------------------------------------------------------------------------------
void LCModbusDataMap::CAddressedDataMapItem::read(
    QSharedPointer<LQModbusDataReader> _reader)
{
  mController.read(
      mspReadSetData->getAddress(), mspReadSetData->getSize(), _reader);
}




//==============================================================================CModbusDataMap
LCModbusDataMap::LCModbusDataMap(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master,
    quint16 _maxBytesPerReq) :
  mControllerHoldingRegs(_devId, _master, _maxBytesPerReq),
  mControllerInputRegs(_devId, _master, _maxBytesPerReq),
  mControllerCoils(_devId, _master, _maxBytesPerReq),
  mControllerDiscreteInputs(_devId, _master, _maxBytesPerReq)
{
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
  mMapItems[convertDataName(_name)] = 
    QSharedPointer<CIDataMapItem>( 
      new CAddressedDataMapItem(_addr, _size, mControllerHoldingRegs));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemInputRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mMapItems[convertDataName(_name)] = 
    QSharedPointer<CIDataMapItem>(
      new CAddressedDataMapItem(_addr, _size, mControllerInputRegs));
}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemDiscreteInputs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mMapItems[convertDataName(_name)] = 
    QSharedPointer<CIDataMapItem>(
      new CAddressedDataMapItem(_addr, _size, mControllerDiscreteInputs));

}

//------------------------------------------------------------------------------
void LCModbusDataMap::addItemCoils(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mMapItems[convertDataName(_name)] = 
    QSharedPointer<CIDataMapItem>(
      new CAddressedDataMapItem(_addr, _size, mControllerCoils));
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

//------------------------------------------------------------------------------
QString LCModbusDataMap::convertDataName(const QString& _dataName)
{
  return QString(_dataName).remove(QChar(' '));
}

//==============================================================================doInParts
static void doInParts(
    quint16 _startAddr, 
    quint16 _size, 
    quint16 _partSize, 
    std::function<bool(quint16 _currAddr, quint16 _count, quint16 _shiftData)> _doer)
{
  if(_partSize == 0) 
  {
    _doer(_startAddr, _size, 0);
    return;
  }

  quint16 curr_addr = _startAddr;
  quint16 count = _partSize;

  while(true)
  {
    bool break_flag = false;
    if((curr_addr + count) >= (_startAddr + _size))
    {
      break_flag = true;
      count = (_startAddr + _size) - curr_addr;
    } 

    break_flag |= !_doer(curr_addr, count, (curr_addr - _startAddr));
    curr_addr += _partSize;
    if(break_flag) break;
  }
}

