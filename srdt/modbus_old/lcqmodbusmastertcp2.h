#ifndef LCQMODBUSMASTERTCP2_H
#define LCQMODBUSMASTERTCP2_H

#include "lcmodbusmasterbase.h"
#include "lqextendevent.h"
#include <QObject>
#include <QModbusTcpClient>
#include <QMutex>
#include <QWaitCondition>
#include <QUrl>

#include <QTimer>

namespace modbus {

class LCQModbusMasterTcp2 : public QObject, public LCModbusMasterBase
{
private:
    class CDataReqBase;

    //------------------------------------------------------------------------------------------------CQEventDataRequest
    class CQEventDataRequest : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        CDataReqBase* mpReq;
        CQEventDataRequest(CDataReqBase* _req);
        virtual ~CQEventDataRequest();
    };

    //---------------------------------------------------------------------------------------------CQEventConnectRequest
    class CQEventConnectRequest : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        QString mHost;
        TUint16 mPort;
    public:
        CQEventConnectRequest(const QUrl& _url);
        virtual ~CQEventConnectRequest();
    };


    //------------------------------------------------------------------------------------------------------CDataReqBase
    class CDataReqBase
    {
    public:
        EOperationStatus    mStatus;
        TUint8              mModbusException;
    public:
        CDataReqBase();
        virtual ~CDataReqBase();
        virtual void handle(QModbusClient* client) = 0;
    };

    //---------------------------------------------------------------------------------------CDataRequestReadHoldingRegs
    class CDataRequestReadHoldingRegs : public CDataReqBase
    {
    public:
        TUint8              mDevId;
        TUint16             mStartAddr;
        TUint16             mRegQuant;
        TUint16*            mpRegs;
        TUint8              mModbusException;
        EOperationStatus    mStatus;
        QModbusDataUnit     mDataUnit;
        QWaitCondition*     mpWaitCond;
        CDataRequestReadHoldingRegs(TUint8 _devId,
                                    TUint16 _startAddr,
                                    TUint16 _regQuant,
                                    TUint16* _regs,
                                    QWaitCondition* _waitCond);
        virtual void handle(QModbusClient* client) override;
    };

    //----------------------------------------------------------------------------------------CDataRequestWriteSingleReg
    class CDataRequestWriteSingleReg : public CDataReqBase
    {
    public:
        TUint8  mDevId;
        TUint16 mAddr;
        TUint16 mReg;
    public:
        CDataRequestWriteSingleReg(TUint8   _devId,
                                   TUint16  _addr,
                                   TUint16  _reg);
        virtual void handle(QModbusClient* client) override;
    };

    //-------------------------------------------------------------------------------------CDataRequestWriteMultipleRegs
    class CDataRequestWriteMultipleRegs : public CDataReqBase
    {
    public:
        TUint8  mDevId;
        TUint16 mAddr;
        TUint16 mRegQuant;
        const TUint16 *mRegs;
    public:
        CDataRequestWriteMultipleRegs(TUint8            _devId,
                                      TUint16           _addr,
                                      TUint16           _regQuant,
                                      const TUint16*    _regs);
        virtual void handle(QModbusClient* client) override;
    };

//    struct SReqReadRegsData
//    {
//        TUint8              devId = 0;
//        TUint16             startAddr = 0;
//        TUint16             regQuant = 0;
//        TUint16*            pRegs = nullptr;
//        TUint8              mModbusException = 0;
//        EOperationStatus    status = EOperationStatus::UNDEF;
//        QModbusDataUnit     dataUnit;
//        SReqReadRegsData() {dataUnit.setRegisterType(QModbusDataUnit::RegisterType::HoldingRegisters);}
//    };

    QModbusTcpClient*   mpModbusClient;
    QModbusDataUnit     mDataUnit;
    QMutex              mMutex;
    QWaitCondition      mWaitCond;

//    QTimer              mTimer;
//    SReqReadRegsData    mCurrReqReadRegsData;

public:
    LCQModbusMasterTcp2(QObject *_parent = nullptr);
    virtual ~LCQModbusMasterTcp2();
//--------------------------------------------------------------------------------------------------------------override
    virtual EOperationStatus readHoldingRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) override;

    virtual EOperationStatus writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg) override;

    virtual EOperationStatus writeMultipleRegisters(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuan, const TUint16 _regs[]) override;
//-----------------------------------------------------------------------------------------------------------------local
    public slots:
        void connectToHost(const QUrl& _url);
        void disconnectFromHost();
private:
    void readyReadRegisters();
    void readyWrite();

    virtual void customEvent(QEvent *event) override;
};

}//namespace

#endif // LCQMODBUSMASTERTCP2_H
