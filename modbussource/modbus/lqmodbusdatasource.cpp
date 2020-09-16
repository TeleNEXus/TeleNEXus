#include <QThread>
#include <typeinfo>
#include <QCoreApplication>
#include <QDebug>

#include "lqmodbusdatasource.h"
#include "lmodbusdefs.h"

#include "lqmodbusdatareader.h"
#include "lqmodbusdatawriter.h"

namespace modbus
{

//==============================================================================CQEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LQModbusDataSource::CQEventBase);

LQModbusDataSource::CQEventBase::CQEventBase() : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//===========================================================================CQEventReqRead
LQModbusDataSource::CQEventStart::CQEventStart(int _updateInterval) : mUpdateInterval(_updateInterval)
{
}

//-----------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventStart::handle(LQModbusDataSource* _base)
{
    _base->mUpdateInterval = mUpdateInterval;
    _base->mTimer.start(mUpdateInterval);
}

//===========================================================================CQEventReqRead
LQModbusDataSource::CQEventStop::CQEventStop()
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventStop::handle(LQModbusDataSource* _base)
{
    _base->mTimer.stop();
}

//===========================================================================CQEventReqRead
LQModbusDataSource::
    CQEventReqRead::
        CQEventReqRead( const QString&  _dataName,
                        QObject*        _reader) :      mDataName(_dataName),
                                                        mpReader(_reader)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::
        CQEventReqRead::handle(LQModbusDataSource* _base)
{
    if(_base->mDataMap.read(mDataName,mpReader)) return;
    QCoreApplication::postEvent( mpReader,
                                new LQModbusDataReader::CQEventDataIsRead(LERemoteDataStatus::DS_WRONG));
}

//==============================================================================CQEventReqWrite
LQModbusDataSource::
    CQEventReqWrite::
        CQEventReqWrite(const QString& _dataName,
                        const QByteArray& _data,
                        QObject* _writer) : mDataName(_dataName),
                                            mData(_data),
                                            mpWriter(_writer)
{

}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::
        CQEventReqWrite::handle(LQModbusDataSource* _base)
{
    if(_base->mDataMap.write(mDataName, mData, mpWriter)) return;
    QCoreApplication::postEvent( mpWriter, 
            new LQModbusDataWriter::
                CQEventDataIsWrite(LERemoteDataStatus::DS_WRONG));
}
//=================================================CQEventReqConnectReader
LQModbusDataSource::
                CQEventReqConnectReader::
                    CQEventReqConnectReader(const QString& _dataName, 
                                            QObject* _reader) :
                                                        mDataName(_dataName),
                                                        mpReader(_reader)
{

}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventReqConnectReader::handle(LQModbusDataSource* _base)
{
    _base->mDataMap.connectReader(mDataName, mpReader);
}
//=================================================CQEventReqDisconnectReader
LQModbusDataSource::
    CQEventReqDisconnectReader::
        CQEventReqDisconnectReader(QObject* _reader) :  mpReader(_reader)
{
}

//------------------------------------------------------------------------------handle
void LQModbusDataSource::CQEventReqDisconnectReader::handle(LQModbusDataSource* _base)
{
    _base->mDataMap.disconnectReader(mpReader);
}

//=================================================CControllerRegistersBase
void LQModbusDataSource::CControllerRegistersBase::CDataItem::notifyReaders()
{
    QLinkedList<QObject*>::iterator it = mReadersList.begin();
    QLinkedList<QObject*>::iterator it_end = mReadersList.end();
    for(;it != it_end; it++)
    {
        QCoreApplication::postEvent( *it,
                new LQModbusDataReader::CQEventDataIsRead(QByteArray(
                                                               (char*)(mpData), mSize * 2), 
                                                                mStatus));
    }
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerRegistersBase::
        CControllerRegistersBase(const quint8& _devId,
                                    QSharedPointer<LQModbusMasterBase> _master) : 
                                                    mDevId(_devId),
                                                    mwpMaster(_master)
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
void LQModbusDataSource::CControllerRegistersBase::addReadDataItem(CDataItem* _dataItem)
{
    if(mReadDataList.contains(_dataItem)) return;
    mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::deleteReadDataItem(CDataItem* _dataItem)
{
    mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::read(quint16 _addr, quint16 _size, QObject* _reader)
{
//    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();

    LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

    if(!mwpMaster.isNull())
    {
        if(readRegs(mwpMaster.data(), _addr, _size, mRegBuff).status ==
                LQModbusMasterBase::SReply::EStatus::OK)
        {
            status = LERemoteDataStatus::DS_OK;
        }
    }

    QCoreApplication::postEvent( _reader,
                                new LQModbusDataReader::CQEventDataIsRead(
                                    QByteArray( (char*)mRegBuff, _size * 2),
                                    status));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::write(quint16 _addr,
                                                             quint16 _size,
                                                             const QByteArray& _data,
                                                             QObject* _writer)
{
    LERemoteDataStatus status = LERemoteDataStatus::DS_UNDEF;

    status = LERemoteDataStatus::DS_WRONG;
    if(_data.size() % 2 == 0)
    {
        quint16 length = _data.size() >> 1;
        if(length == _size)
        {
//            QSharedPointer<LCModbusMasterBase> sp = mwpMaster.lock();
            if(!mwpMaster.isNull())
            {
                if(writeRegs(mwpMaster.data(), _addr, length, ((quint16*)_data.constData())).status ==
                        LQModbusMasterBase::SReply::EStatus::OK)
                    status = LERemoteDataStatus::DS_OK;
            }
        }
    }
    QCoreApplication::postEvent(_writer, new LQModbusDataWriter::CQEventDataIsWrite(status));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerRegistersBase::update()
{
//    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();
    if(mwpMaster.isNull()) return;
    {
        //TODO: Проработать опережающий ответ для всех слушателей при нескольких таймаутах.
        QLinkedList<CDataItem*>::iterator it = mReadDataList.begin();
        QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end();
        for(;it != it_end; it++)
        {
            (*it)->mStatus = LERemoteDataStatus::DS_WRONG;
            if(readRegs(mwpMaster.data(), (*it)->mAddr, (*it)->mSize, (*it)->mpData).status ==
                    LQModbusMasterBase::SReply::EStatus::OK)
            {
                (*it)->mStatus = LERemoteDataStatus::DS_OK;
            }
            (*it)->notifyReaders();
        }
    }
}

//==============================================================CControllerHoldingRegisters
LQModbusDataSource::CControllerHoldingRegisters::
        CControllerHoldingRegisters(const quint8& _devId,
                                    QSharedPointer<LQModbusMasterBase> _master) :
                                                    CControllerRegistersBase(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerHoldingRegisters::
        ~CControllerHoldingRegisters()
{
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
            LQModbusDataSource::
                CControllerHoldingRegisters::
                    readRegs(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{

    return master->readHoldingRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
        LQModbusDataSource::
            CControllerHoldingRegisters::
                writeRegs(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    if(_size == 1)
    {
        return master->writeSingleRegister( mDevId, _addr, _regs[0]);
    }
    return master->writeMultipleRegisters( mDevId, _addr, _size, _regs);
}

//================================================================CControllerInputRegisters
LQModbusDataSource::CControllerInputRegisters::
        CControllerInputRegisters(const quint8& _devId,
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
            LQModbusDataSource::
                CControllerInputRegisters::
                    readRegs(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    return master->readInputRegisters(mDevId, _addr, _size, _regs);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
        LQModbusDataSource::
            CControllerInputRegisters::
                writeRegs(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    Q_UNUSED(master);
    Q_UNUSED(_addr);
    Q_UNUSED(_size);
    Q_UNUSED(_regs);
    return LQModbusMasterBase::SReply(LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}

//=================================================================CControllerRegistersBase
void LQModbusDataSource::CControllerBitsBase::CDataItem::notifyReaders()
{
    QLinkedList<QObject*>::iterator it = mReadersList.begin();
    QLinkedList<QObject*>::iterator it_end = mReadersList.end();
    for(;it != it_end; it++)
    {
        QCoreApplication::postEvent(
                    *it, new LQModbusDataReader::CQEventDataIsRead(QByteArray( (char*)(mpData), mSize), mStatus));
    }
}

//------------------------------------------------------------------------------
LQModbusDataSource::CControllerBitsBase::
        CControllerBitsBase(const quint8& _devId,
                                    QWeakPointer<LQModbusMasterBase> _master) :  mDevId(_devId),
                                                                                 mwpMaster(_master)
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
void LQModbusDataSource::CControllerBitsBase::addReadDataItem(CDataItem* _dataItem)
{
    if(mReadDataList.contains(_dataItem)) return;
    mReadDataList << _dataItem;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::deleteReadDataItem(CDataItem* _dataItem)
{
    mReadDataList.removeOne(_dataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::read(quint16 _addr, quint16 _size, QObject* _reader)
{
    QSharedPointer<LQModbusMasterBase> master = mwpMaster.lock();

    LERemoteDataStatus status = LERemoteDataStatus::DS_WRONG;

    if(!master.isNull())
    {
        if(readBits(master.data(), _addr, _size, mBitsBuff).status ==
                LQModbusMasterBase::SReply::EStatus::OK)
        {
            status = LERemoteDataStatus::DS_OK;
        }
    }

    QCoreApplication::postEvent( _reader,
                                new LQModbusDataReader::CQEventDataIsRead(
                                    QByteArray( (char*)mBitsBuff, _size),
                                    status));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::write(quint16 _addr,
                                                             quint16 _size,
                                                             const QByteArray& _data,
                                                             QObject* _writer)
{
    LERemoteDataStatus status = LERemoteDataStatus::DS_UNDEF;

    status = LERemoteDataStatus::DS_WRONG;

    if(_data.size() == _size)
    {
        QSharedPointer<LQModbusMasterBase> sp = mwpMaster.lock();
        if(!sp.isNull())
        {
            if(writeBits(sp.data(), _addr, _size, ((quint8*)_data.constData())).status ==
                    LQModbusMasterBase::SReply::EStatus::OK)
                status = LERemoteDataStatus::DS_OK;
        }
    }
    QCoreApplication::postEvent(_writer, 
                    new LQModbusDataWriter::CQEventDataIsWrite(status));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CControllerBitsBase::update()
{
    QSharedPointer<LQModbusMasterBase> master = mwpMaster.lock();
    if(master.isNull()) return;
    {
        //TODO: Проработать опережающий ответ для всех слушателей при нескольких таймаутах.
        QLinkedList<CDataItem*>::iterator it = mReadDataList.begin();
        QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end();
        for(;it != it_end; it++)
        {
            (*it)->mStatus = LERemoteDataStatus::DS_WRONG;
            if(readBits(master.data(), (*it)->mAddr, (*it)->mSize, (*it)->mpData).status ==
                    LQModbusMasterBase::SReply::EStatus::OK)
            {
                (*it)->mStatus = LERemoteDataStatus::DS_OK;
            }
            (*it)->notifyReaders();
        }
    }
}

//================================================================CControllerDiscreteInputs
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
            LQModbusDataSource::
                CControllerCoils::
                    readBits(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

    return master->readCoils(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
        LQModbusDataSource::
            CControllerCoils::
                writeBits(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
    if(_size == 1)
    {
        return master->writeSingleCoils( mDevId, _addr, _bits[0]);
    }
    return master->writeMultipleCoils( mDevId, _addr, _size, _bits);
}

//================================================================CControllerDiscreteInputs
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
            LQModbusDataSource::
                CControllerDiscreteInputs::
                    readBits(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

    return master->readDiscreteInputs(mDevId, _addr, _size, _bits);
}

//------------------------------------------------------------------------------
LQModbusMasterBase::SReply
        LQModbusDataSource::
            CControllerDiscreteInputs::
                writeBits(LQModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
    Q_UNUSED(master);
    Q_UNUSED(_addr);
    Q_UNUSED(_size);
    Q_UNUSED(_bits);
    return LQModbusMasterBase::SReply(LQModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}

//=====================================================================CDataMapItemRegsBase
LQModbusDataSource::
                CDataMapItemRegsBase::
                    CDataMapItemRegsBase(quint16 _addr,
                                            quint16 _size,
                                            CControllerRegistersBase& _controller): mDataItem(_addr, _size),
                                                                                    mController(_controller)
{

}

//------------------------------------------------------------------------------
LQModbusDataSource::
                CDataMapItemRegsBase::
                    ~CDataMapItemRegsBase()
{
    mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemRegsBase::connectReader(QObject* _reader)
{
    if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
    mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemRegsBase::disconnectReader(QObject* _reader)
{
    mDataItem.mReadersList.removeOne(_reader);
    if(mDataItem.mReadersList.isEmpty()) mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemRegsBase::read(QObject* _reader)
{
    mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//==================================================================CDataMapItemHoldingRegs
LQModbusDataSource::
                CDataMapItemHoldingRegs::
                    CDataMapItemHoldingRegs(quint16 _addr,
                                            quint16 _size,
                                            CControllerHoldingRegisters& _controller):
                                                CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::
                CDataMapItemHoldingRegs::
                    ~CDataMapItemHoldingRegs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemHoldingRegs::write(const QByteArray& _data, QObject* _writer)
{
    mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//====================================================================CDataMapItemInputRegs
LQModbusDataSource::
                CDataMapItemInputRegs::
                    CDataMapItemInputRegs(  quint16 _addr,
                                            quint16 _size,
                                            CControllerInputRegisters& _controller):
                                            CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::
                CDataMapItemInputRegs::
                    ~CDataMapItemInputRegs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemInputRegs::write(const QByteArray& _data, QObject* _writer)
{
    Q_UNUSED(_data);
    Q_UNUSED(_writer);
    QCoreApplication::postEvent(
            _writer, 
            new LQModbusDataWriter::
                    CQEventDataIsWrite(LERemoteDataStatus::DS_WRONG));
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
void LQModbusDataSource::
        CDataMapItemBitsBase::connectReader(QObject* _reader)
{
    if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
    mDataItem.mReadersList << _reader;
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemBitsBase::disconnectReader(QObject* _reader)
{
    mDataItem.mReadersList.removeOne(_reader);
    if(mDataItem.mReadersList.isEmpty()) mController.deleteReadDataItem(&mDataItem);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemBitsBase::read(QObject* _reader)
{
    mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//========================================================================CDataMapItemCoils
LQModbusDataSource::
                CDataMapItemCoils::
                    CDataMapItemCoils(quint16 _addr,
                                            quint16 _size,
                                            CControllerBitsBase& _controller):
                                                CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::
                CDataMapItemCoils::
                    ~CDataMapItemCoils()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemCoils::write(const QByteArray& _data, QObject* _writer)
{
    mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//====================================================================CDataMapItemInputRegs
LQModbusDataSource::
                CDataMapItemDiscreteInputs::
                    CDataMapItemDiscreteInputs(  quint16 _addr,
                                            quint16 _size,
                                            CControllerBitsBase& _controller):
                                            CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::
                CDataMapItemDiscreteInputs::
                    ~CDataMapItemDiscreteInputs()
{
}

//------------------------------------------------------------------------------
void LQModbusDataSource::
        CDataMapItemDiscreteInputs::write(const QByteArray& _data, QObject* _writer)
{
    Q_UNUSED(_data);
    Q_UNUSED(_writer);
    QCoreApplication::postEvent(_writer, new LQModbusDataWriter::CQEventDataIsWrite(LERemoteDataStatus::DS_WRONG));
}

//=================================================================================CDataMap
LQModbusDataSource::
        CDataMap::
            CDataMap(const quint8& _devId,
                     QSharedPointer<LQModbusMasterBase> _master) :  mControllerHoldingRegs(_devId, _master),
                                                                    mControllerInputRegs(_devId, _master),
                                                                    mControllerCoils(_devId, _master),
                                                                    mControllerDiscreteInputs(_devId, _master)
{
}

//------------------------------------------------------------------------------
LQModbusDataSource::CDataMap::~CDataMap()
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
void LQModbusDataSource::CDataMap::update()
{
    mControllerHoldingRegs.update();
    mControllerInputRegs.update();
    mControllerCoils.update();
    mControllerDiscreteInputs.update();
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::addItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemHoldingRegs(_addr, _size, mControllerHoldingRegs));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::addItemInputRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemInputRegs(_addr, _size, mControllerInputRegs));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::addItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemDiscreteInputs(_addr, _size, mControllerDiscreteInputs));
}

//------------------------------------------------------------------------------
void LQModbusDataSource::CDataMap::addItemCoils(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemCoils(_addr, _size, mControllerCoils));
}

//------------------------------------------------------------------------------
bool LQModbusDataSource::CDataMap::write(
                        const QString& _name, 
                        const QByteArray& _data, QObject* _reader)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->write(_data, _reader);
    return true;
}

//------------------------------------------------------------------------------
bool LQModbusDataSource::CDataMap::read(const QString& _name, QObject* _reader)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->read(_reader);
    return true;
}

//------------------------------------------------------------------------------
bool LQModbusDataSource::CDataMap::connectReader(const QString& _name, QObject* _reader)
{
    if(mMapReaders.contains(_reader)) return false;
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->connectReader(_reader);
    mMapReaders.insert(_reader, (*it));
    return true;
}

//------------------------------------------------------------------------------
bool LQModbusDataSource::CDataMap::disconnectReader(QObject* _reader)
{
    MTReadersIterator it = mMapReaders.find(_reader);
    if(it == mMapReaders.end()) return false;
    (*it)->disconnectReader(_reader);
    mMapReaders.erase(it);
    return true;
}
int __i = 0;
//==============================================================================LCQModbusDataSource
LQModbusDataSource::
                LQModbusDataSource(quint8 _devId,
                                    QSharedPointer<LQModbusMasterBase> _modbusMaster,
                                    QObject *_parent) :
                                                                QObject(_parent),
                                                                mDevId(_devId),
                                                                mModbusMaster(_modbusMaster),
                                                                mUpdateInterval(500),
                                                                mTimer(this),
                                                                mDataMap(mDevId, _modbusMaster)
{
    __i++;
    connect(&mTimer, &QTimer::timeout,[=](){
        mDataMap.update(); });
}

//------------------------------------------------------------------------------~LCQModbusDataController
LQModbusDataSource::~LQModbusDataSource()
{
}

//-----------------------------------------------------------------------------doDeleteLater
static void doDeleteLater(LQModbusDataSource* _obj)
{
    _obj->deleteLater();
}

//------------------------------------------------------------------------------create
QSharedPointer<LQModbusDataSource> LQModbusDataSource::create(quint8 _devId,
                                                                QSharedPointer<LQModbusMasterBase> _modbusMaster,
                                                                QObject *_parent)
{
    return QSharedPointer<LQModbusDataSource>(new LQModbusDataSource(_devId, _modbusMaster, _parent), doDeleteLater);
}



//------------------------------------------------------------------------------addDataItemHoldingRegs
void LQModbusDataSource::addDataItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemHoldingRegs(_name, _addr, _size);
}


//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemInputRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemInputRegs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemDiscreteInputs(_name, _addr, _size);
}

//------------------------------------------------------------------------------
void LQModbusDataSource::addDataItemCoils(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemCoils(_name, _addr, _size);
}

//------------------------------------------------------------------------------start
void LQModbusDataSource::start(int _updateIntervalMs)
{
    QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

void LQModbusDataSource::start(QSharedPointer<QThread> _thread, int _updateIntervalMs)
{
    if(!mspThread.isNull()) return;

    mspThread = _thread;
    moveToThread(mspThread.data());
    if(!mspThread.data()->isRunning()) mspThread.data()->start();

    _updateIntervalMs = (_updateIntervalMs > 50) ? (_updateIntervalMs) : (50);

    QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

//------------------------------------------------------------------------------stop
void LQModbusDataSource::stop()
{
    QCoreApplication::postEvent(this, new CQEventStop);
}

//------------------------------------------------------------------------------readConnect
void LQModbusDataSource::connectReader(const QString& _dataName, QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqConnectReader(_dataName, _reader));
}

//------------------------------------------------------------------------------readDisconnect
void LQModbusDataSource::disconnectReader(QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqDisconnectReader(_reader));
}

//------------------------------------------------------------------------------write
void LQModbusDataSource::read(const QString& _dataName, QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqRead(_dataName, _reader));
}

//------------------------------------------------------------------------------write
void LQModbusDataSource::write(const QString& _dataName, const QByteArray& _data, QObject* _writer)
{
    QCoreApplication::postEvent(this, new CQEventReqWrite(_dataName, _data, _writer));
}

//------------------------------------------------------------------------------customEvent
void LQModbusDataSource::customEvent(QEvent* _event)
{
    if(_event->type() == CQEventBase::msExtendedEventType)
    {
        reinterpret_cast<CQEventBase*>(_event)->handle(this);
    }
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataReader> LQModbusDataSource::createReader()
{
    return QSharedPointer<LIRemoteDataReader>(new LQModbusDataReader());
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataWriter> LQModbusDataSource::createWriter()
{
    return QSharedPointer<LIRemoteDataWriter>(new LQModbusDataWriter());
}

} /* namespace modbus */
