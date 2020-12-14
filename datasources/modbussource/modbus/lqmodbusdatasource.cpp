#include <QThread>
#include <typeinfo>
#include <QCoreApplication>
#include <QDebug>

#include "lqmodbusdatasource.h"
#include "lmodbusdefs.h"

#include "lqmodbusdatareader.h"
#include "lqmodbusdatawriter.h"


//==============================================================================CQEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataSource::CQEventBase);

LQModbusDataSource::CQEventBase::CQEventBase() :
  QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventStart::CQEventStart(int _updateInterval) :
  mUpdateInterval(_updateInterval)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventStart::handle(LQModbusDataSource* _sender)
{
  _sender->mUpdateInterval = mUpdateInterval;
  _sender->mTimer.start(mUpdateInterval);
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventStop::CQEventStop()
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventStop::handle(LQModbusDataSource* _sender)
{
  _sender->mTimer.stop();
}

//==============================================================================CQEventReqRead
LQModbusDataSource::CQEventReqRead::CQEventReqRead(
    QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::
CQEventReqRead::handle(LQModbusDataSource* _sender)
{
  _sender->mDataMap.read(mspReader);
}

//==============================================================================CQEventReqWrite
LQModbusDataSource::
CQEventReqWrite::
CQEventReqWrite(
    QSharedPointer<LQModbusDataWriter> _sp_writer,
    const QByteArray& _data) :
  mspWriter(_sp_writer),
  mData(_data)
{

}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventReqWrite::handle(LQModbusDataSource* _sender)
{
  _sender->mDataMap.write(mspWriter, mData);
}
//==============================================================================CQEventReqConnectReader
LQModbusDataSource::CQEventReqConnectReader::CQEventReqConnectReader(
    QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventReqConnectReader::handle(
    LQModbusDataSource* _sender)
{
  qDebug() << "LQModbusDataSource::CQEventReqConnectReader::handle: "<< mspReader->getDataName();
  _sender->mDataMap.connectReader(mspReader);
}
//==============================================================================CQEventReqDisconnectReader
LQModbusDataSource::CQEventReqDisconnectReader::
CQEventReqDisconnectReader(QSharedPointer<LQModbusDataReader> _reader) :
  mspReader(_reader)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventReqDisconnectReader::handle(
    LQModbusDataSource* _sender)
{
  _sender->mDataMap.disconnectReader(mspReader);
}

//==============================================================================CControllerRegistersBase
void LQModbusDataSource::CControllerRegistersBase::CDataItem::notifyReaders()
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

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerRegistersBase::CControllerRegistersBase(
    const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master) :
  mDevId(_devId),
  mspMaster(_master)
{
  mRegBuff = new quint16[MODBUS_MAX_READ_REGISTER_COUNT + 1];
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerRegistersBase::
~CControllerRegistersBase()
{
  delete [] mRegBuff;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::addReadDataItem(
    CDataItem* _dataItem)
{
  if(mReadDataList.contains(_dataItem)) return;
  mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::deleteReadDataItem(
    CDataItem* _dataItem)
{
  mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

  if(!mspMaster.isNull())
  {
    if(readRegs(mspMaster.data(), _addr, _size, mRegBuff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = LERemoteDataStatus::DS_OK;
    }
  }
  _reader->notifyListener(QByteArray( (char*)mRegBuff, _size * 2), status);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

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
          status = LERemoteDataStatus::DS_OK;
      }
    }
  }
  _writer->notifyListener(status);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::update()
{
  //    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();
  //TODO: Проработать опережающий ответ для всех
  //слушателей при нескольких таймаутах.
  //
  for(auto it = mReadDataList.begin();
      it != mReadDataList.end();
      it++)
  {
    (*it)->mStatus = LERemoteDataStatus::DS_WRONG;
    if(readRegs(
          mspMaster.data(),
          (*it)->mAddr,
          (*it)->mSize,
          (*it)->mpData).status == LQModbusMasterBase::SReply::EStatus::OK)
    {
      (*it)->mStatus = LERemoteDataStatus::DS_OK;
    }
    (*it)->notifyReaders();
    if((*it)->mReadersList.isEmpty()) mReadDataList.erase(it);
  }
}

//==============================================================================CControllerHoldingRegisters
LQModbusDataSource::CControllerHoldingRegisters::CControllerHoldingRegisters(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  CControllerRegistersBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerHoldingRegisters::~CControllerHoldingRegisters()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerHoldingRegisters:: readRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  return master->readHoldingRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerHoldingRegisters::writeRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  if(_size == 1)
  {
    return master->writeSingleRegister( mDevId, _addr, _regs[0]);
  }
  return master->writeMultipleRegisters( mDevId, _addr, _size, _regs);
}

//==============================================================================CControllerInputRegisters
LQModbusDataSource::CControllerInputRegisters::CControllerInputRegisters(
    const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerRegistersBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerInputRegisters::
~CControllerInputRegisters()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerInputRegisters::readRegs(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
  return master->readInputRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerInputRegisters::writeRegs(
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
void LQModbusDataSource::CControllerBitsBase::CDataItem::notifyReaders()
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

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerBitsBase::CControllerBitsBase(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  mDevId(_devId),
  mspMaster(_master)
{
  mBitsBuff = new quint8[MODBUS_MAX_READ_COIL_COUNT + 1];
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerBitsBase::
~CControllerBitsBase()
{
  delete [] mBitsBuff;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::addReadDataItem(
    CDataItem* _dataItem)
{
  if(mReadDataList.contains(_dataItem)) return;
  mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::deleteReadDataItem(
    CDataItem* _dataItem)
{
  mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::read(
    quint16 _addr, quint16 _size, QSharedPointer<LQModbusDataReader> _reader)
{
  LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

  if(!mspMaster.isNull())
  {
    if(readBits(mspMaster.data(), _addr, _size, mBitsBuff).status ==
        LQModbusMasterBase::SReply::EStatus::OK)
    {
      status = LERemoteDataStatus::DS_OK;
    }
  }
  _reader->notifyListener(QByteArray((char*)mBitsBuff, _size), status);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::write(
    quint16 _addr,
    quint16 _size,
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

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
        status = LERemoteDataStatus::DS_OK;
    }
  }
  _writer->notifyListener(status);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::update()
{
  if(mspMaster.isNull()) return;
  {
    //TODO: Проработать опережающий ответ
    //для всех слушателей при нескольких таймаутах.
    for(auto it = mReadDataList.begin(); it != mReadDataList.end(); it++)
    {
      (*it)->mStatus = LERemoteDataStatus::DS_WRONG;
      if(readBits(
            mspMaster.data(),
            (*it)->mAddr,
            (*it)->mSize,
            (*it)->mpData).status ==
          LQModbusMasterBase::SReply::EStatus::OK)
      {
        (*it)->mStatus = LERemoteDataStatus::DS_OK;
      }
      (*it)->notifyReaders();
      if((*it)->mReadersList.isEmpty()) mReadDataList.erase(it);
    }
  }
}

//==============================================================================CControllerDiscreteInputs
LQModbusDataSource::CControllerCoils::
CControllerCoils(const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerBitsBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerCoils::
~CControllerCoils()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerCoils::readBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

  return master->readCoils(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerCoils::writeBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
  if(_size == 1)
  {
    return master->writeSingleCoils( mDevId, _addr, _bits[0]);
  }
  return master->writeMultipleCoils( mDevId, _addr, _size, _bits);
}

//==============================================================================CControllerDiscreteInputs
LQModbusDataSource::CControllerDiscreteInputs::
CControllerDiscreteInputs(const quint8& _devId,
    QWeakPointer<LQModbusMasterBase> _master) :
  CControllerBitsBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerDiscreteInputs::
~CControllerDiscreteInputs()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerDiscreteInputs::readBits(
    LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
  return master->readDiscreteInputs(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
LQModbusDataSource::CControllerDiscreteInputs::writeBits(
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
LQModbusDataSource::CDataMapItemRegsBase::CDataMapItemRegsBase(
    quint16 _addr,
    quint16 _size,
    CControllerRegistersBase& _controller):
  mDataItem(_addr, _size),
  mController(_controller)
{

}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMapItemRegsBase::~CDataMapItemRegsBase()
{
  mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemRegsBase::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
  mDataItem.mReadersList << _reader;
  qDebug() << 
    "LQModbusDataSource::CDataMapItemRegsBase::connectReader mReadersList size = " 
    << mDataItem.mReadersList.size();
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemRegsBase::disconnectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  mDataItem.mReadersList.removeAll(_reader);
  if(mDataItem.mReadersList.isEmpty())
    mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemRegsBase::read(
    QSharedPointer<LQModbusDataReader> _reader)
{
  mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==============================================================================CDataMapItemHoldingRegs
LQModbusDataSource::CDataMapItemHoldingRegs::CDataMapItemHoldingRegs(
    quint16 _addr,
    quint16 _size,
    CControllerHoldingRegisters& _controller):
  CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMapItemHoldingRegs::~CDataMapItemHoldingRegs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemHoldingRegs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//==============================================================================CDataMapItemInputRegs
LQModbusDataSource::CDataMapItemInputRegs::CDataMapItemInputRegs(
    quint16 _addr,
    quint16 _size,
    CControllerInputRegisters& _controller):
  CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMapItemInputRegs::~CDataMapItemInputRegs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemInputRegs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(LERemoteDataStatus::DS_WRONG);
}

//==============================================================================CDataMapItemBitsBase
LQModbusDataSource::
CDataMapItemBitsBase::
CDataMapItemBitsBase(quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller): mDataItem(_addr, _size),
  mController(_controller)
{

}

//------------------------------------------------------------------------------
LQModbusDataSource::
CDataMapItemBitsBase::
~CDataMapItemBitsBase()
{
  mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemBitsBase::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
  if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
  mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
CDataMapItemBitsBase::disconnectReader(QWeakPointer<LQModbusDataReader> _reader)
{
  mDataItem.mReadersList.removeOne(_reader);
  if(mDataItem.mReadersList.isEmpty())
    mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
CDataMapItemBitsBase::read(QSharedPointer<LQModbusDataReader> _reader)
{
  mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==============================================================================CDataMapItemCoils
LQModbusDataSource::CDataMapItemCoils::CDataMapItemCoils(
    quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMapItemCoils::~CDataMapItemCoils()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
CDataMapItemCoils::write(
    const QByteArray& _data, QSharedPointer<LQModbusDataWriter> _writer)
{
  mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//==============================================================================CDataMapItemInputRegs
LQModbusDataSource::CDataMapItemDiscreteInputs::CDataMapItemDiscreteInputs(
    quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMapItemDiscreteInputs::~CDataMapItemDiscreteInputs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMapItemDiscreteInputs::write(
    const QByteArray& _data,
    QSharedPointer<LQModbusDataWriter> _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(LERemoteDataStatus::DS_WRONG);
}

//==============================================================================CDataMap
LQModbusDataSource::CDataMap::CDataMap(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :
  mControllerHoldingRegs(_devId, _master),
  mControllerInputRegs(_devId, _master),
  mControllerCoils(_devId, _master),
  mControllerDiscreteInputs(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMap::~CDataMap()
{
  for(auto it = mMapItems.begin(); it != mMapItems.end(); it++)
  {
    delete *it;
  }
  mMapItems.clear();
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::update()
{
  mControllerHoldingRegs.update();
  mControllerInputRegs.update();
  mControllerCoils.update();
  mControllerDiscreteInputs.update();
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::addItemHoldingRegs(
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
void LQModbusDataSource::CDataMap::addItemInputRegs(
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
void LQModbusDataSource::CDataMap::addItemDiscreteInputs(
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
void LQModbusDataSource::CDataMap::addItemCoils(
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
void LQModbusDataSource::CDataMap::write(
    QSharedPointer<LQModbusDataWriter> _writer, const QByteArray& _data)
{
  auto it = mMapItems.find(_writer->getDataName());
  if(it == mMapItems.end())
  {
    _writer->notifyListener(LERemoteDataStatus::DS_WRONG);
  }
  else
  {
    (*it)->write(_data, _writer);
  }
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::read(QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end())
  {
    _reader->notifyListener(LERemoteDataStatus::DS_UNDEF);
  }
  else
  {
    (*it)->read(_reader);
  }
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::connectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end()) return;
  (*it)->connectReader(_reader);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::disconnectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  auto it = mMapItems.find(_reader->getDataName());
  if(it == mMapItems.end()) return;
  (*it)->disconnectReader(_reader);
}

//==============================================================================LCQModbusDataSource
LQModbusDataSource::LQModbusDataSource(
    quint8 _devId,
    QSharedPointer<LQModbusMasterBase> _modbusMaster):
  QObject(nullptr),
  mDevId(_devId),
  mModbusMaster(_modbusMaster),
  mUpdateInterval(500),
  mTimer(this),
  mDataMap(mDevId, _modbusMaster)
{
  connect(&mTimer, &QTimer::timeout,[=](){
    mDataMap.update(); });
}

//------------------------------------------------------------------------------
LQModbusDataSource::~LQModbusDataSource()
{
}

//------------------------------------------------------------------------------
static void doDeleteLater(QObject* _obj)
{
  _obj->deleteLater();
}

//------------------------------------------------------------------------------
QSharedPointer<LQModbusDataSource> LQModbusDataSource::create(
    quint8 _devId,
    QSharedPointer<LQModbusMasterBase> _modbusMaster)
{
  QSharedPointer<LQModbusDataSource> source(
      new LQModbusDataSource( _devId, _modbusMaster), doDeleteLater);
  source->mwpThis = source;
  return source;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemHoldingRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemHoldingRegs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemInputRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemInputRegs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemDiscreteInputs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemDiscreteInputs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemCoils(
    const QString& _name, quint16 _addr, quint16 _size)
{
  mDataMap.addItemCoils(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::start(int _updateIntervalMs)
{
  QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

void LQModbusDataSource::start(
    QSharedPointer<QThread> _thread, int _updateIntervalMs)
{
  if(!mspThread.isNull()) return;

  mspThread = _thread;
  moveToThread(mspThread.data());
  if(!mspThread->isRunning()) mspThread->start();

  _updateIntervalMs = (_updateIntervalMs > 50) ? (_updateIntervalMs) : (50);

  QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

//------------------------------------------------------------------------------stop
void LQModbusDataSource::stop()
{
  QCoreApplication::postEvent(this, new CQEventStop);
}

//------------------------------------------------------------------------------readConnect
void LQModbusDataSource::connectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqConnectReader(_reader));
}

//------------------------------------------------------------------------------readDisconnect
void LQModbusDataSource::disconnectReader(
    QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqDisconnectReader(_reader));
}

//------------------------------------------------------------------------------write
void LQModbusDataSource::read(QSharedPointer<LQModbusDataReader> _reader)
{
  QCoreApplication::postEvent(this, new CQEventReqRead(_reader));
}

//------------------------------------------------------------------------------write
void LQModbusDataSource::write(
    QSharedPointer<LQModbusDataWriter> _writer, const QByteArray& _data)
{
  QCoreApplication::postEvent( this, new CQEventReqWrite(_writer, _data));
}

//------------------------------------------------------------------------------customEvent
void LQModbusDataSource::customEvent(QEvent* _event)
{
  if(_event->type() == CQEventBase::msExtendedEventType)
  {
    static_cast<CQEventBase*>(_event)->handle(this);
  }
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataReader> LQModbusDataSource::createReader(
    const QString& _dataName,
    QWeakPointer<LIRemoteDataReadListener> _readListener)
{
  return LQModbusDataReader::create(_dataName, _readListener, mwpThis);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataWriter> LQModbusDataSource::createWriter(
    const QString& _dataName,
    LTWriteListener _writeListener)
    /* QWeakPointer<LIRemoteDataWriteListener> _writeListener) */
{
  return LQModbusDataWriter::create(_dataName, _writeListener, mwpThis);
}

