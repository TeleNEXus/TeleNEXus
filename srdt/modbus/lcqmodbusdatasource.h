#ifndef LCQMODBUSDATASOURCE_H
#define LCQMODBUSDATASOURCE_H

//#include "lcqremotedatasourcebase.h"
#include "lcqmodbusmastertcp.h"
#include "lqextendevent.h"

#include <QThread>
#include <QList>
#include <QLinkedList>
#include <QMap>
#include <QTimerEvent>

#include "LCRemoteDataSourceInterface.h"

namespace modbus
{

//===============================================================================================LCQModbusDataController
class LCQModbusDataSource : public QObject, public LCRemoteDataSourceInterface
{
    Q_OBJECT

private:
    //-------------------------------------------------------------------------------------------------------CQEventBase
    class CQEventBase : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        CQEventBase();
        virtual void handle(LCQModbusDataSource* _base) = 0;
    };

    //------------------------------------------------------------------------------------------------------CQEventStart
    class CQEventStart : public CQEventBase
    {
    private:
        int mUpdateInterval;
    public:
        CQEventStart(int _updateInterval);
        virtual void handle(LCQModbusDataSource* _base) override;
    };

    //-------------------------------------------------------------------------------------------------------CQEventStop
    class CQEventStop : public CQEventBase
    {
    public:
        CQEventStop();
        virtual void handle(LCQModbusDataSource* _base) override;
    };

    //----------------------------------------------------------------------------------------------------CQEventReqRead
    class CQEventReqRead : public CQEventBase
    {
    private:
        QString   mDataName;
        QObject*  mpReader;
    public:
        CQEventReqRead(const QString& _dataName, QObject* _reader);
        virtual void handle(LCQModbusDataSource* _base) override;
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
        virtual void handle(LCQModbusDataSource* _base) override;
    };

    //-------------------------------------------------------------------------------------------CQEventReqConnectReader
    class CQEventReqConnectReader : public CQEventBase
    {
    private:
        QString  mDataName;
        QObject* mpReader;
    public:
        CQEventReqConnectReader(const QString& _dataName, QObject* _reader);
        virtual void handle(LCQModbusDataSource* _base) override;
    };

    //----------------------------------------------------------------------------------------CQEventReqDisconnectReader
    class CQEventReqDisconnectReader : public CQEventBase
    {
    private:
        QObject* mpReader;
    public:
        CQEventReqDisconnectReader(QObject* _reader);
        virtual void handle(LCQModbusDataSource* _base) override;
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
            ERemoteDataStatus mStatus;
            quint16 *mpData;
            QLinkedList<QObject*> mReadersList;
        public:
            CDataItem() = delete;

            explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
                                                                mSize(_size),
                                                                mStatus(ERemoteDataStatus::DS_UNDEF)
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
        QSharedPointer<LCModbusMasterBase> mwpMaster;
        QLinkedList<CDataItem*> mReadDataList;

    public:
        CControllerRegistersBase(const quint8& _devId, QSharedPointer<LCModbusMasterBase> _master);
        ~CControllerRegistersBase();
        void addReadDataItem(CDataItem* _dataItem);
        void deleteReadDataItem(CDataItem* _dataItem);
        void read(quint16 _addr, quint16 _size, QObject* _reader);
        void write(quint16 _addr, quint16 _size, const QByteArray& _data, QObject* _writer);
        void update();
    protected:
        virtual LCModbusMasterBase::SReply readRegs(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
        virtual LCModbusMasterBase::SReply writeRegs(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) = 0;
    };

    //---------------------------------------------------------------------------------------CControllerHoldingRegisters
    class CControllerHoldingRegisters : public CControllerRegistersBase
    {
    public:

