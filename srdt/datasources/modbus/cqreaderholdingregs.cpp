//#include "lcqmodbusdatasource.h"
//#include <QCoreApplication>

//namespace modbus {

////===============================================================================================CHoldingRegsBlockReader
//LCQModbusDataSource::
//        CQReaderHoldingRegs::
//                CQReaderHoldingRegs(LCQModbusDataSource* _base) : mpBase(_base)
//{

//}

//LCQModbusDataSource::CQReaderHoldingRegs::~CQReaderHoldingRegs()
//{
//    mObserveBlockList.clear();
//}

////----------------------------------------------------------------------------------------------------------------update
//void LCQModbusDataSource::CQReaderHoldingRegs::updateData()
//{
//    QSharedPointer<LCModbusMasterBase> sp = mpBase->mModbusMaster.lock();
//    if(sp.isNull()) return;
//    {
//        MTIteratorObserve it = mObserveBlockList.begin();
//        for(;it != mObserveBlockList.end(); it++)
//        {
//            EDataStatus status;

//            if(sp->readHoldingRegisters( mpBase->mDevId,
//                                            (*it)->mAddr,(*it)->mLength, mRegBuff).status ==
//                                                LCModbusMasterBase::SReply::EStatus::OK)
//            {
//                status = EDataStatus::DS_OK;
//            }
//            else
//            {
//                status = EDataStatus::DS_WRONG;
//            }
//            (*it)->setRegsData(mRegBuff, status);
//        }
//    }

//    {
//        MTIteratorReadList it = mReadList.begin();
//        for(;it != mReadList.end(); it++)
//        {
//            EDataStatus status;
//            if(sp->readHoldingRegisters( mpBase->mDevId,
//                                            (*it).addr,(*it).length, mRegBuff).status ==
//                                                LCModbusMasterBase::SReply::EStatus::OK)
//            {
//                status = EDataStatus::DS_OK;
//            }
//            else
//            {
//                status = EDataStatus::DS_WRONG;
//            }

//            QCoreApplication::postEvent(
//                        (*it).reader,
//                        new LCQRemoteDataSourceBase::CQEventDataIsRead(
//                                QByteArray( (char*)mRegBuff, (*it).length * 2),
//                                EDataStatus::DS_OK));

//        }
//        mReadList.clear();
//    }
//}

////----------------------------------------------------------------------------------------------------------------notify
//void LCQModbusDataSource::CQReaderHoldingRegs::notifyObserveBlocks()
//{
//    MTIteratorObserve it = mObserveBlockList.begin();
//    for(;it != mObserveBlockList.end(); it++)
//    {
//        (*it)->notifyListeners();
//    }
//}

////--------------------------------------------------------------------------------------------------------------addBlock
//void LCQModbusDataSource::CQReaderHoldingRegs::addObserveBlock(CQHoldingRegsBlock* _regBlock)
//{
//    if(mObserveBlockList.contains(_regBlock)) return;
//    mObserveBlockList << _regBlock;
//}

////-----------------------------------------------------------------------------------------------------------removeBlock
//void LCQModbusDataSource::CQReaderHoldingRegs::removeObserveBlock(CQHoldingRegsBlock* _regBlock)
//{
//    mObserveBlockList.removeOne(_regBlock);
//}

////------------------------------------------------------------------------------------------------------------------read
//void LCQModbusDataSource::CQReaderHoldingRegs::read(TUint16 _addr, TUint16 _length, QObject* _reader)
//{
//    mReadList << SReadData{_addr, _length, _reader};
//}

////-----------------------------------------------------------------------------------------------------------------write
//void LCQModbusDataSource::
//        CQReaderHoldingRegs::
//            write(CQHoldingRegsBlock* _regsBlock, const QByteArray& _data, QObject* _writer)
//{
//    LCQRemoteDataSourceBase::EDataStatus status = LCQRemoteDataSourceBase::EDataStatus::DS_UNDEF;

//    status = LCQRemoteDataSourceBase::EDataStatus::DS_WRONG;
//    if(_data.size() % 2 == 0)
//    {
//        TUint16 length = _data.size() >> 1;
//        if(length == _regsBlock->mLength)
//        {
//            QSharedPointer<LCModbusMasterBase> sp = mpBase->mModbusMaster.lock();
//            if(!sp.isNull())
//            {
//                if(length == 1)
//                {
//                    if(sp->writeSingleRegister( mpBase->mDevId,
//                                                    _regsBlock->mAddr,
//                                                        *((TUint16*)_data.constData()) ).status ==
//                        LCModbusMasterBase::SReply::EStatus::OK)
//                    status = LCQRemoteDataSourceBase::EDataStatus::DS_OK;
//                }
//                else
//                {
//                    if(sp->writeMultipleRegisters( mpBase->mDevId,
//                                                    _regsBlock->mAddr, length,
//                                                        (TUint16*)_data.constData()).status ==
//                        LCModbusMasterBase::SReply::EStatus::OK)
//                    status = LCQRemoteDataSourceBase::EDataStatus::DS_OK;
//                }
//            }
//        }
//    }
//    QCoreApplication::postEvent(_writer, new LCQRemoteDataSourceBase::CQEventDataIsWrite(status, 0));
//}

//} //namespace modbus
