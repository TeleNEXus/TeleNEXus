#include "lcqmodbusmastertcp.h"
#include "lmodbusdefs.h"
#include <QMutexLocker>
#include <QCoreApplication>
#include <QDebug>
#include <QDataStream>


using namespace modbus;

//====================================================================================================CQEventDataRequest
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusMasterTcp::CQEventDataRequest);

LCQModbusMasterTcp::
        CQEventDataRequest::
            CQEventDataRequest(CDataReqBase* _req) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED), mpReq(_req)
{
}

LCQModbusMasterTcp::
        CQEventDataRequest::~CQEventDataRequest()
{
}

//=================================================================================================CQEventConnectRequest
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQModbusMasterTcp::CQEventConnectRequest);

LCQModbusMasterTcp::
        CQEventConnectRequest::
            CQEventConnectRequest(  const QHostAddress& _haddr,
                                    TUint16 _port) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                     mHostAddr(_haddr),
                                                     mHostPort(_port)
{
}

LCQModbusMasterTcp::
        CQEventConnectRequest::~CQEventConnectRequest()
{
}

//==========================================================================================================CDataReqBase
LCQModbusMasterTcp::CDataReqBase::CDataReqBase(): mStatus(EOperationStatus::UNREC_ERROR),
                                                        mModbusException(MBEC_NO_EXCEPTION)
{
}

LCQModbusMasterTcp::CDataReqBase::~CDataReqBase()
{
}

LCQModbusMasterTcp::CDataReqBase::SCommon::SCommon(    QTcpSocket* &_socket,
            int         &_reqTimeout,
            int         &_respTimeout,
            TUint16     &_transactId) :socket(_socket),
                                       reqTimeout(_reqTimeout),
                                       respTimeout(_respTimeout),
                                       transactId(_transactId),
                                       byteBuff(new TUint8[MODBUS_MAX_TCP_ADU_LENGTH]),
                                       regBuff(new TUint16[MODBUS_MAX_READ_REGISTER_COUNT])
{
}

LCQModbusMasterTcp::CDataReqBase::SCommon::~SCommon()
{
    delete[] byteBuff;
    delete[] regBuff;
}

//===========================================================================================CDataRequestReadHoldingRegs
LCQModbusMasterTcp::
        CDataRequestReadHoldingRegs::
            CDataRequestReadHoldingRegs( TUint8   _devId,
                                         TUint16  _addr,
                                         TUint16  _regQuant,
                                         TUint16* _regs) :  mDevId(_devId),
                                                            mAddr(_addr),
                                                            mRegQuant(_regQuant),
                                                            mpRegs(_regs){}


//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusMasterTcp::
        CDataRequestReadHoldingRegs::handle(SCommon& _com)
{
    if(_com.socket == nullptr)
    {
        this->mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    if(_com.socket->state() != QTcpSocket::SocketState::ConnectedState)
    {
        this->mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    QDataStream dataStream;
    dataStream.setDevice(_com.socket);

    TUint16 wr_length = LCModbus::masterWritePduReadHoldingRegisters(&_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE],
                                                                        mAddr,
                                                                        mRegQuant);
    _com.transactId++;
    wr_length += LCModbus::writeTcpMbap(_com.transactId, wr_length +1, mDevId, _com.byteBuff);

    dataStream.resetStatus();
    dataStream.writeRawData((char*)_com.byteBuff, wr_length);

    if(!_com.socket->waitForBytesWritten(_com.reqTimeout))
    {
        mStatus =  EOperationStatus::CONNECTION_ERROR;
        return;
    }

    if(!_com.socket->waitForReadyRead(_com.respTimeout))
    {
        mStatus = EOperationStatus::RESP_TIMEOUT;
        return;
    }

    TUint16 pdu_expect_length = 1/*fcode*/ + 1/*byte count*/ + 2*mRegQuant/*regs*/;
    TUint16 adu_expect_length = MODBUS_TCP_HEADER_MBAP_SIZE + pdu_expect_length;

    int read_length = dataStream.readRawData((char*)_com.byteBuff, adu_expect_length);

    if(read_length < MODBUS_MIN_TCP_ADU_LENGTH)
    {
        mStatus = EOperationStatus::WRONG_RESP;
        return;
    }

    LCModbus::SMasterPduStatus pdu_stat;

    if(!LCModbus::checkTcpMbap(_com.transactId, pdu_expect_length + 1, mDevId, _com.byteBuff))
    {
        pdu_stat = LCModbus::masterReadPduError(
                    &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], MBFC_READ_HOLDING_REGISTERS);
    }
    else
    {
        pdu_stat = LCModbus::masterReadPduReadHoldingRegisters(
                        &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], mpRegs, mRegQuant);
    }

    switch(pdu_stat.error)
    {
    case LCModbus::EMasterPduError::MASTER_PDU_NO_ERROR:
                                        mStatus = EOperationStatus::OK;
        break;

    case LCModbus::EMasterPduError::MASTER_PDU_WRONG_MESSAGE:
                                        mStatus = EOperationStatus::WRONG_RESP;
        break;
    case LCModbus::EMasterPduError::MASTER_PDU_MODBUS_EXCEPTION:
                                        mModbusException = pdu_stat.exceptionCode;
                                        mStatus = EOperationStatus::MODBUS_EXCEPTION;
        break;
    default:
        mStatus = EOperationStatus::UNREC_ERROR;
    }
}

