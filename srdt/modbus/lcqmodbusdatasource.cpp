#include "lcqmodbusdatasource.h"
#include "lmodbusdefs.h"
#include <QThread>
#include <typeinfo>
#include <QCoreApplication>
#include <QDebug>

namespace modbus
{

//===========================================================================================================CQEventBase
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusDataSource::CQEventBase);

LCQModbusDataSource::CQEventBase::CQEventBase() : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
}

//========================================================================================================CQEventReqRead
LCQModbusDataSource::CQEventStart::CQEventStart(int _updateInterval) : mUpdateInterval(_updateInterval)
{
}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::CQEventStart::handle(LCQModbusDataSource* _base)
{
    _base->mUpdateInterval = mUpdateInterval;
    _base->mTimer.start(mUpdateInterval);
    qDebug() << "LCQModbusDataSource::CQEventStart::handle";
}

//========================================================================================================CQEventReqRead
LCQModbusDataSource::CQEventStop::CQEventStop()
{
}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::CQEventStop::handle(LCQModbusDataSource* _base)
{
    _base->mTimer.stop();
}

//========================================================================================================CQEventReqRead
LCQModbusDataSource::
    CQEventReqRead::
        CQEventReqRead( const QString&  _dataName,
                        QObject*        _reader) :      mDataName(_dataName),
                                                        mpReader(_reader)
{
}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::
        CQEventReqRead::handle(LCQModbusDataSource* _base)
{
    if(_base->mDataMap.read(mDataName,mpReader)) return;
    QCoreApplication::postEvent( mpReader,
                                new CQEventDataIsRead(EDataStatus::DS_WRONG));
}

//=======================================================================================================CQEventReqWrite
LCQModbusDataSource::
    CQEventReqWrite::
        CQEventReqWrite(const QString& _dataName,
                        const QByteArray& _data,
                        QObject* _writer) : mDataName(_dataName),
                                            mData(_data),
                                            mpWriter(_writer)
{

}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::
        CQEventReqWrite::handle(LCQModbusDataSource* _base)
{
    if(_base->mDataMap.write(mDataName,mData, mpWriter)) return;
    QCoreApplication::postEvent( mpWriter, new CQEventDataIsWrite(EDataStatus::DS_WRONG));
}
//===============================================================================================CQEventReqConnectReader
LCQModbusDataSource::
                CQEventReqConnectReader::
                    CQEventReqConnectReader(const QString& _dataName, QObject* _reader) :
                                                            mDataName(_dataName),
                                                            mpReader(_reader)
{

}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::CQEventReqConnectReader::handle(LCQModbusDataSource* _base)
{
    _base->mDataMap.connectReader(mDataName, mpReader);
}
//============================================================================================CQEventReqDisconnectReader
LCQModbusDataSource::
    CQEventReqDisconnectReader::
        CQEventReqDisconnectReader(QObject* _reader) :  mpReader(_reader)
{
}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusDataSource::CQEventReqDisconnectReader::handle(LCQModbusDataSource* _base)
{
    _base->mDataMap.disconnectReader(mpReader);
}

