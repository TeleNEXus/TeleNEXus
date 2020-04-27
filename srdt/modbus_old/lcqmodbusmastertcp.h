#ifndef LCQMODBUSMASTERTCP_H
#define LCQMODBUSMASTERTCP_H

#include "lcmodbusmasterbase.h"
#include "lqextendevent.h"
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QTimer>
#include <QHostAddress>
#include <QTcpSocket>

namespace modbus
{

class LCQModbusMasterTcp : public QObject, public LCModbusMasterBase
{
    Q_OBJECT
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
        QHostAddress mHostAddr;
        TUint16 mHostPort;
    public:
        CQEventConnectRequest(const QHostAddress& _haddr, TUint16 _port);
        virtual ~CQEventConnectRequest();
    };

    //------------------------------------------------------------------------------------------------------CDataReqBase
    class CDataReqBase
    {
    public:
        struct SCommon
        {
            QTcpSocket* &socket;
            int         &reqTimeout;
            int         &respTimeout;
            TUint16     &transactId;

            TUint8      *byteBuff;
            TUint16     *regBuff;

            SCommon(    QTcpSocket* &_socket,

                        int         &_reqTimeout,
                        int         &_respTimeout,
                        TUint16     &_transactId);

            ~SCommon();

        };
        EOperationStatus    mStatus;
        TUint8              mModbusException;
    public:
        CDataReqBase();
        virtual ~CDataReqBase();
        virtual void handle(SCommon&) = 0;
    };

    //---------------------------------------------------------------------------------------CDataRequestReadHoldingRegs
    class CDataRequestReadHoldingRegs : public CDataReqBase
    {
    public:
        TUint8      mDevId;
        TUint16     mAddr;
        TUint16     mRegQuant;
        TUint16     *mpRegs;
        CDataRequestReadHoldingRegs(TUint8   _devId,
                                    TUint16  _addr,
                                    TUint16  _regQuant,
                                    TUint16* _regs);
        virtual void handle(SCommon& _com) override;
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
        virtual void handle(SCommon& _com) override;
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
        virtual void handle(SCommon& _com) override;
    };

    QHostAddress    mHostAddr;
    TUint16         mHostPort;
    QTcpSocket*     mpSocket;
    QTimer          mTimer;

    int mTimeConnectTimeoutMs;  // Интервал ожидания подключения.
    int mTimeConnectCtrlMs;     // Интервал контроля соединения.
    int mTimeRequestTimeoutMs;  // Интервал ожидания посылки запроса.
    int mTimeResponseTimeoutMs; // Интервал ожидания ответа от мастера.

    TUint16     mTransactionId;

    CDataReqBase::SCommon mDataReqCommon;

    QMutex          mMutex;
    QWaitCondition  mWaitCond;
    QMetaObject::Connection mTimerConnection;

private:
    explicit LCQModbusMasterTcp(QObject *parent = nullptr);
public:
    LCQModbusMasterTcp(const LCQModbusMasterTcp&) = delete;
    LCQModbusMasterTcp& operator=(const LCQModbusMasterTcp&) = delete;
    virtual ~LCQModbusMasterTcp();
    static QSharedPointer<LCQModbusMasterTcp> create(QObject *_parent = nullptr);
//--------------------------------------------------------------------------------------------------------------override
    virtual EOperationStatus readHoldingRegisters(TUint8 _devId,
                                                  TUint16 _addr, TUint16 _regQuant, TUint16 _regs[]) override;

    virtual EOperationStatus writeSingleRegister(TUint8 _devId, TUint16 _addr, TUint16 _reg) override;

    virtual EOperationStatus writeMultipleRegisters(TUint8 _devId,
                                                    TUint16 _addr, TUint16 _regQuant, const TUint16 _regs[]) override;

//-----------------------------------------------------------------------------------------------------------------local
public slots:
    void connectToHost(const QHostAddress& _haddr, TUint16 _port);
    void disconnectFromHost();
signals:
private:
    void connectionControl(bool _reconnect = false);
//--------------------------------------------------------------------------------------------------------------override
    virtual void customEvent(QEvent* _event) override;

    friend class CDataRequestReadHoldingRegs;
    friend class CDataRequestWriteSingleReg;
    friend class CDataRequestWriteMultipleRegs;

};

}
#endif // LCQMODBUSCLIENTTCP_H