//============================================================================================CDataRequestWriteSingleReg
LCQModbusMasterTcp::
        CDataRequestWriteSingleReg::
            CDataRequestWriteSingleReg( TUint8   _devId,
                                        TUint16  _addr,
                                        TUint16  _reg): mDevId(_devId),
                                                        mAddr(_addr),
                                                        mReg(_reg)
{
};
//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusMasterTcp::
        CDataRequestWriteSingleReg::handle(SCommon& _com)
{
    if(_com.socket == nullptr)
    {
        mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    if(_com.socket->state() != QTcpSocket::SocketState::ConnectedState)
    {
        mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    QDataStream dataStream;
    dataStream.setDevice(_com.socket);

    TUint16 wr_length = LCModbus::masterWritePduWriteSingleRegister(
                                &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], mAddr, mReg);
    _com.transactId++;
    wr_length += LCModbus::writeTcpMbap(_com.transactId, wr_length +1, mDevId, _com.byteBuff);

    dataStream.resetStatus();
    dataStream.writeRawData((char*)_com.byteBuff, wr_length);

    if(!_com.socket->waitForBytesWritten(_com.reqTimeout))
    {
        mStatus = EOperationStatus::CONNECTION_ERROR;
        return;
    }

    if(!_com.socket->waitForReadyRead(_com.respTimeout))
    {
        mStatus = EOperationStatus::RESP_TIMEOUT;
        return;
    }

    TUint16 pdu_expect_length = 1/*fcode*/ + 2/*addr*/ + 2/*regval*/;

    TUint16 adu_expect_length = MODBUS_TCP_HEADER_MBAP_SIZE + pdu_expect_length;

    int read_length = dataStream.readRawData((char*)_com.byteBuff, adu_expect_length);


    if(read_length < MODBUS_MIN_TCP_ADU_LENGTH)
    {
        mStatus = EOperationStatus::WRONG_RESP;
        return;
    }

    LCModbus::SMasterPduStatus pdu_stat;

    if(!LCModbus::checkTcpMbap(_com.transactId, pdu_expect_length + 1, mDevId, _com.byteBuff))
    {
        pdu_stat = LCModbus::masterReadPduError(
                    &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], MBFC_WRITE_SINGLE_REGISTER);
    }
    else
    {
        pdu_stat = LCModbus::masterReadPduWriteSingleRegister(
                    &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], mAddr, mReg);
    }

    switch(pdu_stat.error)
    {
    case LCModbus::EMasterPduError::MASTER_PDU_NO_ERROR:
        mStatus = EOperationStatus::OK;
        break;
    case LCModbus::EMasterPduError::MASTER_PDU_WRONG_MESSAGE:
        mStatus = EOperationStatus::WRONG_RESP;
        break;
    case LCModbus::EMasterPduError::MASTER_PDU_MODBUS_EXCEPTION:
        mStatus = EOperationStatus::MODBUS_EXCEPTION;
        mModbusException = pdu_stat.exceptionCode;
        break;
    default:
        mStatus = EOperationStatus::UNREC_ERROR;
        break;
    }
}

//=========================================================================================CDataRequestWriteMultipleRegs
LCQModbusMasterTcp::
        CDataRequestWriteMultipleRegs::
            CDataRequestWriteMultipleRegs(  TUint8    _devId,
                                            TUint16   _addr,
                                            TUint16   _regQuant,
                                            const TUint16* _regs) : mDevId(_devId),
                                                                    mAddr(_addr),
                                                                    mRegQuant(_regQuant),
                                                                    mRegs(_regs)
{
}

