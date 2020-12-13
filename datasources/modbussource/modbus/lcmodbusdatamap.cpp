#include "lcmodbusdatamap.h"
#include "lqmodbusdatawriter.h"


/* namespace modbus */
/* { */ 

/* //------------------------------------------------------------------------------ */
/* CControllerRegistersBase::CControllerRegistersBase(const quint8& _devId, */
/*     QSharedPointer<LQModbusMasterBase> _master) : */ 
/*   mDevId(_devId), */
/*   mwpMaster(_master) */
/* { */
/*   mRegBuff = new quint16[MODBUS_MAX_READ_REGISTER_COUNT + 1]; */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerRegistersBase::~CControllerRegistersBase() */
/* { */
/*   delete [] mRegBuff; */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerRegistersBase::addReadDataItem( */
/*     CDataItem* _dataItem) */
/* { */
/*   if(mReadDataList.contains(_dataItem)) return; */
/*   mReadDataList << _dataItem; */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerRegistersBase::deleteReadDataItem( */
/*     CDataItem* _dataItem) */
/* { */
/*   mReadDataList.removeOne(_dataItem); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerRegistersBase::read( */
/*     quint16 _addr, quint16 _size, QObject* _reader) */
/* { */
/*   //    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock(); */

/*   LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG; */

/*   if(!mwpMaster.isNull()) */
/*   { */
/*     if(readRegs(mwpMaster.data(), _addr, _size, mRegBuff).status == */
/*         LQModbusMasterBase::SReply::EStatus::OK) */
/*     { */
/*       status = LERemoteDataStatus::DS_OK; */
/*     } */
/*   } */

/*   QCoreApplication::postEvent( _reader, */
/*       new LQModbusDataReader::CQEventDataIsRead( */
/*         QByteArray( (char*)mRegBuff, _size * 2), */
/*         status)); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerRegistersBase::write(quint16 _addr, */
/*     quint16 _size, */
/*     const QByteArray& _data, */
/*     LQModbusDataWriter* _writer) */
/* { */
/*   LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG; */

/*   if(_data.size() % 2 == 0) */
/*   { */
/*     quint16 length = _data.size() >> 1; */
/*     if(length == _size) */
/*     { */
/*       //            QSharedPointer<LCModbusMasterBase> sp = mwpMaster.lock(); */
/*       if(!mwpMaster.isNull()) */
/*       { */
/*         if(writeRegs( */
/*               mwpMaster.data(), */ 
/*               _addr, */ 
/*               length, */ 
/*               ((quint16*)_data.constData())).status == */
/*             LQModbusMasterBase::SReply::EStatus::OK) */
/*           status = LERemoteDataStatus::DS_OK; */
/*       } */
/*     } */
/*   } */
/*   _writer->notifyListener(status); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerRegistersBase::update() */
/* { */
/*   //    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock(); */
/*   if(mwpMaster.isNull()) return; */
/*   { */
/*     //TODO: Проработать опережающий ответ для всех */ 
/*     //слушателей при нескольких таймаутах. */
/*     QLinkedList<CDataItem*>::iterator it = mReadDataList.begin(); */
/*     QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end(); */
/*     for(;it != it_end; it++) */
/*     { */
/*       (*it)->mStatus = LERemoteDataStatus::DS_WRONG; */
/*       if(readRegs( */
/*             mwpMaster.data(), */ 
/*             (*it)->mAddr, */ 
/*             (*it)->mSize, */ 
/*             (*it)->mpData).status == */
/*           LQModbusMasterBase::SReply::EStatus::OK) */
/*       { */
/*         (*it)->mStatus = LERemoteDataStatus::DS_OK; */
/*       } */
/*       (*it)->notifyReaders(); */
/*     } */
/*   } */
/* } */

/* //==============================================================================CControllerHoldingRegisters */
/* CControllerHoldingRegisters:: */
/* CControllerHoldingRegisters(const quint8& _devId, */
/*     QSharedPointer<LQModbusMasterBase> _master) : */
/*   CControllerRegistersBase(_devId, _master) */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerHoldingRegisters:: */
/* ~CControllerHoldingRegisters() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */ 
/* CControllerHoldingRegisters:: readRegs( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[]) */
/* { */
/*   return master->readHoldingRegisters(mDevId, _addr, _size, _regs); */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerHoldingRegisters::writeRegs( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[]) */
/* { */
/*   if(_size == 1) */
/*   { */
/*     return master->writeSingleRegister( mDevId, _addr, _regs[0]); */
/*   } */
/*   return master->writeMultipleRegisters( mDevId, _addr, _size, _regs); */
/* } */

/* //==============================================================================CControllerInputRegisters */
/* CControllerInputRegisters::CControllerInputRegisters( */
/*     const quint8& _devId, */
/*     QWeakPointer<LQModbusMasterBase> _master) : */
/*   CControllerRegistersBase(_devId, _master) */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerInputRegisters:: */
/* ~CControllerInputRegisters() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerInputRegisters::readRegs( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[]) */
/* { */
/*   return master->readInputRegisters(mDevId, _addr, _size, _regs); */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerInputRegisters::writeRegs( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[]) */
/* { */
/*   Q_UNUSED(master); */
/*   Q_UNUSED(_addr); */
/*   Q_UNUSED(_size); */
/*   Q_UNUSED(_regs); */
/*   return LQModbusMasterBase::SReply( */
/*       LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0); */
/* } */

/* //==============================================================================CControllerRegistersBase */
/* void CControllerBitsBase::CDataItem::notifyReaders() */
/* { */
/*   QLinkedList<QObject*>::iterator it = mReadersList.begin(); */
/*   QLinkedList<QObject*>::iterator it_end = mReadersList.end(); */
/*   for(;it != it_end; it++) */
/*   { */
/*     QCoreApplication::postEvent( */
/*         *it, */ 
/*         new LQModbusDataReader::CQEventDataIsRead( */
/*           QByteArray( (char*)(mpData), mSize), mStatus)); */
/*   } */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerBitsBase:: */
/* CControllerBitsBase(const quint8& _devId, */
/*     QWeakPointer<LQModbusMasterBase> _master) :  mDevId(_devId), */
/*   mwpMaster(_master) */
/* { */
/*   mBitsBuff = new quint8[MODBUS_MAX_READ_COIL_COUNT + 1]; */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerBitsBase:: */
/* ~CControllerBitsBase() */
/* { */
/*   delete [] mBitsBuff; */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerBitsBase::addReadDataItem( */
/*     CDataItem* _dataItem) */
/* { */
/*   if(mReadDataList.contains(_dataItem)) return; */
/*   mReadDataList << _dataItem; */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerBitsBase::deleteReadDataItem( */
/*     CDataItem* _dataItem) */
/* { */
/*   mReadDataList.removeOne(_dataItem); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerBitsBase::read( */
/*     quint16 _addr, quint16 _size, QObject* _reader) */
/* { */
/*   QSharedPointer<LQModbusMasterBase> master = mwpMaster.lock(); */

/*   LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG; */

/*   if(!master.isNull()) */
/*   { */
/*     if(readBits(master.data(), _addr, _size, mBitsBuff).status == */
/*         LQModbusMasterBase::SReply::EStatus::OK) */
/*     { */
/*       status = LERemoteDataStatus::DS_OK; */
/*     } */
/*   } */

/*   QCoreApplication::postEvent( _reader, */
/*       new LQModbusDataReader::CQEventDataIsRead( */
/*         QByteArray( (char*)mBitsBuff, _size), */
/*         status)); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerBitsBase::write( */
/*     quint16 _addr, */
/*     quint16 _size, */
/*     const QByteArray& _data, */
/*     LQModbusDataWriter* _writer) */
/* { */
/*   LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG; */

/*   if(_data.size() == _size) */
/*   { */
/*     QSharedPointer<LQModbusMasterBase> sp = mwpMaster.lock(); */
/*     if(!sp.isNull()) */
/*     { */
/*       if(writeBits( */
/*             sp.data(), */ 
/*             _addr, */ 
/*             _size, */ 
/*             ((quint8*)_data.constData())).status == */
/*           LQModbusMasterBase::SReply::EStatus::OK) */
/*         status = LERemoteDataStatus::DS_OK; */
/*     } */
/*   } */
/*   _writer->notifyListener(status); */
/* } */

/* //------------------------------------------------------------------------------ */
/* void CControllerBitsBase::update() */
/* { */
/*   QSharedPointer<LQModbusMasterBase> master = mwpMaster.lock(); */
/*   if(master.isNull()) return; */
/*   { */
/*     //TODO: Проработать опережающий ответ */ 
/*     //для всех слушателей при нескольких таймаутах. */
/*     QLinkedList<CDataItem*>::iterator it = mReadDataList.begin(); */
/*     QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end(); */
/*     for(;it != it_end; it++) */
/*     { */
/*       (*it)->mStatus = LERemoteDataStatus::DS_WRONG; */
/*       if(readBits( */
/*             master.data(), */ 
/*             (*it)->mAddr, */ 
/*             (*it)->mSize, */ 
/*             (*it)->mpData).status == */
/*           LQModbusMasterBase::SReply::EStatus::OK) */
/*       { */
/*         (*it)->mStatus = LERemoteDataStatus::DS_OK; */
/*       } */
/*       (*it)->notifyReaders(); */
/*     } */
/*   } */
/* } */

/* //==============================================================================CControllerDiscreteInputs */
/* CControllerCoils:: */
/* CControllerCoils(const quint8& _devId, */
/*     QWeakPointer<LQModbusMasterBase> _master) : */
/*   CControllerBitsBase(_devId, _master) */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerCoils:: */
/* ~CControllerCoils() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerCoils::readBits( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[]) */
/* { */

/*   return master->readCoils(mDevId, _addr, _size, _bits); */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerCoils::writeBits( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[]) */
/* { */
/*   if(_size == 1) */
/*   { */
/*     return master->writeSingleCoils( mDevId, _addr, _bits[0]); */
/*   } */
/*   return master->writeMultipleCoils( mDevId, _addr, _size, _bits); */
/* } */

/* //==============================================================================CControllerDiscreteInputs */
/* CControllerDiscreteInputs:: */
/* CControllerDiscreteInputs(const quint8& _devId, */
/*     QWeakPointer<LQModbusMasterBase> _master) : */
/*   CControllerBitsBase(_devId, _master) */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* CControllerDiscreteInputs:: */
/* ~CControllerDiscreteInputs() */
/* { */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerDiscreteInputs::readBits( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[]) */
/* { */
/*   return master->readDiscreteInputs(mDevId, _addr, _size, _bits); */
/* } */

/* //------------------------------------------------------------------------------ */
/* LQModbusMasterBase::SReply */
/* CControllerDiscreteInputs::writeBits( */
/*     LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[]) */
/* { */
/*   Q_UNUSED(master); */
/*   Q_UNUSED(_addr); */
/*   Q_UNUSED(_size); */
/*   Q_UNUSED(_bits); */
/*   return LQModbusMasterBase::SReply( */
/*       LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0); */
/* } */

//==============================================================================CDataMapItemRegsBase
CDataMap::CDataMapItemRegsBase::CDataMapItemRegsBase(
  quint16 _addr, 
  quint16 _size, 
  CDataMap& _dataMap): 
CDataMapItemBase(_dataMap),
mAddr(_addr),
mSize(_size)
{
}

//------------------------------------------------------------------------------
CDataMap::CDataMapItemRegsBase::~CDataMapItemRegsBase()
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemRegsBase::connectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemRegsBase::disconnectReader(
    QWeakPointer<LQModbusDataReader> _reader)
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemRegsBase::read(LQModbusDataWriter* _reader)
{
}

//==============================================================================CDataMapItemHoldingRegs
CDataMap::CDataMapItemHoldingRegs::CDataMapItemHoldingRegs(
    quint16 _addr,
    quint16 _size,
    CDataMap& _dataMap):
  CDataMapItemRegsBase(_addr, _size, _dataMap)
{
}

//------------------------------------------------------------------------------

CDataMap::CDataMapItemHoldingRegs::~CDataMapItemHoldingRegs()
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemHoldingRegs::write(const QByteArray& _data, 
        LQModbusDataWriter* _writer)
{
}

//==============================================================================CDataMapItemInputRegs
CDataMap::CDataMapItemInputRegs::CDataMapItemInputRegs(  
    quint16 _addr,
    quint16 _size,
    CDataMap& _dataMap):
  CDataMapItemRegsBase(_addr, _size, _dataMap)
{
}

//------------------------------------------------------------------------------

CDataMap::CDataMapItemInputRegs::~CDataMapItemInputRegs()
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemInputRegs::write(
    const QByteArray& _data, 
    LQModbusDataWriter* _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(LERemoteDataStatus::DS_WRONG);
}

//==============================================================================CDataMapItemBitsBase
CDataMap::CDataMapItemBitsBase::CDataMapItemBitsBase(
    quint16 _addr,
    quint16 _size,
    CDataMap& _dataMap): 
  CDataMapItemBase(_dataMap)
{

}

//------------------------------------------------------------------------------
CDataMap::CDataMapItemBitsBase:: ~CDataMapItemBitsBase()
{
  mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemBitsBase::connectReader(QObject* _reader)
{
  if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
  mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemBitsBase::disconnectReader(QObject* _reader)
{
  mDataItem.mReadersList.removeOne(_reader);
  if(mDataItem.mReadersList.isEmpty()) 
    mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemBitsBase::read(QObject* _reader)
{
  mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==============================================================================CDataMapItemCoils
CDataMap::CDataMapItemCoils::CDataMapItemCoils(quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
CDataMap::CDataMapItemCoils::~CDataMapItemCoils()
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemCoils::write(const QByteArray& _data, QObject* _writer)
{
  mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//==============================================================================CDataMapItemInputRegs
CDataMap::CDataMapItemDiscreteInputs::CDataMapItemDiscreteInputs(  
    quint16 _addr,
    quint16 _size,
    CControllerBitsBase& _controller):
  CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
CDataMap::CDataMapItemDiscreteInputs::~CDataMapItemDiscreteInputs()
{
}

//------------------------------------------------------------------------------
void CDataMap::CDataMapItemDiscreteInputs::write(
    const QByteArray& _data, 
    LQModbusDataWriter* _writer)
{
  Q_UNUSED(_data);
  Q_UNUSED(_writer);
  _writer->notifyListener(LERemoteDataStatus::DS_WRONG);
}

//==============================================================================CDataMap
CDataMap::CDataMap(
    const quint8& _devId,
    QSharedPointer<LQModbusMasterBase> _master) :  
  mControllerHoldingRegs(_devId, _master),
  mControllerInputRegs(_devId, _master),
  mControllerCoils(_devId, _master),
  mControllerDiscreteInputs(_devId, _master)
{
}

//------------------------------------------------------------------------------
CDataMap::~CDataMap()
{
  MTItemIterator it = mMapItems.begin();
  for(;it != mMapItems.end(); it++)
  {
    delete *it;
  }
  mMapItems.clear();
  mMapReaders.clear();
}

//------------------------------------------------------------------------------
void CDataMap::update()
{
  mControllerHoldingRegs.update();
  mControllerInputRegs.update();
  mControllerCoils.update();
  mControllerDiscreteInputs.update();
}

//------------------------------------------------------------------------------
void CDataMap::addItemHoldingRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, 
      new CDataMapItemHoldingRegs(_addr, _size, mControllerHoldingRegs));
}

//------------------------------------------------------------------------------
void CDataMap::addItemInputRegs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, new CDataMapItemInputRegs(_addr, _size, mControllerInputRegs));
}

//------------------------------------------------------------------------------
void CDataMap::addItemDiscreteInputs(
    const QString& _name, quint16 _addr, quint16 _size)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, 
      new CDataMapItemDiscreteInputs(_addr, _size, mControllerDiscreteInputs));
}

//------------------------------------------------------------------------------
void CDataMap::addItemCoils(
    const QString& _name, quint16 _addr, quint16 _size)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it != mMapItems.end())
  {
    delete it.value();
  }
  mMapItems.insert(
      _name, new CDataMapItemCoils(_addr, _size, mControllerCoils));
}

//------------------------------------------------------------------------------
bool CDataMap::write(
    const QString& _name, 
    const QByteArray& _data, QObject* _reader)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it == mMapItems.end()) return false;
  (*it)->write(_data, _reader);
  return true;
}

//------------------------------------------------------------------------------
bool CDataMap::read(const QString& _name, QObject* _reader)
{
  MTItemIterator it = mMapItems.find(_name);
  if(it == mMapItems.end()) return false;
  (*it)->read(_reader);
  return true;
}

//------------------------------------------------------------------------------
bool CDataMap::connectReader(
    const QString& _name, QObject* _reader)
{
  if(mMapReaders.contains(_reader)) return false;
  MTItemIterator it = mMapItems.find(_name);
  if(it == mMapItems.end()) return false;
  (*it)->connectReader(_reader);
  mMapReaders.insert(_reader, (*it));
  return true;
}

//------------------------------------------------------------------------------
bool CDataMap::disconnectReader(QObject* _reader)
{
  MTReadersIterator it = mMapReaders.find(_reader);
  if(it == mMapReaders.end()) return false;
  (*it)->disconnectReader(_reader);
  mMapReaders.erase(it);
  return true;
}

/* } */