//==============================================================================================CControllerRegistersBase
void LCQModbusDataSource::CControllerRegistersBase::CDataItem::notifyReaders()
{
    QLinkedList<QObject*>::iterator it = mReadersList.begin();
    QLinkedList<QObject*>::iterator it_end = mReadersList.end();
    for(;it != it_end; it++)
    {
        QCoreApplication::postEvent( *it, new CQEventDataIsRead(QByteArray( (char*)(mpData), mSize * 2), mStatus));
    }
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerRegistersBase::
        CControllerRegistersBase(const quint8& _devId,
                                    QSharedPointer<LCModbusMasterBase> _master) : mDevId(_devId),
                                                                                 mwpMaster(_master)
{
    mRegBuff = new quint16[MODBUS_MAX_READ_REGISTER_COUNT + 1];
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerRegistersBase::
        ~CControllerRegistersBase()
{
    delete [] mRegBuff;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerRegistersBase::addReadDataItem(CDataItem* _dataItem)
{
    if(mReadDataList.contains(_dataItem)) return;
    mReadDataList << _dataItem;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerRegistersBase::deleteReadDataItem(CDataItem* _dataItem)
{
    mReadDataList.removeOne(_dataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerRegistersBase::read(quint16 _addr, quint16 _size, QObject* _reader)
{
//    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();

    EDataStatus status = EDataStatus::DS_WRONG;

    if(!mwpMaster.isNull())
    {
        if(readRegs(mwpMaster.data(), _addr, _size, mRegBuff).status ==
                LCModbusMasterBase::SReply::EStatus::OK)
        {
            status = EDataStatus::DS_OK;
        }
    }

    QCoreApplication::postEvent( _reader,
                                new LCQRemoteDataSourceBase::CQEventDataIsRead(
                                    QByteArray( (char*)mRegBuff, _size * 2),
                                    status));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerRegistersBase::write(quint16 _addr,
                                                             quint16 _size,
                                                             const QByteArray& _data,
                                                             QObject* _writer)
{
    EDataStatus status = EDataStatus::DS_UNDEF;

    status = EDataStatus::DS_WRONG;
    if(_data.size() % 2 == 0)
    {
        quint16 length = _data.size() >> 1;
        if(length == _size)
        {
//            QSharedPointer<LCModbusMasterBase> sp = mwpMaster.lock();
            if(!mwpMaster.isNull())
            {
                if(writeRegs(mwpMaster.data(), _addr, length, ((quint16*)_data.constData())).status ==
                        LCModbusMasterBase::SReply::EStatus::OK)
                    status = EDataStatus::DS_OK;
            }
        }
    }
    QCoreApplication::postEvent(_writer, new CQEventDataIsWrite(status));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerRegistersBase::update()
{
//    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();
    if(mwpMaster.isNull()) return;
    {
        //TODO: Проработать опережающий ответ для всех слушателей при нескольких таймаутах.
        QLinkedList<CDataItem*>::iterator it = mReadDataList.begin();
        QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end();
        for(;it != it_end; it++)
        {
            (*it)->mStatus = EDataStatus::DS_WRONG;
            if(readRegs(mwpMaster.data(), (*it)->mAddr, (*it)->mSize, (*it)->mpData).status ==
                    LCModbusMasterBase::SReply::EStatus::OK)
            {
                (*it)->mStatus = EDataStatus::DS_OK;
            }
            (*it)->notifyReaders();
        }
    }
}

//===========================================================================================CControllerHoldingRegisters
LCQModbusDataSource::CControllerHoldingRegisters::
        CControllerHoldingRegisters(const quint8& _devId,
                                    QSharedPointer<LCModbusMasterBase> _master) :
                                                    CControllerRegistersBase(_devId, _master)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerHoldingRegisters::
        ~CControllerHoldingRegisters()
{
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
            LCQModbusDataSource::
                CControllerHoldingRegisters::
                    readRegs(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{

    return master->readHoldingRegisters(mDevId, _addr, _size, _regs);
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCQModbusDataSource::
            CControllerHoldingRegisters::
                writeRegs(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    if(_size == 1)
    {
        return master->writeSingleRegister( mDevId, _addr, _regs[0]);
    }
    return master->writeMultipleRegisters( mDevId, _addr, _size, _regs);
}

//=============================================================================================CControllerInputRegisters
LCQModbusDataSource::CControllerInputRegisters::
        CControllerInputRegisters(const quint8& _devId,
                                    QWeakPointer<LCModbusMasterBase> _master) :
                                                    CControllerRegistersBase(_devId, _master)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerInputRegisters::
        ~CControllerInputRegisters()
{
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
            LCQModbusDataSource::
                CControllerInputRegisters::
                    readRegs(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    return master->readInputRegisters(mDevId, _addr, _size, _regs);
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCQModbusDataSource::
            CControllerInputRegisters::
                writeRegs(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint16 _regs[])
{
    Q_UNUSED(master);
    Q_UNUSED(_addr);
    Q_UNUSED(_size);
    Q_UNUSED(_regs);
    return LCModbusMasterBase::SReply(LCModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}




















//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//==============================================================================================CControllerRegistersBase
void LCQModbusDataSource::CControllerBitsBase::CDataItem::notifyReaders()
{
    QLinkedList<QObject*>::iterator it = mReadersList.begin();
    QLinkedList<QObject*>::iterator it_end = mReadersList.end();
    for(;it != it_end; it++)
    {
        QCoreApplication::postEvent( *it, new CQEventDataIsRead(QByteArray( (char*)(mpData), mSize), mStatus));
    }
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerBitsBase::
        CControllerBitsBase(const quint8& _devId,
                                    QWeakPointer<LCModbusMasterBase> _master) :  mDevId(_devId),
                                                                                 mwpMaster(_master)
{
    mBitsBuff = new quint8[MODBUS_MAX_READ_COIL_COUNT + 1];
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerBitsBase::
        ~CControllerBitsBase()
{
    delete [] mBitsBuff;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerBitsBase::addReadDataItem(CDataItem* _dataItem)
{
    if(mReadDataList.contains(_dataItem)) return;
    mReadDataList << _dataItem;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerBitsBase::deleteReadDataItem(CDataItem* _dataItem)
{
    mReadDataList.removeOne(_dataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerBitsBase::read(quint16 _addr, quint16 _size, QObject* _reader)
{
    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();

    EDataStatus status = EDataStatus::DS_WRONG;

    if(!master.isNull())
    {
        if(readBits(master.data(), _addr, _size, mBitsBuff).status ==
                LCModbusMasterBase::SReply::EStatus::OK)
        {
            status = EDataStatus::DS_OK;
        }
    }

    QCoreApplication::postEvent( _reader,
                                new LCQRemoteDataSourceBase::CQEventDataIsRead(
                                    QByteArray( (char*)mBitsBuff, _size),
                                    status));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerBitsBase::write(quint16 _addr,
                                                             quint16 _size,
                                                             const QByteArray& _data,
                                                             QObject* _writer)
{
    EDataStatus status = EDataStatus::DS_UNDEF;

    status = EDataStatus::DS_WRONG;

    if(_data.size() == _size)
    {
        QSharedPointer<LCModbusMasterBase> sp = mwpMaster.lock();
        if(!sp.isNull())
        {
            if(writeBits(sp.data(), _addr, _size, ((quint8*)_data.constData())).status ==
                    LCModbusMasterBase::SReply::EStatus::OK)
                status = EDataStatus::DS_OK;
        }
    }
    QCoreApplication::postEvent(_writer, new CQEventDataIsWrite(status));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CControllerBitsBase::update()
{
    QSharedPointer<LCModbusMasterBase> master = mwpMaster.lock();
    if(master.isNull()) return;
    {
        //TODO: Проработать опережающий ответ для всех слушателей при нескольких таймаутах.
        QLinkedList<CDataItem*>::iterator it = mReadDataList.begin();
        QLinkedList<CDataItem*>::iterator it_end = mReadDataList.end();
        for(;it != it_end; it++)
        {
            (*it)->mStatus = EDataStatus::DS_WRONG;
            if(readBits(master.data(), (*it)->mAddr, (*it)->mSize, (*it)->mpData).status ==
                    LCModbusMasterBase::SReply::EStatus::OK)
            {
                (*it)->mStatus = EDataStatus::DS_OK;
            }
            (*it)->notifyReaders();
        }
    }
}

//=============================================================================================CControllerDiscreteInputs
LCQModbusDataSource::CControllerCoils::
        CControllerCoils(const quint8& _devId,
                                    QWeakPointer<LCModbusMasterBase> _master) :
                                                    CControllerBitsBase(_devId, _master)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerCoils::
        ~CControllerCoils()
{
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
            LCQModbusDataSource::
                CControllerCoils::
                    readBits(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

    return master->readCoils(mDevId, _addr, _size, _bits);
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCQModbusDataSource::
            CControllerCoils::
                writeBits(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
    if(_size == 1)
    {
        return master->writeSingleCoils( mDevId, _addr, _bits[0]);
    }
    return master->writeMultipleCoils( mDevId, _addr, _size, _bits);
}

//=============================================================================================CControllerDiscreteInputs
LCQModbusDataSource::CControllerDiscreteInputs::
        CControllerDiscreteInputs(const quint8& _devId,
                                    QWeakPointer<LCModbusMasterBase> _master) :
                                                    CControllerBitsBase(_devId, _master)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CControllerDiscreteInputs::
        ~CControllerDiscreteInputs()
{
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
            LCQModbusDataSource::
                CControllerDiscreteInputs::
                    readBits(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{

    return master->readDiscreteInputs(mDevId, _addr, _size, _bits);
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCQModbusDataSource::
            CControllerDiscreteInputs::
                writeBits(LCModbusMasterBase* master, quint16 _addr, quint16 _size, quint8 _bits[])
{
    Q_UNUSED(master);
    Q_UNUSED(_addr);
    Q_UNUSED(_size);
    Q_UNUSED(_bits);
    return LCModbusMasterBase::SReply(LCModbusMasterBase::SReply::EStatus::WRONG_REQ, 0);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



















//==================================================================================================CDataMapItemRegsBase
LCQModbusDataSource::
                CDataMapItemRegsBase::
                    CDataMapItemRegsBase(quint16 _addr,
                                            quint16 _size,
                                            CControllerRegistersBase& _controller): mDataItem(_addr, _size),
                                                                                    mController(_controller)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemRegsBase::
                    ~CDataMapItemRegsBase()
{
    mController.deleteReadDataItem(&mDataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemRegsBase::connectReader(QObject* _reader)
{
    if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
    mDataItem.mReadersList << _reader;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemRegsBase::disconnectReader(QObject* _reader)
{
    mDataItem.mReadersList.removeOne(_reader);
    if(mDataItem.mReadersList.isEmpty()) mController.deleteReadDataItem(&mDataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemRegsBase::read(QObject* _reader)
{
    mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//===============================================================================================CDataMapItemHoldingRegs
LCQModbusDataSource::
                CDataMapItemHoldingRegs::
                    CDataMapItemHoldingRegs(quint16 _addr,
                                            quint16 _size,
                                            CControllerHoldingRegisters& _controller):
                                                CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemHoldingRegs::
                    ~CDataMapItemHoldingRegs()
{
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemHoldingRegs::write(const QByteArray& _data, QObject* _writer)
{
    mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//=================================================================================================CDataMapItemInputRegs
LCQModbusDataSource::
                CDataMapItemInputRegs::
                    CDataMapItemInputRegs(  quint16 _addr,
                                            quint16 _size,
                                            CControllerInputRegisters& _controller):
                                            CDataMapItemRegsBase(_addr, _size, _controller)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemInputRegs::
                    ~CDataMapItemInputRegs()
{
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemInputRegs::write(const QByteArray& _data, QObject* _writer)
{
    Q_UNUSED(_data);
    Q_UNUSED(_writer);
    QCoreApplication::postEvent(_writer, new CQEventDataIsWrite(EDataStatus::DS_WRONG));
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//==================================================================================================CDataMapItemBitsBase
LCQModbusDataSource::
                CDataMapItemBitsBase::
                    CDataMapItemBitsBase(quint16 _addr,
                                            quint16 _size,
                                            CControllerBitsBase& _controller): mDataItem(_addr, _size),
                                                                               mController(_controller)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemBitsBase::
                    ~CDataMapItemBitsBase()
{
    mController.deleteReadDataItem(&mDataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemBitsBase::connectReader(QObject* _reader)
{
    if(mDataItem.mReadersList.isEmpty()) mController.addReadDataItem(&mDataItem);
    mDataItem.mReadersList << _reader;
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemBitsBase::disconnectReader(QObject* _reader)
{
    mDataItem.mReadersList.removeOne(_reader);
    if(mDataItem.mReadersList.isEmpty()) mController.deleteReadDataItem(&mDataItem);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemBitsBase::read(QObject* _reader)
{
    mController.read(mDataItem.mAddr, mDataItem.mSize, _reader);
}

//=====================================================================================================CDataMapItemCoils
LCQModbusDataSource::
                CDataMapItemCoils::
                    CDataMapItemCoils(quint16 _addr,
                                            quint16 _size,
                                            CControllerBitsBase& _controller):
                                                CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemCoils::
                    ~CDataMapItemCoils()
{
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemCoils::write(const QByteArray& _data, QObject* _writer)
{
    mController.write(mDataItem.mAddr, mDataItem.mSize, _data, _writer);
}

//=================================================================================================CDataMapItemInputRegs
LCQModbusDataSource::
                CDataMapItemDiscreteInputs::
                    CDataMapItemDiscreteInputs(  quint16 _addr,
                                            quint16 _size,
                                            CControllerBitsBase& _controller):
                                            CDataMapItemBitsBase(_addr, _size, _controller)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::
                CDataMapItemDiscreteInputs::
                    ~CDataMapItemDiscreteInputs()
{
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::
        CDataMapItemDiscreteInputs::write(const QByteArray& _data, QObject* _writer)
{
    Q_UNUSED(_data);
    Q_UNUSED(_writer);
    QCoreApplication::postEvent(_writer, new CQEventDataIsWrite(EDataStatus::DS_WRONG));
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




//==============================================================================================================CDataMap
LCQModbusDataSource::
        CDataMap::
            CDataMap(const quint8& _devId,
                     QSharedPointer<LCModbusMasterBase> _master) :  mControllerHoldingRegs(_devId, _master),
                                                                    mControllerInputRegs(_devId, _master),
                                                                    mControllerCoils(_devId, _master),
                                                                    mControllerDiscreteInputs(_devId, _master)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusDataSource::CDataMap::~CDataMap()
{
    MTItemIterator it = mMapItems.begin();
    for(;it != mMapItems.end(); it++)
    {
        delete *it;
    }
    mMapItems.clear();
    mMapReaders.clear();
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CDataMap::update()
{
    mControllerHoldingRegs.update();
    mControllerInputRegs.update();
    mControllerCoils.update();
    mControllerDiscreteInputs.update();
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CDataMap::addItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemHoldingRegs(_addr, _size, mControllerHoldingRegs));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CDataMap::addItemInputRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemInputRegs(_addr, _size, mControllerInputRegs));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CDataMap::addItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemDiscreteInputs(_addr, _size, mControllerDiscreteInputs));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::CDataMap::addItemCoils(const QString& _name, quint16 _addr, quint16 _size)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it != mMapItems.end())
    {
        delete it.value();
    }
    mMapItems.insert(_name, new CDataMapItemCoils(_addr, _size, mControllerCoils));
}

//----------------------------------------------------------------------------------------------------------------------
bool LCQModbusDataSource::CDataMap::write(const QString& _name, const QByteArray& _data, QObject* _reader)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->write(_data, _reader);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool LCQModbusDataSource::CDataMap::read(const QString& _name, QObject* _reader)
{
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->read(_reader);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool LCQModbusDataSource::CDataMap::connectReader(const QString& _name, QObject* _reader)
{
    if(mMapReaders.contains(_reader)) return false;
    MTItemIterator it = mMapItems.find(_name);
    if(it == mMapItems.end()) return false;
    (*it)->connectReader(_reader);
    mMapReaders.insert(_reader, (*it));
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool LCQModbusDataSource::CDataMap::disconnectReader(QObject* _reader)
{
    MTReadersIterator it = mMapReaders.find(_reader);
    if(it == mMapReaders.end()) return false;
    (*it)->disconnectReader(_reader);
    mMapReaders.erase(it);
    return true;
}
int __i = 0;
//===================================================================================================LCQModbusDataSource
LCQModbusDataSource::
                LCQModbusDataSource(quint8 _devId,
                                    QSharedPointer<LCModbusMasterBase> _modbusMaster,
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
        qDebug() << "update";
        mDataMap.update(); });
}

//----------------------------------------------------------------------------------------------~LCQModbusDataController
LCQModbusDataSource::~LCQModbusDataSource()
{
}

//---------------------------------------------------------------------------------------------------------doDeleteLater
static void doDeleteLater(LCQModbusDataSource* _obj)
{
    _obj->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------create
QSharedPointer<LCQModbusDataSource> LCQModbusDataSource::create(quint8 _devId,
                                                                QSharedPointer<LCModbusMasterBase> _modbusMaster,
                                                                QObject *_parent)
{
    return QSharedPointer<LCQModbusDataSource>(new LCQModbusDataSource(_devId, _modbusMaster, _parent), doDeleteLater);
}



//------------------------------------------------------------------------------------------------addDataItemHoldingRegs
void LCQModbusDataSource::addDataItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemHoldingRegs(_name, _addr, _size);
}


//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::addDataItemInputRegs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemInputRegs(_name, _addr, _size);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::addDataItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemDiscreteInputs(_name, _addr, _size);
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusDataSource::addDataItemCoils(const QString& _name, quint16 _addr, quint16 _size)
{
    mDataMap.addItemCoils(_name, _addr, _size);
}

//-----------------------------------------------------------------------------------------------------------------start
void LCQModbusDataSource::start(int _updateIntervalMs)
{
    QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

void LCQModbusDataSource::start(QSharedPointer<QThread> _thread, int _updateIntervalMs)
{
    if(!mspThread.isNull()) return;

    mspThread = _thread;
    moveToThread(mspThread.data());
    if(!mspThread.data()->isRunning()) mspThread.data()->start();

    _updateIntervalMs = (_updateIntervalMs > 50) ? (_updateIntervalMs) : (50);

    QCoreApplication::postEvent(this, new CQEventStart(_updateIntervalMs));
}

//------------------------------------------------------------------------------------------------------------------stop
void LCQModbusDataSource::stop()
{
    QCoreApplication::postEvent(this, new CQEventStop);
}

//-----------------------------------------------------------------------------------------------------------readConnect
void LCQModbusDataSource::connectReader(const QString& _dataName, QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqConnectReader(_dataName, _reader));
}

//--------------------------------------------------------------------------------------------------------readDisconnect
void LCQModbusDataSource::disconnectReader(QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqDisconnectReader(_reader));
}

//-----------------------------------------------------------------------------------------------------------------write
void LCQModbusDataSource::read(const QString& _dataName, QObject* _reader)
{
    QCoreApplication::postEvent(this, new CQEventReqRead(_dataName, _reader));
}

//-----------------------------------------------------------------------------------------------------------------write
void LCQModbusDataSource::write(const QString& _dataName, const QByteArray& _data, QObject* _writer)
{
    QCoreApplication::postEvent(this, new CQEventReqWrite(_dataName, _data, _writer));
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQModbusDataSource::customEvent(QEvent* _event)
{
    if(_event->type() == CQEventBase::msExtendedEventType)
    {
        reinterpret_cast<CQEventBase*>(_event)->handle(this);
    }
    qDebug()<<"LCQModbusDataSource::customEvent";
}

} /* namespace modbus */