    public:
        CControllerHoldingRegisters(const quint8& _devId, QSharedPointer<LCModbusMasterBase> _master);
        ~CControllerHoldingRegisters();
        virtual LCModbusMasterBase::SReply readRegs(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
        virtual LCModbusMasterBase::SReply writeRegs(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
    };

    //-----------------------------------------------------------------------------------------CControllerInputRegisters
    class CControllerInputRegisters : public CControllerRegistersBase
    {
    public:

    public:
        CControllerInputRegisters(const quint8& _devId, QWeakPointer<LCModbusMasterBase> _master);
        ~CControllerInputRegisters();
        virtual LCModbusMasterBase::SReply readRegs(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint16 _regs[]) override;
        virtual LCModbusMasterBase::SReply writeRegs(LCModbusMasterBase* master,
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
            ERemoteDataStatus mStatus;
            quint8 *mpData;
            QLinkedList<QObject*> mReadersList;
        public:
            CDataItem() = delete;

            explicit CDataItem(quint16 _addr, quint16 _size) :  mAddr(_addr),
                                                                mSize(_size),
                                                                mStatus(ERemoteDataStatus::DS_UNDEF)
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
        QWeakPointer<LCModbusMasterBase> mwpMaster;
        QLinkedList<CDataItem*> mReadDataList;

    public:
        CControllerBitsBase(const quint8& _devId, QWeakPointer<LCModbusMasterBase> _master);
        ~CControllerBitsBase();
        void addReadDataItem(CDataItem* _dataItem);
        void deleteReadDataItem(CDataItem* _dataItem);
        void read(quint16 _addr, quint16 _size, QObject* _reader);
        void write(quint16 _addr, quint16 _size, const QByteArray& _data, QObject* _writer);
        void update();
    protected:
        virtual LCModbusMasterBase::SReply readBits(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
        virtual LCModbusMasterBase::SReply writeBits(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) = 0;
    };


    //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
    class CControllerDiscreteInputs : public CControllerBitsBase
    {
    public:

    public:
        CControllerDiscreteInputs(const quint8& _devId, QWeakPointer<LCModbusMasterBase> _master);
        ~CControllerDiscreteInputs();
        virtual LCModbusMasterBase::SReply readBits(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
        virtual LCModbusMasterBase::SReply writeBits(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
    };

    //-----------------------------------------------------------------------------------------CControllerDiscreteInputs
    class CControllerCoils : public CControllerBitsBase
    {
    public:

    public:
        CControllerCoils(const quint8& _devId, QWeakPointer<LCModbusMasterBase> _master);
        ~CControllerCoils();
        virtual LCModbusMasterBase::SReply readBits(LCModbusMasterBase* master,
                                                        quint16 _addr, quint16 _size, quint8 _bits[]) override;
        virtual LCModbusMasterBase::SReply writeBits(LCModbusMasterBase* master,
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
        CDataMap(const quint8& _devId, QSharedPointer<LCModbusMasterBase> _master);
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
    QSharedPointer<LCModbusMasterBase> mModbusMaster;
    int mUpdateInterval;
    QTimer mTimer;

    CDataMap mDataMap;
    QSharedPointer<QThread> mspThread;
private:
    explicit LCQModbusDataSource(quint8 _devId,
                                 QSharedPointer<LCModbusMasterBase> _modbusMaster,
                                 QObject *_parent = nullptr);
    LCQModbusDataSource(const LCQModbusDataSource&) = delete;
    LCQModbusDataSource& operator=(const LCQModbusDataSource&) = delete;
public:
    virtual ~LCQModbusDataSource();
    static QSharedPointer<LCQModbusDataSource> create(  quint8 _devId,
                                                        QSharedPointer<LCModbusMasterBase> _modbusMaster,
                                                        QObject *_parent = nullptr);
    void addDataItemHoldingRegs(    const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemInputRegs(      const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemDiscreteInputs( const QString& _name, quint16 _addr, quint16 _size);
    void addDataItemCoils(          const QString& _name, quint16 _addr, quint16 _size);
    void start(int _updateIntervalMs = 500);
    void start(QSharedPointer<QThread> _thread, int _updateIntervalMs = 500);
    void stop();

    virtual QSharedPointer<LCRemoteDataReaderInterface> createReader() override;
    virtual QSharedPointer<LCRemoteDataWriterInterface> createWriter() override;


    //----------------------------------------------------------------------------------------------------------override
private:

    //---------------------------------------------------------------------------------------------------CQEventDataRead
    class CQEventDataIsRead : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        QSharedPointer<QByteArray> mData;
        ERemoteDataStatus mStatus;
        explicit CQEventDataIsRead(const QByteArray& _data, ERemoteDataStatus _status);
        explicit CQEventDataIsRead(ERemoteDataStatus _status);
    };

    //--------------------------------------------------------------------------------------------------CQEventDataWrite
    class CQEventDataIsWrite : public QEvent
    {
        __LQ_EXTENDED_QEVENT_DECLARATION
    public:
        const ERemoteDataStatus mStatus;
        explicit CQEventDataIsWrite(ERemoteDataStatus _status);
    };



    void connectReader(const QString& _name, QObject* _reader);
    void disconnectReader(QObject* _reader);
    void read(const QString& _name, QObject* _reader);
    void write(const QString& _name, const QByteArray& _data, QObject* _writer = nullptr);
    virtual void customEvent(QEvent* _event) override;

    friend class LCQModbusDataReader;
};

}
#endif // LCQMODBUSDATASOURCE_H
