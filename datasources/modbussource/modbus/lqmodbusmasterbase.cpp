#include "lqmodbusmasterbase.h"
#include "lqextendevent.h"
#include "lmodbusdefs.h"
#include <QDebug>
#include <QCoreApplication>
#include <QBitArray>
#include <QThread>

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
        using MTMaster  = LQModbusMasterBase;
        QWaitCondition* mpWaitCond;
        QModbusReply *mpModbusReply;
    public:
        LQModbusMasterBase::SReply  mReply;

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
        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) = 0;


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
                                static_cast<quint8>(mpModbusReply->rawResult().exceptionCode());
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

        quint8                      mDevId;
        quint16                     mStartAddr;
        quint16                     mRegQuant;
        quint16*                    mpRegs;

        QModbusDataUnit::RegisterType mRegType;


        explicit CDataRequestReadRegs(   quint8 _devId,
                                quint16 _startAddr,
                                quint16 _regQuant,
                                quint16* _regs,
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

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mRegQuant) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != mRegType))
            {
                return  MTMaster::SReply::EStatus::WRONG_RESP;
            }

            for(quint16 i = 0; i < mRegQuant; i++)
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
        quint8                      mDevId;
        quint16                     mAddr;
        quint16                     mReg;

//        QModbusRequest mReq;

        explicit CDataRequestWriteSingleReg(quint8   _devId,
                                            quint16  _addr,
                                            quint16  _reg,
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

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
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
        quint8                      mDevId;
        quint16                     mStartAddr;
        quint16                     mRegQuant;
        QModbusDataUnit             mDataUnit;

        explicit CDataRequestWriteMultipleRegs(  quint8 _devId,
                                        quint16 _startAddr,
                                        quint16 _regQuant,
                                        const quint16* _regs,
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

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
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

        quint8                      mDevId;
        quint16                     mStartAddr;
        quint16                     mBitsQuant;
        quint8*                     mpBitsArray;
        QModbusDataUnit::RegisterType mRegType;

        explicit CDataRequestReadBits(  quint8          _devId,
                                        quint16         _startAddr,
                                        quint16         _bitsQuant,
                                        quint8*         _bitsArray,
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

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
        {
            const QModbusDataUnit result = _reply->result();
            if( (result.valueCount() != mBitsQuant) ||
                    (result.startAddress() != mStartAddr) ||
                    (result.registerType() != mRegType))
            {
                return MTMaster::SReply::EStatus::WRONG_RESP;
            }

            for(quint16 i = 0; i < mBitsQuant; i++)
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
        quint8          mDevId;
        quint16         mAddr;
        quint8          mBit;

        explicit CDataRequestWriteSingleBit(quint8   _devId,
                                   quint16  _addr,
                                   quint8   _bit,
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
            quint16 data = (mBit == 0) ? (0) : (0xff00);

            return _client->sendRawRequest(QModbusRequest(QModbusRequest::FunctionCode::WriteSingleCoil,
                                                          mAddr,
                                                          data),
                                           mDevId);
        }

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
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
        quint8                      mDevId;
        quint16                     mStartAddr;
        quint16                     mSize;
        LQModbusMasterBase::SReply  mReply;
        QModbusDataUnit             mDataUnit;

        CDataRequestWriteMultipleBits(  quint8 _devId,
                                        quint16 _startAddr,
                                        quint16 _size,
                                        const quint8* _bits,
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

        virtual LQModbusMasterBase::SReply::EStatus  handleReply(QModbusReply* _reply) final
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
LQModbusMasterBase::LQModbusMasterBase(QObject* _parent) : QObject(_parent), mpTimer(nullptr)
{
}

//----------------------------------------------------------------------------------------------------------------------
LQModbusMasterBase::~LQModbusMasterBase()
{
    if(mpTimer != nullptr) mpTimer->deleteLater();
}

//----------------------------------------------------------------------------------------------------------------------
LQModbusMasterBase::SReply LQModbusMasterBase::
    readInputRegisters(quint8 _devId,quint16 _addr, quint16 _regQuant, quint16 _regs[])
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
LQModbusMasterBase::SReply
        LQModbusMasterBase::readHoldingRegisters(quint8 _devId, quint16 _addr, quint16 _regQuant, quint16 _regs[])
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
LQModbusMasterBase::SReply
        LQModbusMasterBase::writeSingleRegister(quint8 _devId, quint16 _addr, quint16 _reg)
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
LQModbusMasterBase::SReply
        LQModbusMasterBase::writeMultipleRegisters(quint8 _devId, quint16 _addr,
                                                        quint16 _regQuant, const quint16 _regs[])
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
LQModbusMasterBase::SReply LQModbusMasterBase::readCoils(quint8 _devId, quint16 _startAddr,
                                                            quint16 _coilsQuant, quint8 _bits[])
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
LQModbusMasterBase::SReply
        LQModbusMasterBase::writeSingleCoils(quint8 _devId, quint16 _addr, quint8 _bit)
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
LQModbusMasterBase::SReply
        LQModbusMasterBase::writeMultipleCoils(quint8 _devId, quint16 _addr,
                                                        quint16 _size, const quint8 _bits[])
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
LQModbusMasterBase::SReply LQModbusMasterBase::readDiscreteInputs(quint8 _devId, quint16 _startAddr,
                                                            quint16 _inputsQuant, quint8 _bits[])
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
void LQModbusMasterBase::connectRequest()
{
    QCoreApplication::postEvent(this, new CQEventConnectRequest);
}


//-----------------------------------------------------------------------------------------------------------customEvent
void LQModbusMasterBase::customEvent(QEvent* _event)
{
    //Внимание!!!
    //Необходимо для синхронизации с началом ожидания вызывающего потока.
    mMutexEvent.lock();
    mMutexEvent.unlock();

    if(_event->type() == CQEventDataRequest::msExtendedEventType)
    {

        static_cast<CQEventDataRequest*>(_event)->mpReq->handle(getMaster());
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
                        if(master->state() == QModbusDevice::State::UnconnectedState)
                        {
                            master = createMaster();
                            master->connectDevice();
                        }
                    }
                });
            mpTimer->start(1000);
        }
    }

}