//----------------------------------------------------------------------------------------------------------------handle
void LCQModbusMasterTcp::CDataRequestWriteMultipleRegs::handle(SCommon& _com)
{
    if(_com.socket == nullptr)
    {
        mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    if(_com.socket->state() != QTcpSocket::SocketState::ConnectedState)
    {
        mStatus = EOperationStatus::NO_CONNECTION;
        return;
    }

    QDataStream dataStream;
    dataStream.setDevice(_com.socket);

    TUint16 wr_length = LCModbus::masterWritePduWriteMultipleRegisters(
                &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], mAddr, mRegQuant, mRegs);

    _com.transactId++;
    wr_length += LCModbus::writeTcpMbap(_com.transactId, wr_length +1, mDevId, _com.byteBuff);

    dataStream.resetStatus();
    dataStream.writeRawData((char*)_com.byteBuff, wr_length);

    if(!_com.socket->waitForBytesWritten(_com.reqTimeout))
    {
        mStatus = EOperationStatus::CONNECTION_ERROR;
        return;
    }

    if(!_com.socket->waitForReadyRead(_com.respTimeout))
    {
        mStatus = EOperationStatus::RESP_TIMEOUT;
        return;
    }

    TUint16 pdu_expect_length = 1/*fcode*/ + 2/*addr*/ + 2/*quant regs*/;

    TUint16 adu_expect_length = MODBUS_TCP_HEADER_MBAP_SIZE + pdu_expect_length;

    int read_length = dataStream.readRawData((char*)_com.byteBuff, adu_expect_length);


    if(read_length < MODBUS_MIN_TCP_ADU_LENGTH)
    {
        mStatus = EOperationStatus::WRONG_RESP;
        return;
    }

    LCModbus::SMasterPduStatus pdu_stat;

    if(!LCModbus::checkTcpMbap(_com.transactId, pdu_expect_length + 1, mDevId, _com.byteBuff))
    {
        pdu_stat = LCModbus::masterReadPduError(
                    &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], MBFC_WRITE_MULTIPLE_REGISTERS);
    }
    else
    {
        pdu_stat = LCModbus::masterReadPduWriteMultipleRegisters(
                    &_com.byteBuff[MODBUS_TCP_HEADER_MBAP_SIZE], mAddr, mRegQuant);
    }

    switch(pdu_stat.error)
    {
    case LCModbus::EMasterPduError::MASTER_PDU_NO_ERROR:
        mStatus = EOperationStatus::OK;
        break;
    case LCModbus::EMasterPduError::MASTER_PDU_WRONG_MESSAGE:
        mStatus = EOperationStatus::WRONG_RESP;
        break;
    case LCModbus::EMasterPduError::MASTER_PDU_MODBUS_EXCEPTION:
        mStatus = EOperationStatus::MODBUS_EXCEPTION;
        mModbusException = pdu_stat.exceptionCode;
        break;
    default:
        mStatus = EOperationStatus::UNREC_ERROR;
        break;
    }
}

//==============================================================================================LCQModbusMasterTcp
LCQModbusMasterTcp::LCQModbusMasterTcp(QObject *parent) :   QObject(parent),
                                                                        mHostPort(0),
                                                                        mpSocket(nullptr),
                                                                        mTimer(this),
                                                                        mTimeConnectTimeoutMs(5000),
                                                                        mTimeConnectCtrlMs(1000),
                                                                        mTimeRequestTimeoutMs(250),
                                                                        mTimeResponseTimeoutMs(500),
                                                                        mTransactionId(0),
                                                                        mDataReqCommon(mpSocket,
                                                                                       mTimeRequestTimeoutMs,
                                                                                       mTimeResponseTimeoutMs,
                                                                                       mTransactionId)
{
    connect(&mTimer, &QTimer::timeout, [=](){this->connectionControl();});
}

LCQModbusMasterTcp::~LCQModbusMasterTcp()
{
}

static void doDeleteLater(QObject* _obj)
{
    _obj->deleteLater();
}

QSharedPointer<LCQModbusMasterTcp> LCQModbusMasterTcp::create(QObject *_parent)
{
    return QSharedPointer<LCQModbusMasterTcp>(new LCQModbusMasterTcp(_parent), doDeleteLater);
}

