#ifndef LQMODBUSDATASOURCE_H
#define LQMODBUSDATASOURCE_H

#include <QThread>
#include <QList>
#include <QLinkedList>
#include <QMap>
#include <QTimerEvent>

#include "lqextendevent.h"
#include "lqmodbusmasterbase.h"
#include "LIRemoteDataSource.h"

namespace modbus
{

//===============================================================================================LCQModbusDataController
class LQModbusDataSource : public QObject, public LIRemoteDataSource
{
    Q_OBJECT

private:
    //-------------------------------------------------------------------------------------------------------CQEventBase
    class CQEventBase : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        CQEventBase();
        virtual void handle(LQModbusDataSource* _base) = 0;
    };

    //------------------------------------------------------------------------------------------------------CQEventStart
    class CQEventStart : public CQEventBase
    {
    private:
        int mUpdateInterval;
    public:
        CQEventStart(int _updateInterval);
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //-------------------------------------------------------------------------------------------------------CQEventStop
    class CQEventStop : public CQEventBase
    {
    public:
        CQEventStop();
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //----------------------------------------------------------------------------------------------------CQEventReqRead
    class CQEventReqRead : public CQEventBase
    {
    private:
        QString   mDataName;
        QObject*  mpReader;
    public:
        CQEventReqRead(const QString& _dataName, QObject* _reader);
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //---------------------------------------------------------------------------------------------------CQEventReqWrite
    class CQEventReqWrite : public CQEventBase
    {
    private:
        QString     mDataName;
        QByteArray  mData;
        QObject*    mpWriter;
    public:
        CQEventReqWrite(const QString& _dataName, const QByteArray& _data, QObject* _writer);
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //-------------------------------------------------------------------------------------------CQEventReqConnectReader
    class CQEventReqConnectReader : public CQEventBase
    {
    private:
        QString  mDataName;
        QObject* mpReader;
    public:
        CQEventReqConnectReader(const QString& _dataName, QObject* _reader);
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //----------------------------------------------------------------------------------------CQEventReqDisconnectReader
    class CQEventReqDisconnectReader : public CQEventBase
    {
    private:
        QObject* mpReader;
    public:
        CQEventReqDisconnectReader(QObject* _reader);
        virtual void handle(LQModbusDataSource* _base) override;
    };

    //------------------------------------------------------------------------------------------CControllerRegistersBase
    class CControllerRegistersBase
    {
    public:
        class CDataItem
        {
        public:
            quint16 mAddr;
            quint16 mSize;
            LERemoteDataStatus mStatus;
            quint16 *mpData;
            QLinkedList<QObject*> mReadersList;
        public:
            CDataItem() = delete;

            explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
                                                                mSize(_size),
                                                                mStatus(LERemoteDataStatus::DS_UNDEF)
            {
                mpData = new quint16[_size];
            }
            ~CDataItem(){delete [] mpData;}
            void notifyReaders();
        };
    protected:
        const quint8& mDevId;
    private:
        quint16* mRegBuff;
        QSharedPointer<LQModbusMasterBase> mwpMaster;
        QLinkedList<CDataItem*> mReadDataList;

    public:
        CControllerRegistersBase(const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
        ~CControllerRegistersBase();
        void addReadDataItem(CDataItem* _dataItem);
        void deleteReadDataItem(CDataItem* _dataItem);
        void read(quint16 _addr, quint16 _size, QObject* _reader);
        void write(quint16 _addr, quint16 _size, const QByteArray& _data, QObject* _writer);
        void update();
    protected:
        virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
        virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
    };

    //---------------------------------------------------------------------------------------CControllerHoldingRegisters
    class CControllerHoldingRegisters : public CControllerRegistersBase
    {
    public:

    public:
        CControllerHoldingRegisters(const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
        ~CControllerHoldingRegisters();
        virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
        virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
    };

    //-----------------------------------------------------------------------------------------CControllerInputRegisters
    class CControllerInputRegisters : public CControllerRegistersBase
    {
    public:

    public:
        CControllerInputRegisters(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
        ~CControllerInputRegisters();
        virtual LQModbusMasterBase::SReply readRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
        virtual LQModbusMasterBase::SReply writeRegs(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
    };










    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //-----------------------------------------------------------------------------------------------CControllerBitsBase
    class CControllerBitsBase
    {
    public:
        class CDataItem
        {
        public:
            quint16 mAddr;
            quint16 mSize;
            LERemoteDataStatus mStatus;
            quint8 *mpData;
            QLinkedList<QObject*> mReadersList;
        public:
            CDataItem() = delete;

            explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
                                                                mSize(_size),
                                                                mStatus(LERemoteDataStatus::DS_UNDEF)
            {
                mpData = new quint8[_size];
            }
            ~CDataItem(){delete [] mpData;}
            void notifyReaders();
        };
    protected:
        const quint8& mDevId;
    private:
        quint8* mBitsBuff;
        QWeakPointer<LQModbusMasterBase> mwpMaster;
        QLinkedList<CDataItem*> mReadDataList;

    public:
        CControllerBitsBase(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
        ~CControllerBitsBase();
        void addReadDataItem(CDataItem* _dataItem);
        void deleteReadDataItem(CDataItem* _dataItem);
        void read(quint16 _addr, quint16 _size, QObject* _reader);
        void write(quint16 _addr, quint16 _size, const QByteArray& _data, QObject* _writer);
        void update();
    protected:
        virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
        virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
    };


    //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
    class CControllerDiscreteInputs : public CControllerBitsBase
    {
    public:

    public:
        CControllerDiscreteInputs(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
        ~CControllerDiscreteInputs();
        virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
        virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    };

    //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
    class CControllerCoils : public CControllerBitsBase
    {
    public:

    public:
        CControllerCoils(const quint8& _devId, QWeakPointer<LQModbusMasterBase> _master);
        ~CControllerCoils();
        virtual LQModbusMasterBase::SReply readBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
        virtual LQModbusMasterBase::SReply writeBits(LQModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    };
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++







    //--------------------------------------------------------------------------------------------------CDataMapItemBase
    class CDataMapItemBase
    {
    public:
        CDataMapItemBase(){}
        virtual ~CDataMapItemBase(){}
        virtual void connectReader(QObject* reader) = 0;
        virtual void disconnectReader(QObject* reader) = 0;
        virtual void write(const QByteArray& _data, QObject* _reader) = 0;
        virtual void read(QObject* _reader) = 0;
    };

    //----------------------------------------------------------------------------------------------CDataMapItemRegsBase
    class CDataMapItemRegsBase : public CDataMapItemBase
    {
    protected:
        CControllerRegistersBase::CDataItem mDataItem;
        CControllerRegistersBase& mController;
    public:
        explicit CDataMapItemRegsBase(quint16 _addr, quint16 _size, CControllerRegistersBase& _controller);
        virtual ~CDataMapItemRegsBase();

        virtual void connectReader(QObject* reader) override;
        virtual void disconnectReader(QObject* reader) override;
        virtual void read(QObject* _reader) override;
    };

    //-------------------------------------------------------------------------------------------CDataMapItemHoldingRegs
    class CDataMapItemHoldingRegs : public CDataMapItemRegsBase
    {
    public:
        explicit CDataMapItemHoldingRegs(quint16 _addr, quint16 _size, CControllerHoldingRegisters& _controller);
        virtual ~CDataMapItemHoldingRegs();
        virtual void write(const QByteArray& _data, QObject* _writer) override;
    };

    //---------------------------------------------------------------------------------------------CDataMapItemInputRegs
    class CDataMapItemInputRegs : public CDataMapItemRegsBase
    {
    public:
        explicit CDataMapItemInputRegs(quint16 _addr, quint16 _size, CControllerInputRegisters& _controller);
        virtual ~CDataMapItemInputRegs();
        virtual void write(const QByteArray& _data, QObject* _writer) override;
    };





    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //----------------------------------------------------------------------------------------------CDataMapItemBitsBase
    class CDataMapItemBitsBase : public CDataMapItemBase
    {
    protected:
        CControllerBitsBase::CDataItem mDataItem;
        CControllerBitsBase& mController;
    public:
        explicit CDataMapItemBitsBase(quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
        virtual ~CDataMapItemBitsBase();

        virtual void connectReader(QObject* reader) override;
        virtual void disconnectReader(QObject* reader) override;
        virtual void read(QObject* _reader) override;
    };

    //-------------------------------------------------------------------------------------------------CDataMapItemCoils
    class CDataMapItemCoils : public CDataMapItemBitsBase
    {
    public:
        explicit CDataMapItemCoils(quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
        virtual ~CDataMapItemCoils();
        virtual void write(const QByteArray& _data, QObject* _writer) override;
    };

    //----------------------------------------------------------------------------------------CDataMapItemDiscreteInputs
    class CDataMapItemDiscreteInputs : public CDataMapItemBitsBase
    {
    public:
        explicit CDataMapItemDiscreteInputs(quint16 _addr, quint16 _size, CControllerBitsBase& _controller);
        virtual ~CDataMapItemDiscreteInputs();
        virtual void write(const QByteArray& _data, QObject* _writer) override;
    };

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //----------------------------------------------------------------------------------------------------------CDataMap
    class CDataMap
    {
    private:

        using MTItemIterator = QMap<QString, CDataMapItemBase*>::iterator;
        using MTReadersIterator = QMap<QObject*, CDataMapItemBase*>::iterator;

        CControllerHoldingRegisters mControllerHoldingRegs;
        CControllerInputRegisters   mControllerInputRegs;

        CControllerCoils mControllerCoils;
        CControllerDiscreteInputs mControllerDiscreteInputs;

        QMap<QString, CDataMapItemBase*> mMapItems;
        QMap<QObject*, CDataMapItemBase*> mMapReaders;

    public:
        CDataMap(const quint8& _devId, QSharedPointer<LQModbusMasterBase> _master);
        ~CDataMap();

        void update();

        void addItemHoldingRegs(const QString& _name, quint16 _addr, quint16 _size);
        void addItemInputRegs(const QString& _name, quint16 _addr, quint16 _size);
        void addItemDiscreteInputs(const QString& _name, quint16 _addr, quint16 _size);
        void addItemCoils(const QString& _name, quint16 _addr, quint16 _size);

        bool write(const QString& _name, const QByteArray& _data, QObject* _writer);
        bool read(const QString& _name, QObject* _reader);
        bool connectReader(const QString& _name, QObject* reader);
        bool disconnectReader(QObject* reader);
    };

    quint8 mDevId;
    QSharedPointer<LQModbusMasterBase> mModbusMaster;
    int mUpdateInterval;
    QTimer mTimer;

    CDataMap mDataMap;
    QSharedPointer<QThread> mspThread;
private:
    explicit LQModbusDataSource(quint8 _devId,
                                 QSharedPointer<LQModbusMasterBase> _modbusMaster,
                                 QObject *_parent = nullptr);
    LQModbusDataSource(const LQModbusDataSource&) = delete;
    LQModbusDataSource& operator=(const LQModbusDataSource&) = delete;
public:
    virtual ~LQModbusDataSource();
    static QSharedPointer<LQModbusDataSource> create(  quint8 _devId,
                                                        QSharedPointer<LQModbusMasterBase> _modbusMaster,
                                                        QObject *_parent = nullptr);
    void addDataItemHoldingRegs(    const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemInputRegs(      const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemDiscreteInputs( const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemCoils(          const QString& _name, quint16 _addr, quint16 _size);
    void start(int _updateIntervalMs = 500);
    void start(QSharedPointer<QThread> _thread, int _updateIntervalMs = 500);
    void stop();


private:
    void connectReader(const QString& _name, QObject* _reader);
    void disconnectReader(QObject* _reader);
    void read(const QString& _name, QObject* _reader);
    void write(const QString& _name, const QByteArray& _data, QObject* _writer = nullptr);
    virtual void customEvent(QEvent* _event) override;

public:
    //----------------------------------------------------------------------------------------------------------override
    virtual QSharedPointer<LIRemoteDataReader> createReader() override;
    virtual QSharedPointer<LIRemoteDataWriter> createWriter() override;

    friend class LQModbusDataReader;
    friend class LQModbusDataWriter;
};

}
#endif // LQMODBUSDATASOURCE_H
