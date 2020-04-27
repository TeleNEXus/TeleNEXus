#include "lcmodbusmasterbase.h"
#include "lqextendevent.h"
#include "lmodbusdefs.h"
#include <QDebug>
#include <QCoreApplication>
#include <QBitArray>
#include <QThread>
namespace modbus {

    //---------------------------------------------------------------------------------------------CQEventConnectRequest
    class CQEventConnectRequest : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
            CQEventConnectRequest():QEvent(__LQ_EXTENDED_QEVENT_REGISTERED){};
        virtual ~CQEventConnectRequest(){};
    };
    __LQ_EXTENDED_QEVENT_IMPLEMENTATION(CQEventConnectRequest);

    //------------------------------------------------------------------------------------------------CQEventDataRequest
    class CDataReqBase;
    class CQEventDataRequest : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        CDataReqBase* mpReq;
        explicit CQEventDataRequest(CDataReqBase* _req): QEvent(__LQ_EXTENDED_QEVENT_REGISTERED), mpReq(_req){}
        virtual ~CQEventDataRequest(){};
    };
    __LQ_EXTENDED_QEVENT_IMPLEMENTATION(CQEventDataRequest);

    //------------------------------------------------------------------------------------------------------CDataReqBase
    class CDataReqBase
    {
    protected:
        using MTMaster  = LCModbusMasterBase;
        QWaitCondition* mpWaitCond;
        QModbusReply *mpModbusReply;
    public:
        LCModbusMasterBase::SReply  mReply;

    private:
        CDataReqBase(){};
    public:
        explicit CDataReqBase(QWaitCondition* _waitCond) : mpWaitCond(_waitCond), mpModbusReply(nullptr){}
        virtual ~CDataReqBase(){};
        MTMaster::SReply::EStatus errorTranslate(QModbusDevice::Error _mb_error)
        {
            switch(_mb_error)
            {
            case QModbusDevice::Error::NoError:
                return MTMaster::SReply::EStatus::OK;
            case QModbusDevice::Error::ConnectionError:
                return MTMaster::SReply::EStatus::CONNECTION_ERROR;
            case QModbusDevice::Error::TimeoutError:
                return MTMaster::SReply::EStatus::RESP_TIMEOUT;
            case QModbusDevice::Error::ProtocolError:
                return MTMaster::SReply::EStatus::MODBUS_EXCEPTION;
                break;
            case QModbusDevice::Error::UnknownError:
                return MTMaster::SReply::EStatus::UNREC_ERROR;
                break;
            default:
                //case QModbusDevice::Error::ReplyAbortedError:
                //case QModbusDevice::Error::ReadError:
                //case QModbusDevice::Error::WriteError:
                //case QModbusDevice::Error::ConfigurationError:
                return MTMaster::SReply::EStatus::WRONG_RESP;
                break;
            }
        }

        virtual void handle(QModbusClient* _client) final;

    protected:
        virtual QModbusReply* sendRequest(QModbusClient* _client) = 0;
        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) = 0;


    };

    void CDataReqBase::handle(QModbusClient* _client)
    {
        if(_client == nullptr)
        {
            mReply.status = MTMaster::SReply::EStatus::NO_CONNECTION;
            mpWaitCond->wakeOne();
            return;
        }

        if(_client->state() != QModbusDevice::ConnectedState)
        {
            mReply.status = MTMaster::SReply::EStatus::NO_CONNECTION;
            mpWaitCond->wakeOne();
            return;
        }

        mpModbusReply = sendRequest(_client);

        if(mpModbusReply)
        {
            if(!mpModbusReply->isFinished())
            {
                QObject::connect(mpModbusReply, &QModbusReply::finished,
                    [=](){

                        if(mpModbusReply->error() == QModbusDevice::Error::NoError)
                        {
                            mReply.status = handleReply(mpModbusReply);
                        }
                        else
                        {
                            mReply.status = errorTranslate(mpModbusReply->error());
                        }
                        mReply.modbusExceptionCode =
                                static_cast<TUint8>(mpModbusReply->rawResult().exceptionCode());
                        mpModbusReply->deleteLater();
                        mpWaitCond->wakeOne();
                    });
                return;
            }
            mpModbusReply->deleteLater();
        }
        mReply.status = MTMaster::SReply::EStatus::UNREC_ERROR;
        mpWaitCond->wakeOne();
    }

    //---------------------------------------------------------------------------------------CDataRequestReadHoldingRegs
    class CDataRequestReadRegs : public CDataReqBase
    {
    public:
        enum EMode
        {
            INPUT,
            HOLDING
        };

        TUint8                      mDevId;
        TUint16                     mStartAddr;
        TUint16                     mRegQuant;
        TUint16*                    mpRegs;

        QModbusDataUnit::RegisterType mRegType;


        explicit CDataRequestReadRegs(   TUint8 _devId,
                                TUint16 _startAddr,
                                TUint16 _regQuant,
                                TUint16* _regs,
                                EMode    _mode,
                                QWaitCondition* _waitCond) :    CDataReqBase(_waitCond),
                                                                mDevId(_devId),
                                                                mStartAddr(_startAddr),
                                                                mRegQuant(_regQuant),
                                                                mpRegs(_regs)
        {
            mRegType = (_mode == HOLDING) ?
                        (QModbusDataUnit::RegisterType::HoldingRegisters) :
                        (QModbusDataUnit::RegisterType::InputRegisters);
        }

        virtual ~CDataRequestReadRegs(){}

        QModbusReply* sendRequest(QModbusClient* _client) final
        {
            return _client->sendReadRequest(QModbusDataUnit( mRegType, mStartAddr, mRegQuant), mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mRegQuant) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != mRegType))
            {
                return  MTMaster::SReply::EStatus::WRONG_RESP;
            }

            for(TUint16 i = 0; i < mRegQuant; i++)
            {
                mpRegs[i] = result.value(i);
            }
            return  MTMaster::SReply::EStatus::OK;
        }

    };

    //----------------------------------------------------------------------------------------CDataRequestWriteSingleReg
    class CDataRequestWriteSingleReg : public CDataReqBase
    {
    public:
        TUint8                      mDevId;
        TUint16                     mAddr;
        TUint16                     mReg;

//        QModbusRequest mReq;

        explicit CDataRequestWriteSingleReg(TUint8   _devId,
                                            TUint16  _addr,
                                            TUint16  _reg,
                                            QWaitCondition* _waitCond) : CDataReqBase(_waitCond),
            mDevId(_devId),
            mAddr(_addr),
            mReg(_reg)
        {
        }

        virtual ~CDataRequestWriteSingleReg(){}

        virtual QModbusReply* sendRequest(QModbusClient* _client) final
        {
            return _client->sendRawRequest(QModbusRequest(QModbusRequest::FunctionCode::WriteSingleRegister,
                                                     mAddr, mReg), mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
                if(_reply->rawResult().functionCode() != QModbusResponse::FunctionCode::WriteSingleRegister)
                {
                    return  MTMaster::SReply::EStatus::WRONG_RESP;
                }
                return MTMaster::SReply::EStatus::OK;
        }
    };

    //-------------------------------------------------------------------------------------CDataRequestWriteMultipleRegs
    class CDataRequestWriteMultipleRegs : public CDataReqBase
    {
    public:
        TUint8                      mDevId;
        TUint16                     mStartAddr;
        TUint16                     mRegQuant;
        QModbusDataUnit             mDataUnit;

        explicit CDataRequestWriteMultipleRegs(  TUint8 _devId,
                                        TUint16 _startAddr,
                                        TUint16 _regQuant,
                                        const TUint16* _regs,
                                        QWaitCondition* _waitCond) : CDataReqBase(_waitCond),
                mDevId(_devId),
                mStartAddr(_startAddr),
                mRegQuant(_regQuant),
                mDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, _startAddr, _regQuant)
        {
            for(int i = 0; i < _regQuant; i++)
            {
                mDataUnit.setValue(i, _regs[i]);
            }
        }

        virtual ~CDataRequestWriteMultipleRegs(){}

        virtual QModbusReply* sendRequest(QModbusClient* _client) final
        {
            return _client->sendWriteRequest(mDataUnit, mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mRegQuant) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != QModbusDataUnit::RegisterType::HoldingRegisters))
            {
                return MTMaster::SReply::EStatus::WRONG_RESP;
            }
            return MTMaster::SReply::EStatus::OK;
        }
    };

    //-------------------------------------------------------------------------------------CDataRequestWriteMultipleRegs
    class CDataRequestReadBits : public CDataReqBase
    {
    public:
        enum class EMode
        {
            COILS,
            INPUTS
        };

        TUint8                      mDevId;
        TUint16                     mStartAddr;
        TUint16                     mBitsQuant;
        TUint8*                     mpBitsArray;
        QModbusDataUnit::RegisterType mRegType;

        explicit CDataRequestReadBits(  TUint8          _devId,
                                        TUint16         _startAddr,
                                        TUint16         _bitsQuant,
                                        TUint8*         _bitsArray,
                                        EMode           _mode,
                                        QWaitCondition* _waitCond) : CDataReqBase(_waitCond),
                mDevId(_devId),
                mStartAddr(_startAddr),
                mBitsQuant(_bitsQuant),
                mpBitsArray(_bitsArray)
        {
            mRegType = (_mode == EMode::COILS) ? (QModbusDataUnit::RegisterType::Coils):
                                                            (QModbusDataUnit::RegisterType::DiscreteInputs);
        }

        virtual ~CDataRequestReadBits(){}

        virtual QModbusReply* sendRequest(QModbusClient* _client) final
        {
            return _client->sendReadRequest(QModbusDataUnit(mRegType, mStartAddr, mBitsQuant), mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mBitsQuant) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != mRegType))
            {
                return MTMaster::SReply::EStatus::WRONG_RESP;
            }

            for(TUint16 i = 0; i < mBitsQuant; i++)
            {
                mpBitsArray[i] = result.value(i);
            }
            return MTMaster::SReply::EStatus::OK;
        }
    };

    //----------------------------------------------------------------------------------------CDataRequestWriteSingleBit
    class CDataRequestWriteSingleBit : public CDataReqBase
    {
    public:
        TUint8          mDevId;
        TUint16         mAddr;
        TUint8          mBit;

        explicit CDataRequestWriteSingleBit(TUint8   _devId,
                                   TUint16  _addr,
                                   TUint8   _bit,
                                   QWaitCondition* _waitCond) : CDataReqBase(_waitCond),
            mDevId(_devId),
            mAddr(_addr),
            mBit(_bit)
        {
        }

        ~CDataRequestWriteSingleBit()
        {
        }

        virtual QModbusReply* sendRequest(QModbusClient* _client) final
        {
            TUint16 data = (mBit == 0) ? (0) : (0xff00);

            return _client->sendRawRequest(QModbusRequest(QModbusRequest::FunctionCode::WriteSingleCoil,
                                                          mAddr,
                                                          data),
                                           mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            if(_reply->rawResult().functionCode() != QModbusResponse::FunctionCode::WriteSingleCoil)
            {
                return MTMaster::SReply::EStatus::WRONG_RESP;
            }
            else
            {
                return MTMaster::SReply::EStatus::OK;
            }
        }
    };

    //-------------------------------------------------------------------------------------CDataRequestWriteMultipleBits
    class CDataRequestWriteMultipleBits : public CDataReqBase
    {
    public:
        TUint8                      mDevId;
        TUint16                     mStartAddr;
        TUint16                     mSize;
        LCModbusMasterBase::SReply  mReply;
        QModbusDataUnit             mDataUnit;

        CDataRequestWriteMultipleBits(  TUint8 _devId,
                                        TUint16 _startAddr,
                                        TUint16 _size,
                                        const TUint8* _bits,
                                        QWaitCondition* _waitCond) : CDataReqBase(_waitCond),
                mDevId(_devId),
                mStartAddr(_startAddr),
                mSize(_size),
                mDataUnit(QModbusDataUnit::RegisterType::Coils, _startAddr, _size)
        {
            for(int i = 0; i < _size; i++)
            {
                mDataUnit.setValue(i, _bits[i]);
            }
        }

        ~CDataRequestWriteMultipleBits()
        {

        }

        virtual QModbusReply* sendRequest(QModbusClient* _client) final
        {
            return _client->sendWriteRequest(mDataUnit, mDevId);
        }

        virtual LCModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mSize) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != QModbusDataUnit::RegisterType::Coils))
            {
                return MTMaster::SReply::EStatus::WRONG_RESP;
            }
            return MTMaster::SReply::EStatus::OK;
        }
    };