//--------------------------------------------------------------------------------------------------readHoldingRegisters
LCQModbusMasterTcp::EOperationStatus
        LCQModbusMasterTcp::readHoldingRegisters( TUint8 _devId,
                                                            TUint16 _addr, TUint16 _regQuant, TUint16 _regs[])
{
    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||(_regQuant == 0)||(_regs == nullptr))
    {
        return EOperationStatus::WRONG_REQ;
    }
    CDataRequestReadHoldingRegs req(_devId, _addr, _regQuant, _regs);
    QMutexLocker locker(&mMutex);
    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutex);
    return req.mStatus;
}

//---------------------------------------------------------------------------------------------------writeSingleRegister
LCQModbusMasterTcp::EOperationStatus
        LCQModbusMasterTcp::writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg)
{
    CDataRequestWriteSingleReg req(_devId, _addr, _reg);
    QMutexLocker locker(&mMutex);
    QCoreApplication::postEvent(this, new CQEventDataRequest(&req));
    mWaitCond.wait(&mMutex);
    return req.mStatus;
}

//------------------------------------------------------------------------------------------------writeMultipleRegisters
LCQModbusMasterTcp::EOperationStatus
        LCQModbusMasterTcp::writeMultipleRegisters(TUint8 _devId,
                                                            TUint16 _addr, TUint16 _regQuant, const TUint16 _regs[])
{
    if((_regQuant > MODBUS_MAX_READ_REGISTER_COUNT)||(_regQuant == 0)||(_regs == nullptr))
    {
        return EOperationStatus::WRONG_REQ;
    }
    CDataRequestWriteMultipleRegs req(_devId, _addr, _regQuant, _regs);
    QMutexLocker locker(&mMutex);
    QCoreApplication::postEvent(this, new CQEventDataRequest( &req ));
    mWaitCond.wait(&mMutex);
    return req.mStatus;
}

//--------------------------------------------------------------------------------------------------readHoldingRegisters
void LCQModbusMasterTcp::connectToHost(const QHostAddress& _haddr, TUint16 _port)
{
    QCoreApplication::postEvent(this, new CQEventConnectRequest(_haddr, _port));
}

void LCQModbusMasterTcp::disconnectFromHost()
{
    QMutexLocker locker(&mMutex);
    mTimer.stop();
    if(mpSocket != nullptr)
    {
        mpSocket->disconnectFromHost();
        mpSocket->deleteLater();
    }
}

//-----------------------------------------------------------------------------------------------------connectionControl
void LCQModbusMasterTcp::connectionControl(bool _reconnect)
{
    if(mpSocket != nullptr)
    {
        if(((mpSocket->peerAddress() != mHostAddr)) || ((mpSocket->peerPort() != mHostPort)) || (_reconnect))
        {
            mpSocket->disconnectFromHost();
            mpSocket->deleteLater();
        }
        else if(mpSocket->state() != QTcpSocket::SocketState::UnconnectedState) return;
    }

    mpSocket = new QTcpSocket(this);

    connect(mpSocket, &QTcpSocket::stateChanged,
        [](QAbstractSocket::SocketState socketState){
            qDebug() << "---------------------------------------------Socket state changed: " << socketState;
        });

    qDebug() << "---------------------------------------------Start connecting to Host";

    mpSocket->connectToHost(mHostAddr, mHostPort);
    if(!mpSocket->waitForConnected(mTimeConnectTimeoutMs))
    {
        mpSocket->abort();
        mpSocket->deleteLater();
        mpSocket = nullptr;
        return;
    }
}

//-----------------------------------------------------------------------------------------------------------customEvent
void LCQModbusMasterTcp::customEvent(QEvent* _event)
{
    if(_event->type() == CQEventDataRequest::msExtendedEventType)
    {
        reinterpret_cast<CQEventDataRequest*>(_event)->mpReq->handle(mDataReqCommon);
        mWaitCond.wakeOne();
    }
    else if(_event->type() == CQEventConnectRequest::msExtendedEventType)
    {
        mTimer.stop();
        mHostAddr = reinterpret_cast<CQEventConnectRequest*>(_event)->mHostAddr;
        mHostPort = reinterpret_cast<CQEventConnectRequest*>(_event)->mHostPort;
        connectionControl(true);
        mTimer.start(1000);
    }
}
