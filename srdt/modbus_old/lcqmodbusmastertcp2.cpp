#include "lcqmodbusmastertcp2.h"
#include "lmodbusdefs.h"
#include <QDebug>
#include <QCoreApplication>
namespace modbus {

//====================================================================================================CQEventDataRequest
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusMasterTcp2::CQEventDataRequest);

LCQModbusMasterTcp2::
        CQEventDataRequest::
            CQEventDataRequest(CDataReqBase* _req) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED), mpReq(_req)
{
}

LCQModbusMasterTcp2::
        CQEventDataRequest::~CQEventDataRequest()
{
}

//=================================================================================================CQEventConnectRequest
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusMasterTcp2::CQEventConnectRequest);

LCQModbusMasterTcp2::
        CQEventConnectRequest::
            CQEventConnectRequest( const QUrl& _url) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED)
{
    mHost = _url.host();
    mPort = _url.port();
}

LCQModbusMasterTcp2::
        CQEventConnectRequest::~CQEventConnectRequest()
{
}

//==========================================================================================================CDataReqBase
LCQModbusMasterTcp2::CDataReqBase::CDataReqBase(): mStatus(EOperationStatus::UNREC_ERROR),
                                                        mModbusException(MBEC_NO_EXCEPTION)
{
}

LCQModbusMasterTcp2::CDataReqBase::~CDataReqBase()
{
}

//===========================================================================================CDataRequestReadHoldingRegs
LCQModbusMasterTcp2::
        CDataRequestReadHoldingRegs::
            CDataRequestReadHoldingRegs(TUint8 _devId,
                                        TUint16 _startAddr,
                                        TUint16 _regQuant,
                                        TUint16* _regs,
                                        QWaitCondition* _waitCond) : mDevId(_devId),
                                                          mStartAddr(_startAddr),
                                                          mRegQuant(_regQuant),
                                                          mpRegs(_regs),
                                                          mModbusException(MBEC_NO_EXCEPTION),
                                                          mStatus(EOperationStatus::UNDEF),
                                                          mpWaitCond(_waitCond)
{
    mDataUnit.setRegisterType(QModbusDataUnit::RegisterType::HoldingRegisters);
    mDataUnit.setStartAddress(_startAddr);
    mDataUnit.setValueCount(_regQuant);
}



//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusMasterTcp2::
        CDataRequestReadHoldingRegs::handle(QModbusClient* client)
{
        if((mRegQuant > MODBUS_MAX_READ_REGISTER_COUNT)||
                (mRegQuant == 0)||(mpRegs == nullptr))
        {
            mStatus = EOperationStatus::WRONG_REQ;
            mpWaitCond->wakeOne();
            return;
        }

        if(client == nullptr)
        {
            mStatus = EOperationStatus::NO_CONNECTION;
            mpWaitCond->wakeOne();
            return;
        }

        if(client->state() != QModbusDevice::ConnectedState)
        {
            mStatus = EOperationStatus::NO_CONNECTION;
            mpWaitCond->wakeOne();
            return;
        }
        QModbusReply *reply = client->sendReadRequest(mDataUnit, mDevId);

        if(reply)
        {
            if(!reply->isFinished())
            {
                QObject::connect(reply, &QModbusReply::finished,
                    [=](){
                    switch(reply->error())
                    {
                    case QModbusDevice::Error::NoError:
                        {
                            const QModbusDataUnit result = reply->result();
                              if( (result.valueCount() != mRegQuant) ||
                                    (result.startAddress() != mStartAddr) ||
                                    (result.registerType() != QModbusDataUnit::RegisterType::HoldingRegisters))
                            {
                                mStatus = EOperationStatus::WRONG_RESP;
                                break;
                            }
                            for(TUint16 i = 0; i < mRegQuant; i++)
                            {
                                mpRegs[i] = result.value(i);
                            }
                            mStatus = EOperationStatus::OK;
                        }
                        break;
                    case QModbusDevice::Error::ConnectionError:
                        mStatus = EOperationStatus::CONNECTION_ERROR;
                        break;
                    case QModbusDevice::Error::TimeoutError:
                        mStatus = EOperationStatus::RESP_TIMEOUT;
                        break;
                    case QModbusDevice::Error::ProtocolError:
                        mStatus = EOperationStatus::MODBUS_EXCEPTION;
                        mModbusException = static_cast<TUint8>(reply->rawResult().exceptionCode());
                        qDebug() << "CDataRequestReadHoldingRegs::handle exceptionCode" << reply->rawResult().exceptionCode();
                        break;
                    case QModbusDevice::Error::UnknownError:
                        mStatus = EOperationStatus::UNREC_ERROR;
                        break;
                    default:
//                    case QModbusDevice::Error::ReplyAbortedError:
//                    case QModbusDevice::Error::ReadError:
//                    case QModbusDevice::Error::WriteError:
//                    case QModbusDevice::Error::ConfigurationError:
                        mStatus = EOperationStatus::WRONG_RESP;
                        break;
                    }
                    qDebug() << "CDataRequestReadHoldingRegs::handle 1 ERROR" << reply->error();
                    reply->deleteLater();
                    mpWaitCond->wakeOne();
                });
                return;
            }
            qDebug() << "CDataRequestReadHoldingRegs::handle 2 ERROR" << reply->error();
            reply->deleteLater();
        }
        mStatus = EOperationStatus::UNREC_ERROR;
        mpWaitCond->wakeOne();
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp2::LCQModbusMasterTcp2(QObject *_parent) : QObject(_parent), mpModbusClient(nullptr)
{

}

LCQModbusMasterTcp2::~LCQModbusMasterTcp2()
{
//    qDebug() << "LCQModbusMasterTcp2::~LCQModbusMasterTcp2() 0";

//    if(mpModbusClient != nullptr)
//    {
//        qDebug() << "LCQModbusMasterTcp2::~LCQModbusMasterTcp2() 1";
//        mpModbusClient->deleteLater();
//    }
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp2::EOperationStatus
        LCQModbusMasterTcp2::readHoldingRegisters(TUint8 _devId, TUint16 _addr, TUint16 _regQuant, TUint16 _regs[])
{
    QMutexLocker locker(&mMutex);

    CDataRequestReadHoldingRegs req(_devId, _addr, _regQuant, _regs, &mWaitCond);
    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));

    mWaitCond.wait(&mMutex);
    return req.mStatus;

//    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||(_regQuant == 0)||(_regs == nullptr))
//    {
//        return EOperationStatus::WRONG_REQ;
//    }
//    QMutexLocker locker(&mMutex);
//    if(mpModbusClient == nullptr)
//        return EOperationStatus::NO_CONNECTION;
//    if(mpModbusClient->state() != QModbusDevice::ConnectedState)
//        return EOperationStatus::NO_CONNECTION;
//    mDataUnit.setRegisterType(QModbusDataUnit::RegisterType::HoldingRegisters);
//    mDataUnit.setStartAddress(_addr);
//    mDataUnit.setValueCount(_regQuant);
//    mCurrReqReadRegs.startAddr = _addr;
//    mCurrReqReadRegs.regQuant = _regQuant;
//    mCurrReqReadRegs.regs = _regs;

//    QModbusReply *reply = mpModbusClient->sendReadRequest(mDataUnit, _devId);

//    if(reply)
//    {
//        if(!reply->isFinished())
//        {
//            connect(reply, &QModbusReply::finished, this, &LCQModbusMasterTcp2::readyReadRegisters);
//        }
//        else
//        {
//            return EOperationStatus::UNREC_ERROR;
//        }
//    }
//    else
//    {
//        return EOperationStatus::UNREC_ERROR;
//    }


//    mWaitCond.wait(&mMutex);
//    return mCurrReqReadRegs.status;
//    return EOperationStatus::UNREC_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp2::EOperationStatus
        LCQModbusMasterTcp2::writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg)
{

}