//====================================================================================================LCModbusMasterBase
LCModbusMasterBase::LCModbusMasterBase(QObject* _parent) : QObject(_parent), mpTimer(nullptr)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::~LCModbusMasterBase()
{
    if(mpTimer != nullptr) mpTimer->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply LCModbusMasterBase::
    readInputRegisters(TUint8 _devId,TUint16 _addr, TUint16 _regQuant, TUint16 _regs[])
{
    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||
            (_regQuant == 0)||(_regs == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestReadRegs req(_devId, _addr, _regQuant, _regs, CDataRequestReadRegs::INPUT, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCModbusMasterBase::readHoldingRegisters(TUint8 _devId, TUint16 _addr, TUint16 _regQuant, TUint16 _regs[])
{
    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||
            (_regQuant == 0)||(_regs == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestReadRegs req(_devId, _addr, _regQuant, _regs, CDataRequestReadRegs::HOLDING, &mWaitCond);
    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCModbusMasterBase::writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg)
{

    CDataRequestWriteSingleReg req(_devId, _addr, _reg, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCModbusMasterBase::writeMultipleRegisters(TUint8 _devId, TUint16 _addr,
                                                        TUint16 _regQuant, const TUint16 _regs[])
{
    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||
            (_regQuant == 0)||(_regs == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestWriteMultipleRegs req(_devId, _addr, _regQuant, _regs, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();

    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply LCModbusMasterBase::readCoils(TUint8 _devId, TUint16 _startAddr,
                                                            TUint16 _coilsQuant, TUint8 _bits[])
{
    if((_coilsQuant > MODBUS_MAX_READ_COIL_COUNT)||
            (_coilsQuant == 0) || (_bits == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestReadBits req(_devId, _startAddr, _coilsQuant, _bits,
                                    CDataRequestReadBits::EMode::COILS, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();

    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCModbusMasterBase::writeSingleCoils(TUint8 _devId, TUint16 _addr, TUint8 _bit)
{
    CDataRequestWriteSingleBit req(_devId, _addr,
                                   _bit,
                                   &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply
        LCModbusMasterBase::writeMultipleCoils(TUint8 _devId, TUint16 _addr,
                                                        TUint16 _size, const TUint8 _bits[])
{
    if((_size > MODBUS_MAX_READ_COIL_COUNT)||
            (_size == 0)||(_bits == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestWriteMultipleBits req(_devId, _addr, _size, _bits, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
LCModbusMasterBase::SReply LCModbusMasterBase::readDiscreteInputs(TUint8 _devId, TUint16 _startAddr,
                                                            TUint16 _inputsQuant, TUint8 _bits[])
{
    if((_inputsQuant > MODBUS_MAX_READ_COIL_COUNT)||
            (_inputsQuant == 0) || (_bits == nullptr))
    {
        return SReply(SReply::EStatus::WRONG_REQ, 0);
    }

    CDataRequestReadBits req(_devId, _startAddr, _inputsQuant, _bits,
                                    CDataRequestReadBits::EMode::INPUTS, &mWaitCond);

    mMutexThread.lock();
    mMutexEvent.lock();

    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutexEvent);

    mMutexEvent.unlock();
    mMutexThread.unlock();
    return req.mReply;
}

//----------------------------------------------------------------------------------------------------------------------
void LCModbusMasterBase::connectRequest()
{
    QCoreApplication::postEvent(this, new CQEventConnectRequest);
}


//-----------------------------------------------------------------------------------------------------------customEvent
void LCModbusMasterBase::customEvent(QEvent* _event)
{
    //Внимание!!!
    //Необходимо для синхронизации с началом ожидания вызывающего потока.
    mMutexEvent.lock();
    mMutexEvent.unlock();

    if(_event->type() == CQEventDataRequest::msExtendedEventType)
    {

        reinterpret_cast<CQEventDataRequest*>(_event)->mpReq->handle(getMaster());
    }
    else
    if(_event->type() == CQEventConnectRequest::msExtendedEventType)
    {
        QModbusClient* master = createMaster();
        master->connectDevice();
        if(!mpTimer)
        {
            mpTimer = new QTimer(this);

            mpTimer->moveToThread(QThread::currentThread());

            connect(mpTimer, &QTimer::timeout,
                [=]{
                    QModbusClient* master = getMaster();
                    if(master != nullptr)
                    {
                        qDebug() << "ModbusClient state =" << master->state();
                        if(master->state() == QModbusDevice::State::UnconnectedState)
                        {
                            master = createMaster();
                            master->connectDevice();
                        }
                    }
                    else
                    {
                        qDebug() << "ModbusClient state =" << QModbusDevice::State::UnconnectedState;
                    }
                });
            mpTimer->start(1000);
        }
    }

}

}//namespace