//----------------------------------------------------------------------------------------------------------------------
LCQModbusMasterTcp2::EOperationStatus
        LCQModbusMasterTcp2::writeMultipleRegisters(TUint8 _devId, TUint16 _addr,
                                                        TUint16 _regQuan, const TUint16 _regs[])
{

}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp2::connectToHost(const QUrl& _url)
{
    QCoreApplication::postEvent(this, new CQEventConnectRequest(_url));
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp2::disconnectFromHost()
{
    //TODO: Add Code
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp2::readyReadRegisters()
{
//    QModbusReply * reply = qobject_cast<QModbusReply *>(sender());

//    switch(reply->error())
//    {
//    case QModbusDevice::Error::NoError:
//        {
//            const QModbusDataUnit result = reply->result();
//              if( (result.valueCount() != mCurrReqReadRegs.regQuant) ||
//                    (result.startAddress() != mCurrReqReadRegs.startAddr) ||
//                    (result.registerType() != QModbusDataUnit::RegisterType::InputRegisters))
//            {
//                mCurrReqReadRegs.status = EOperationStatus::WRONG_RESP;
//                break;
//            }
//            for(TUint16 i = 0; i < mCurrReqReadRegs.regQuant; i++)
//            {
//                mCurrReqReadRegs.regs[i] = result.value(i);
//            }
//            mCurrReqReadRegs.status = EOperationStatus::OK;
//        }
//        break;
//    case QModbusDevice::Error::TimeoutError:
//        mCurrReqReadRegs.status = EOperationStatus::RESP_TIMEOUT;
//        break;
//    case QModbusDevice::Error::ProtocolError:
//        mCurrReqReadRegs.status = EOperationStatus::MODBUS_EXCEPTION;
//        break;
//    default:
//        mCurrReqReadRegs.status = EOperationStatus::UNREC_ERROR;
//        break;
//    }
//    mWaitCond.wakeOne();
}

//----------------------------------------------------------------------------------------------------------------------
void LCQModbusMasterTcp2::readyWrite()
{

}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQModbusMasterTcp2::customEvent(QEvent* _event)
{
    if(_event->type() == CQEventDataRequest::msExtendedEventType)
    {
        reinterpret_cast<CQEventDataRequest*>(_event)->mpReq->handle(mpModbusClient);
//        mWaitCond.wakeOne();
    }
    else
    if(_event->type() == CQEventConnectRequest::msExtendedEventType)
    {
        auto e = static_cast<CQEventConnectRequest*>(_event);
        if(mpModbusClient != nullptr) mpModbusClient->deleteLater();

        mpModbusClient = new QModbusTcpClient(this);

        mpModbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, e->mPort);
        mpModbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, e->mHost);

        connect(mpModbusClient, &QModbusTcpClient::stateChanged,
                [=](){
            qDebug() << "LCQModbusMasterTcp2::LCQModbusMasterTcp2::mpModbusClient->state =" << mpModbusClient->state();
            if(mpModbusClient->state() == QModbusDevice::State::UnconnectedState) mpModbusClient->connectDevice();
        });

        if(mpModbusClient->connectDevice())
        {
            qDebug() << "LCQModbusMasterTcp2::connectToHost 7";
        }
        else
        {
            qDebug() << "LCQModbusMasterTcp2::connectToHost 8";
        }
    }
}

}//namespace
