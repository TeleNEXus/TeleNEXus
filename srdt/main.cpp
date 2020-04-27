#include <QCoreApplication>
#include <signal.h>
#include <iostream>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QHostAddress>

#include "cqsocketclient1.h"
#include "cqclienttcpecho.h"
#include "cqtestimer.h"
#include "cqmythread.h"
#include "cqclientecho2.h"

#include "cqclientechoblocking1.h"

//#include "lmodbusregsarray.h"
#include "lcqmodbusmastertcp.h"
#include "lcqmodbusmasterrtu.h"
//#include "lcqmodbusmastersimulator.h"
#include "lcqmodbusdatasource.h"
#include <QMap>
#include <QEvent>
#include <QMutex>
#include <atomic>
#include <QByteArray>
#include <QList>
#include <QBitArray>


#include <QIntValidator>
#include <QDoubleValidator>



//using namespace std;

QThread* __gStopTh1 = nullptr;

CQClientEchoProcess* __gCep1;
CQClientEchoProcess* __gCep2;


#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTabWidget>
#include <QTabBar>
#include <QToolBox>

#include <functional>
#include <limits>
#include "lcqremotedatareader.h"


#include "lcqremlabel.h"
#include "lcqremlineedit.h"

#include "lcdatastrformatbool.h"

#include "lcdatastrformatu8.h"
#include "lcdatastrformats8.h"

#include "lcdatastrformatu16.h"
#include "lcdatastrformats16.h"

#include "lcdatastrformatu32.h"
#include "lcdatastrformats32.h"

#include "lcdatastrformatf32.h"
#include "lcqu32validator.h"

#include "lcdatastrformathex.h"

#include "lcqmodbusmastertcp.h"

//using namespace remgui;

void exitApp(int _sig)
{
    switch(_sig)
    {
    case SIGINT:
        std::cout << "User exit request" << std::endl;
        break;
    case SIGTERM:
        std::cout << "System terminate request" << std::endl;
        break;
    }
    qDebug() << "Start termination application...";

    qDebug() << "Exit application" << "SIG = " << _sig;
    QCoreApplication::quit();
}



void calcDelay(quint64 pass)
{
    float __f1 = 0.0f;
    for(quint64 i = 0; i < pass;i++)
    {
        __f1 =( float)i/10.1;
        __f1 *= 10.1;
        __f1 = ( float)i/11.1;
        __f1 *= 11.1;
        __f1 = ( float)i/12.1;
        __f1 *= 12.1;
        __f1 = ( float)i/13.1;
        __f1 *= 13.1;
    }
}
class C1
{
public:
    C1(){qDebug() << "-------------------------------C1->Constructor";}
    ~C1(){qDebug() << "-------------------------------C1->Destructor";}
    virtual void dosm(){qDebug() << "C1->dosm()";}
};

class C2 : public C1
{
public:
    C2(){qDebug() << "-------------------------------C2->Constructor";}
    ~C2(){qDebug() << "-------------------------------C2->Destructor";}
    virtual void dosm(){qDebug() << "C2->dosm()";}
};

class C3
{
public:
    C3(){qDebug() << "-------------------------------C3->Constructor";}
    ~C3(){qDebug() << "-------------------------------C3->Destructor";}
    virtual void dosm(){qDebug() << "C3->dosm()";}
};

int testMasterTcp1(int argc, char *argv[]);
int testGuiTab(int argc, char *argv[]);
int testGui1Simulator(int argc, char *argv[]);
int testModbusMasterSimulator(int argc, char *argv[]);
int testGui1(int argc, char *argv[]);

int testXml1();
int testXmlDom1(int argc, char *argv[]);

#include <memory>

#include "lcxmlapplication.h"

#include <QDomElement>
#include <QDomDocument>
#include "lcxmlwidgetcreatorsmap.h"
#include "xmlwidgets/lcxmlwidget.h"
#include "xmlwidgets/lcxmllabel.h"

#include <QLibrary>

#include "testdllinterface.h"

class CTestDllRecurse : public LCTestDllInterface
{
public:
    CTestDllRecurse(){}
    virtual ~CTestDllRecurse(){}
    virtual QString getString() override {return "Test Dll String";}
};

int main(int argc, char *argv[])
{

//    std::shared_ptr<C2> sp_c1(new C2);
//    std::shared_ptr<C1> sp_c2 = sp_c1;

//    qDebug() << "sp_c1.clear()";
//    sp_c1.reset();
//    qDebug() << "sp_c2.clear()";
//    sp_c2.reset();
//    qDebug() << "Exit Programm";
//    return 0;
//    return testGui1(argc, argv);
//    return testXml1();

//    return testXmlDom1(argc, argv);

    QLibrary lib("D:/Dokuments/Kuzmenko/Programs/PROGRAM_PROJECTS/srdtrep/build-extwidglib-Desktop-Debug/debug/extwidglib");
    typedef const QString (*TFct)(LCTestDllInterface*);
    qDebug()<< "pass0";
    TFct fct = (TFct)(lib.resolve("getExportString"));
    qDebug()<< "pass1";

    CTestDllRecurse tci;
    if(!fct)
    {
        qDebug()<< "No recognize function";
    }
    else
    {
        qDebug()<< fct(&tci);
    }

//    if(lib.resolve("Extwidglib"))
//    {
//        qDebug()<< "resolve yes";
//    }
//    else
//    {
//        qDebug()<< "resolve no";
//    }


//    return 0;

    return LCXmlApplication::instance().exec(argc, argv);

//    remgui::LCDataStrFormatHex formatter;

//    QByteArray arr;
//    arr[0]  = 0;
//    arr[1]  = 1;
//    arr[2]  = 2;
//    arr[3]  = 3;
//    arr[4]  = 4;
//    arr[5]  = 5;
//    arr[6]  = 6;
//    arr[7]  = 7;
//    arr[8]  = 8;
//    arr[9]  = 9;
//    arr[10] = 10;
//    arr[11] = 11;
//    arr[12] = 12;
//    arr[13] = 13;
//    arr[14] = 14;
//    arr[15] = 15;
//    arr[16] = 16;
//    arr[17] = 17;
//    arr[18] = 18;
//    arr[19] = 19;
//    arr[20] = (char)255;


//    qDebug() << "toString\t" << formatter.toString(arr);

//    QString str = "00h01h02h03h04h05h06h07h01h01h01h01h01h01h01h";
//    qDebug() << "toBytes\t" << formatter.toString(formatter.toBytes(formatter.toString(arr)));
//    return 0;

}



int testXmlDom1(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QFile file("D:\\Dokuments\\Kuzmenko\\Programs\\PROGRAM_PROJECTS\\qttest\\qtcls1\\qtcls1\\test1.xml");
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument domDoc;

    LCXmlWidgetCreatorsMap::instace().addCreator("widget", new xmlwidgetcreators::LCXmlWidget);
    LCXmlWidgetCreatorsMap::instace().addCreator("label", new xmlwidgetcreators::LCXmlLabel);

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Parse error at line:" << errorLine << " column:" << errorColumn << " msg: " << errorStr;
    }
    else
    {
        QDomElement rootElement = domDoc.documentElement();
        LCXmlWidgetCreatorBase* cr = LCXmlWidgetCreatorsMap::instace().getCreator(rootElement.tagName());
        if(cr)
        {
            QWidget* w = cr->create(rootElement);
            if(w)
            {
                w->show();
            }
        }
    }

    return app.exec();
}

#include "lqtestsaxhandler.h"

int testXml1()
{
    LQTestSaxHandler saxHendler;
    QFile file("D:\\Dokuments\\Kuzmenko\\Programs\\PROGRAM_PROJECTS\\qttest\\qtcls1\\qtcls1\\test.xml");
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(&saxHendler);
    reader.parse(source);
    return 0;
}

static void qobject_deleter(QObject* _obj)
{
    _obj->deleteLater();
}

//--------------------------------------------------------------------------------------------------------testMasterTcp1
int testMasterTcp1(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "-----------------------------------------START TEST0";

    QTimer t1;
    int pass_counter = 0;

    QThread *th2 = new QThread;
    QThread *th3 = new QThread;

    QSharedPointer<modbus::LCQModbusMasterTcp> master = modbus::LCQModbusMasterTcp::create();
    master->connectToHost(QUrl::fromUserInput( QStringLiteral("192.168.0.2:502") ));

//    modbus::LCQModbusMasterRtu *master = new modbus::LCQModbusMasterRtu;
//    master->moveToThread(th1);
//    master->connectToPort();


    QObject::connect(th2, &QThread::started,
        [&](){
            using namespace modbus;
            unsigned int pass_counter = 0;
            QString status;
            quint16 regs[128];
            forever
            {
//                const LCQModbusMasterTcp::SReply reply = master->readHoldingRegisters(7, 0, 1, regs);
//                const LCQModbusMasterTcp::SReply reply = master->writeSingleRegister(7, 202, pass_counter);
                regs[0] = pass_counter;
                regs[1] = pass_counter;
                regs[2] = pass_counter;
                regs[3] = pass_counter;
                regs[4] = pass_counter;
                regs[5] = pass_counter;
                regs[6] = pass_counter;

//                const LCQModbusMasterTcp::SReply reply = master->writeMultipleRegisters(7, 200, 8, regs);
                const LCQModbusMasterTcp::SReply reply = master->writeSingleRegister(7, 200, pass_counter);

                switch(reply.status)
                {
                case modbus::LCModbusMasterBase::SReply::EStatus::OK:
                    status = "OK";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::UNDEF:
                    status = "UNDEF";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::WRONG_REQ:
                    status = "WRONG_REQ";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::WRONG_RESP:
                    status = "WRONG_RESP";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::UNREC_ERROR:
                    status = "UNREC_ERROR";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::RESP_TIMEOUT:
                    status = "RESP_TIMEOUT";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::NO_CONNECTION:
                    status = "NO_CONNECTION";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::MODBUS_EXCEPTION:
                    status = "MODBUS_EXCEPTION";
                    qDebug() << "th2 pass MODBUS_EXCEPTION code" << reply.modbusExceptionCode;
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::CONNECTION_ERROR:
                    status = "CONNECTION_ERROR";
                    break;
                default:
                    status = "???????";
                    break;
                }

                qDebug() << "th2 pass " << pass_counter << "\t read status =" << status << " reg =" << regs[0];
                pass_counter++;
                QThread::msleep(1000);
            }
    });

    QObject::connect(th3, &QThread::started,
        [&](){
            using namespace modbus;
            unsigned int pass_counter = 0;
            QString status;
            quint16 regs[128];
            quint8  bitArray[127];
            forever
            {
//                const LCQModbusMasterTcp::SReply reply = master->readHoldingRegisters(7, 1, 1, regs);
//                const LCQModbusMasterTcp::SReply reply = master->readInputRegisters(7, 200, 1, regs);
                const LCQModbusMasterTcp::SReply reply = master->readDiscreteInputs(7,0,10,bitArray);
                switch(reply.status)
                {
                case modbus::LCModbusMasterBase::SReply::EStatus::OK:
                    status = "OK";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::UNDEF:
                    status = "UNDEF";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::WRONG_REQ:
                    status = "WRONG_REQ";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::WRONG_RESP:
                    status = "WRONG_RESP";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::UNREC_ERROR:
                    status = "UNREC_ERROR";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::RESP_TIMEOUT:
                    status = "RESP_TIMEOUT";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::NO_CONNECTION:
                    status = "NO_CONNECTION";
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::MODBUS_EXCEPTION:
                    status = "MODBUS_EXCEPTION";
                    qDebug() << "th3 pass MODBUS_EXCEPTION code" << reply.modbusExceptionCode;
                    break;
                case modbus::LCModbusMasterBase::SReply::EStatus::CONNECTION_ERROR:
                    status = "CONNECTION_ERROR";
                    break;
                default:
                    status = "???????";
                    break;
                }

//                qDebug() << "th3 pass " << pass_counter << "\t read status =" << status << " reg =" << regs[0];
                qDebug() << "th3 pass " << pass_counter << "\t read status =" << status << " bits ="
                         << bitArray[0] << bitArray[1] << bitArray[2] << bitArray[3];
                pass_counter++;
                QThread::msleep(1000);
            }
    });

    th2->start();
    th3->start();

    QObject::connect(&t1, &QTimer::timeout,
        [&](){
          qDebug() << "Timer1 pass counter =" << pass_counter;
//          calcDelay(180000000);

          pass_counter++;
    });

    t1.start(1000);

    return app.exec();
}

//---------------------------------------------------------------------------------------------testModbusMasterSimulator
int testGuiTab(int argc, char *argv[])
{
    using namespace remgui;
    QApplication app(argc, argv);

    QTimer t1;
    int pass_counter = 0;

    QThread *source_thread = new QThread;

    QSharedPointer<modbus::LCQModbusMasterTcp> sp_mbmaster = modbus::LCQModbusMasterTcp::create();
    sp_mbmaster->connectToHost(QUrl::fromUserInput( QStringLiteral("192.168.0.2:502") ));

//    QSharedPointer<modbus::LCQModbusMasterRtu> sp_mbmaster(new modbus::LCQModbusMasterRtu);
//    sp_mbmaster->moveToThread(master_thread);
//    master_thread->start();
//    sp_mbmaster->connectToPort("COM3",
//                               QSerialPort::BaudRate::Baud115200,
//                               QSerialPort::Parity::OddParity,
//                               QSerialPort::DataBits::Data8,
//                               QSerialPort::StopBits::OneStop,
//                               250);

//    QSharedPointer<modbus::LCQModbusMasterTcp> sp_mbmaster = modbus::LCQModbusMasterTcp::create();
//    sp_mbmaster->moveToThread(master_thread);
//    master_thread->start();
//    sp_mbmaster->connectToHost(QHostAddress("192.168.0.2"), 502);

    QSharedPointer<modbus::LCQModbusDataSource> sp_datasource = modbus::LCQModbusDataSource::create(7, sp_mbmaster);

    sp_datasource->moveToThread(source_thread);
    source_thread->start();

    QObject::connect(&t1, &QTimer::timeout,
            [&](){
                qDebug() << "QTimer::timeout pass" << pass_counter;
                sp_mbmaster->writeSingleRegister(7, 200, pass_counter);
                pass_counter++;
            });
    t1.start(1000);

    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){

            qDebug() << "QApplication::aboutToQuit";
    });


    sp_datasource.data()->addDataItemHoldingRegs("counter", 200,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Uint16",  201,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Int16",   202,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Uint32",  204,  2);
    sp_datasource.data()->addDataItemHoldingRegs("Int32",   206,  2);
    sp_datasource.data()->addDataItemHoldingRegs("float32", 208,  2);

    sp_datasource.data()->addDataItemHoldingRegs("RW0",    200,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW1",    201,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW2",    202,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW3",    203,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW4",    204,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW5",    205,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW6",    206,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW7",    207,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW8",    208,  1);
    sp_datasource.data()->addDataItemHoldingRegs("RW9",    209,  1);


    QSharedPointer<LCDataStrFormatBase> formatterUint8(    new LCDataStrFormatU8());
    QSharedPointer<LCDataStrFormatBase> formatterInt8(     new LCDataStrFormatS8());
    QSharedPointer<LCDataStrFormatBase> formatterUint16(   new LCDataStrFormatU16());
    QSharedPointer<LCDataStrFormatBase> formatterInt16(    new LCDataStrFormatS16());
    QSharedPointer<LCDataStrFormatBase> formatterUint32(   new LCDataStrFormatU32());
    QSharedPointer<LCDataStrFormatBase> formatterInt32(    new LCDataStrFormatS32());
    QSharedPointer<LCDataStrFormatBase> formatterFloat32(  new LCDataStrFormatF32());

    LCQRemLabel *labelRemReg0 = new LCQRemLabel("RW0", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg1 = new LCQRemLabel("RW1", sp_datasource, formatterUint16);

    QPushButton *pushBatton1 = new QPushButton("Controller Start");
    QPushButton *pushBatton2 = new QPushButton("Controller Stop");
    QPushButton *pushBatton3 = new QPushButton("Activate true");
    QPushButton *pushBatton4 = new QPushButton("Activate false");

    QObject::connect(pushBatton1, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button: \"Controller Start\"";
            sp_datasource->start();
        });

    QObject::connect(pushBatton2, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button: \"Controller Stop\"";
            sp_datasource->stop();
        });

    QObject::connect(pushBatton3, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button \"Activate True\"";
            labelRemReg0->setActive(true);
            labelRemReg1->setActive(true);
        });

    QObject::connect(pushBatton4, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button \"Activate False\"";
            labelRemReg0->setActive(false);
            labelRemReg1->setActive(false);
        });

    QWidget *widget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;

    vboxLayout->addWidget(pushBatton1);
    vboxLayout->addWidget(pushBatton2);
    vboxLayout->addWidget(pushBatton3);
    vboxLayout->addWidget(pushBatton4);

    QTabWidget  *tab = new QTabWidget;

    QGridLayout *gridLayout;


    gridLayout = new QGridLayout;
    gridLayout->addWidget(labelRemReg0, 0, 0);
    widget = new QWidget();
    widget->setLayout(gridLayout);
    tab->addTab(widget, "Tab1");


    gridLayout = new QGridLayout;
    gridLayout->addWidget(labelRemReg1, 0, 0);
    widget = new QWidget();
    widget->setLayout(gridLayout);
    tab->addTab(widget, "Tab2");

    QHBoxLayout *hboxLayout = new QHBoxLayout;

    hboxLayout->addWidget(tab);
    hboxLayout->addItem(vboxLayout);

    widget = new QWidget();

    widget->setLayout(hboxLayout);
    widget->show();

    return  app.exec();
}

//--------------------------------------------------------------------------------------------------------------testGui1
int testGui1(int argc, char *argv[])
{
    using namespace remgui;
    QApplication app(argc, argv);

    QTimer t1;
    int pass_counter = 0;

    QThread *source_thread = new QThread;

    QSharedPointer<modbus::LCQModbusMasterTcp> sp_mbmaster = modbus::LCQModbusMasterTcp::create();
    sp_mbmaster->connectToHost(QUrl::fromUserInput( QStringLiteral("127.0.0.1:1502") ));
//    sp_mbmaster->connectToHost(QUrl::fromUserInput( QStringLiteral("192.168.0.2:502") ));

//    QSharedPointer<modbus::LCQModbusMasterRtu> sp_mbmaster = modbus::LCQModbusMasterRtu::create();

//    sp_mbmaster.reset();
//    sp_mbmaster = modbus::LCQModbusMasterRtu::create();
//    sp_mbmaster->connectToPort("COM3",
//                               QSerialPort::BaudRate::Baud115200,
//                               QSerialPort::Parity::OddParity,
//                               QSerialPort::DataBits::Data8,
//                               QSerialPort::StopBits::OneStop,
//                               250);

//    QSharedPointer<modbus::LCQModbusMasterTcp> sp_mbmaster = modbus::LCQModbusMasterTcp::create();
//    sp_mbmaster->moveToThread(master_thread);
//    master_thread->start();
//    sp_mbmaster->connectToHost(QHostAddress("192.168.0.2"), 502);

    QSharedPointer<modbus::LCQModbusDataSource> sp_datasource = modbus::LCQModbusDataSource::create(7, sp_mbmaster);

    sp_datasource->moveToThread(source_thread);
    source_thread->start();

    QObject::connect(&t1, &QTimer::timeout,
            [&](){

                sp_mbmaster->writeSingleRegister(7, 0, pass_counter);
                sp_mbmaster->writeSingleCoils(7, 0, pass_counter & 1);
                quint8 bits[3];
                bits[0] = pass_counter & 1;
                bits[1] = (pass_counter >> 1) & 1;
                bits[2] = (pass_counter >> 2) & 1;
                sp_mbmaster->writeMultipleCoils(7, 1, 3, bits);
                sp_mbmaster->readDiscreteInputs(7, 0, 3, bits);
                qDebug() << "main timer t1 pass din " << bits[2] << bits[1] << bits[0];
                sp_mbmaster->readCoils(7, 1, 3, bits);
                qDebug() << "main timer t1 pass coils" << bits[2] << bits[1] << bits[0];
//                qDebug() << "main timer t1 pass =" << pass_counter;
//                calcDelay(180000000);
                pass_counter++;
            });
//    t1.start(2000);

    QThread *th1 = new QThread;
    QObject::connect(th1, &QThread::started,
            [&](){
            forever
            {
                sp_mbmaster->writeSingleRegister(7, 0, pass_counter);
                sp_mbmaster->writeSingleCoils(7, 0, pass_counter & 1);
                quint8 bits[3];
                bits[0] = pass_counter & 1;
                bits[1] = (pass_counter >> 1) & 1;
                bits[2] = (pass_counter >> 2) & 1;
                sp_mbmaster->writeMultipleCoils(7, 1, 3, bits);
                sp_mbmaster->readDiscreteInputs(7, 0, 3, bits);
                qDebug() << "main timer t1 pass din " << bits[2] << bits[1] << bits[0];
                sp_mbmaster->readCoils(7, 1, 3, bits);
                qDebug() << "main timer t1 pass coils" << bits[2] << bits[1] << bits[0];
//                qDebug() << "main timer t1 pass =" << pass_counter;
//                calcDelay(180000000);
                pass_counter++;
                QThread::msleep(250);
            }
            });
//    th1->start();



    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){

            qDebug() << "QApplication::aboutToQuit";
    });


    sp_datasource.data()->addDataItemCoils("coil0", 0, 1);
    sp_datasource.data()->addDataItemCoils("coil1", 1, 1);
    sp_datasource.data()->addDataItemCoils("coil2", 2, 1);
    sp_datasource.data()->addDataItemCoils("coil3", 3, 1);
    sp_datasource.data()->addDataItemCoils("coil4", 4, 1);
    sp_datasource.data()->addDataItemCoils("coil5", 5, 1);
    sp_datasource.data()->addDataItemCoils("coil6", 6, 1);
    sp_datasource.data()->addDataItemCoils("coil7", 7, 1);
    sp_datasource.data()->addDataItemCoils("coil8", 8, 1);
    sp_datasource.data()->addDataItemCoils("coil9", 9, 1);

    sp_datasource.data()->addDataItemDiscreteInputs("din0", 0, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din1", 1, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din2", 2, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din3", 3, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din4", 4, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din5", 5, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din6", 6, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din7", 7, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din8", 8, 1);
    sp_datasource.data()->addDataItemDiscreteInputs("din9", 9, 1);

    sp_datasource.data()->addDataItemHoldingRegs("counter", 0,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Uint16",  1,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Int16",   2,  1);
    sp_datasource.data()->addDataItemHoldingRegs("Uint32",  4,  2);
    sp_datasource.data()->addDataItemHoldingRegs("Int32",   6,  2);
    sp_datasource.data()->addDataItemHoldingRegs("float32", 8,  2);

    sp_datasource.data()->addDataItemInputRegs("reg0",    0,  1);
    sp_datasource.data()->addDataItemInputRegs("reg1",    1,  1);
    sp_datasource.data()->addDataItemInputRegs("reg2",    2,  1);
    sp_datasource.data()->addDataItemInputRegs("reg3",    3,  1);
    sp_datasource.data()->addDataItemInputRegs("reg4",    4,  1);
    sp_datasource.data()->addDataItemInputRegs("reg5",    5,  1);
    sp_datasource.data()->addDataItemInputRegs("reg6",    6,  1);
    sp_datasource.data()->addDataItemInputRegs("reg7",    7,  1);
    sp_datasource.data()->addDataItemInputRegs("reg8",    8,  1);
    sp_datasource.data()->addDataItemInputRegs("reg9",    9,  1);




    QWidget *window = new QWidget;

    QLabel *labelRegName0  = new QLabel("RW0:");
    QLabel *labelRegName1  = new QLabel("RW1:");
    QLabel *labelRegName2  = new QLabel("RW2:");
    QLabel *labelRegName3  = new QLabel("RW3:");
    QLabel *labelRegName4  = new QLabel("RW4:");
    QLabel *labelRegName5  = new QLabel("RW5:");
    QLabel *labelRegName6  = new QLabel("RW6:");
    QLabel *labelRegName7  = new QLabel("RW7:");
    QLabel *labelRegName8  = new QLabel("RW8:");
    QLabel *labelRegName9  = new QLabel("RW9:");

    QSharedPointer<LCDataStrFormatBase> formatterBool(     new LCDataStrFormatBool("False", "True"));

    QSharedPointer<LCDataStrFormatBase> formatterUint8(    new LCDataStrFormatU8());
    QSharedPointer<LCDataStrFormatBase> formatterInt8(     new LCDataStrFormatS8());
    QSharedPointer<LCDataStrFormatBase> formatterUint16(   new LCDataStrFormatU16());
    QSharedPointer<LCDataStrFormatBase> formatterInt16(    new LCDataStrFormatS16());
    QSharedPointer<LCDataStrFormatBase> formatterUint32(   new LCDataStrFormatU32());
    QSharedPointer<LCDataStrFormatBase> formatterInt32(    new LCDataStrFormatS32());
    QSharedPointer<LCDataStrFormatBase> formatterFloat32(  new LCDataStrFormatF32());

    LCQRemLabel *labelRemReg0 = new LCQRemLabel("reg0", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg1 = new LCQRemLabel("reg1", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg2 = new LCQRemLabel("reg2", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg3 = new LCQRemLabel("reg3", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg4 = new LCQRemLabel("reg4", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg5 = new LCQRemLabel("reg5", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg6 = new LCQRemLabel("reg6", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg7 = new LCQRemLabel("reg7", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg8 = new LCQRemLabel("reg8", sp_datasource, formatterUint16);
    LCQRemLabel *labelRemReg9 = new LCQRemLabel("reg9", sp_datasource, formatterUint16);

    LCQRemLineEdit *lineEditRemReg0 = new LCQRemLineEdit("reg0", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg1 = new LCQRemLineEdit("reg1", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg2 = new LCQRemLineEdit("reg2", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg3 = new LCQRemLineEdit("reg3", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg4 = new LCQRemLineEdit("reg4", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg5 = new LCQRemLineEdit("reg5", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg6 = new LCQRemLineEdit("reg6", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg7 = new LCQRemLineEdit("reg7", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg8 = new LCQRemLineEdit("reg8", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemReg9 = new LCQRemLineEdit("reg9", sp_datasource, formatterUint16);

    LCQRemLabel    *labelRemData0   = new LCQRemLabel("counter", sp_datasource, formatterUint16);
    LCQRemLabel    *labelRemData1   = new LCQRemLabel("Uint16",  sp_datasource, formatterUint16);
    LCQRemLabel    *labelRemData2   = new LCQRemLabel("Int16",   sp_datasource, formatterInt16);
    LCQRemLabel    *labelRemData3   = new LCQRemLabel("Uint32",  sp_datasource, formatterUint32);
    LCQRemLabel    *labelRemData4   = new LCQRemLabel("Int32",   sp_datasource, formatterInt32);
    LCQRemLabel    *labelRemData5   = new LCQRemLabel("float32", sp_datasource, formatterFloat32);

    LCQRemLineEdit *lineEditRemData0 = new LCQRemLineEdit("counter", sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemData1 = new LCQRemLineEdit("Uint16",  sp_datasource, formatterUint16);
    LCQRemLineEdit *lineEditRemData2 = new LCQRemLineEdit("Int16",   sp_datasource, formatterInt16);
    LCQRemLineEdit *lineEditRemData3 = new LCQRemLineEdit("Uint32",  sp_datasource, formatterUint32);
    LCQRemLineEdit *lineEditRemData4 = new LCQRemLineEdit("Int32",   sp_datasource, formatterInt32);
    LCQRemLineEdit *lineEditRemData5 = new LCQRemLineEdit("float32", sp_datasource, formatterFloat32);


    QPushButton *pushBatton1 = new QPushButton("Controller Start");
    QPushButton *pushBatton2 = new QPushButton("Controller Stop");
    QPushButton *pushBatton3 = new QPushButton("Activate true");
    QPushButton *pushBatton4 = new QPushButton("Activate false");


    QGridLayout *gridLayout1 = new QGridLayout;

    int nc = 0;
    gridLayout1->addWidget(labelRegName0,  0,  nc);
    gridLayout1->addWidget(labelRegName1,  1,  nc);
    gridLayout1->addWidget(labelRegName2,  2,  nc);
    gridLayout1->addWidget(labelRegName3,  3,  nc);
    gridLayout1->addWidget(labelRegName4,  4,  nc);
    gridLayout1->addWidget(labelRegName5,  5,  nc);
    gridLayout1->addWidget(labelRegName6,  6,  nc);
    gridLayout1->addWidget(labelRegName7,  7,  nc);
    gridLayout1->addWidget(labelRegName8,  8,  nc);
    gridLayout1->addWidget(labelRegName9,  9,  nc);

    nc++;
    gridLayout1->addWidget(labelRemReg0,  0,  nc);
    gridLayout1->addWidget(labelRemReg1,  1,  nc);
    gridLayout1->addWidget(labelRemReg2,  2,  nc);
    gridLayout1->addWidget(labelRemReg3,  3,  nc);
    gridLayout1->addWidget(labelRemReg4,  4,  nc);
    gridLayout1->addWidget(labelRemReg5,  5,  nc);
    gridLayout1->addWidget(labelRemReg6,  6,  nc);
    gridLayout1->addWidget(labelRemReg7,  7,  nc);
    gridLayout1->addWidget(labelRemReg8,  8,  nc);
    gridLayout1->addWidget(labelRemReg9,  9,  nc);

    nc++;
    gridLayout1->addWidget(lineEditRemReg0,  0,  nc);
    gridLayout1->addWidget(lineEditRemReg1,  1,  nc);
    gridLayout1->addWidget(lineEditRemReg2,  2,  nc);
    gridLayout1->addWidget(lineEditRemReg3,  3,  nc);
    gridLayout1->addWidget(lineEditRemReg4,  4,  nc);
    gridLayout1->addWidget(lineEditRemReg5,  5,  nc);
    gridLayout1->addWidget(lineEditRemReg6,  6,  nc);
    gridLayout1->addWidget(lineEditRemReg7,  7,  nc);
    gridLayout1->addWidget(lineEditRemReg8,  8,  nc);
    gridLayout1->addWidget(lineEditRemReg9,  9,  nc);


    nc++;
    gridLayout1->addWidget(new QLabel("counter"),   0,  nc);//counter",
    gridLayout1->addWidget(new QLabel("Uint16"),    1,  nc);//Uint16",
    gridLayout1->addWidget(new QLabel("Int16"),     2,  nc);//Int16",
    gridLayout1->addWidget(new QLabel(""),          3,  nc);
    gridLayout1->addWidget(new QLabel("Uint32"),    4,  nc);//Uint32",
    gridLayout1->addWidget(new QLabel(""),          5,  nc);
    gridLayout1->addWidget(new QLabel("Int32"),     6,  nc);//Int32",
    gridLayout1->addWidget(new QLabel(""),          7,  nc);
    gridLayout1->addWidget(new QLabel("float32"),   8,  nc);//float32",
    gridLayout1->addWidget(new QLabel(""),          9,  nc);

    nc++;
    gridLayout1->addWidget(labelRemData0,   0,  nc);//counter",
    gridLayout1->addWidget(labelRemData1,   1,  nc);//Uint16",
    gridLayout1->addWidget(labelRemData2,   2,  nc);//Int16",
    gridLayout1->addWidget(new QLabel(""),  3,  nc);
    gridLayout1->addWidget(labelRemData3,   4,  nc);//Uint32",
    gridLayout1->addWidget(new QLabel(""),  5,  nc);
    gridLayout1->addWidget(labelRemData4,   6,  nc);//Int32",
    gridLayout1->addWidget(new QLabel(""),  7,  nc);
    gridLayout1->addWidget(labelRemData5,   8,  nc);//float32",
    gridLayout1->addWidget(new QLabel(""),  9,  nc);

    nc++;
    gridLayout1->addWidget(lineEditRemData0, 0, nc);//counter",
    gridLayout1->addWidget(lineEditRemData1, 1, nc);//Uint16",
    gridLayout1->addWidget(lineEditRemData2, 2, nc);//Int16",
    gridLayout1->addWidget(new QLabel(""),   3, nc);
    gridLayout1->addWidget(lineEditRemData3, 4, nc);//Uint32",
    gridLayout1->addWidget(new QLabel(""),   5, nc);
    gridLayout1->addWidget(lineEditRemData4, 6, nc);//Int32",
    gridLayout1->addWidget(new QLabel(""),   7, nc);
    gridLayout1->addWidget(lineEditRemData5, 8, nc);//float32",
    gridLayout1->addWidget(new QLabel(""),   9, nc);

    nc++;
    gridLayout1->addWidget(new LCQRemLineEdit("coil0", sp_datasource, formatterBool), 0, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil1", sp_datasource, formatterBool), 1, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil2", sp_datasource, formatterBool), 2, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil3", sp_datasource, formatterBool), 3, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil4", sp_datasource, formatterBool), 4, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil5", sp_datasource, formatterBool), 5, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil6", sp_datasource, formatterBool), 6, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil7", sp_datasource, formatterBool), 7, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil8", sp_datasource, formatterBool), 8, nc);
    gridLayout1->addWidget(new LCQRemLineEdit("coil9", sp_datasource, formatterBool), 9, nc);

    nc++;
    gridLayout1->addWidget(new LCQRemLabel("coil0", sp_datasource, formatterBool), 0, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil1", sp_datasource, formatterBool), 1, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil2", sp_datasource, formatterBool), 2, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil3", sp_datasource, formatterBool), 3, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil4", sp_datasource, formatterBool), 4, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil5", sp_datasource, formatterBool), 5, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil6", sp_datasource, formatterBool), 6, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil7", sp_datasource, formatterBool), 7, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil8", sp_datasource, formatterBool), 8, nc);
    gridLayout1->addWidget(new LCQRemLabel("coil9", sp_datasource, formatterBool), 9, nc);

    nc++;
    gridLayout1->addWidget(new LCQRemLabel("din0", sp_datasource, formatterBool), 0, nc);
    gridLayout1->addWidget(new LCQRemLabel("din1", sp_datasource, formatterBool), 1, nc);
    gridLayout1->addWidget(new LCQRemLabel("din2", sp_datasource, formatterBool), 2, nc);
    gridLayout1->addWidget(new LCQRemLabel("din3", sp_datasource, formatterBool), 3, nc);
    gridLayout1->addWidget(new LCQRemLabel("din4", sp_datasource, formatterBool), 4, nc);
    gridLayout1->addWidget(new LCQRemLabel("din5", sp_datasource, formatterBool), 5, nc);
    gridLayout1->addWidget(new LCQRemLabel("din6", sp_datasource, formatterBool), 6, nc);
    gridLayout1->addWidget(new LCQRemLabel("din7", sp_datasource, formatterBool), 7, nc);
    gridLayout1->addWidget(new LCQRemLabel("din8", sp_datasource, formatterBool), 8, nc);
    gridLayout1->addWidget(new LCQRemLabel("din9", sp_datasource, formatterBool), 9, nc);

    QVBoxLayout *vboxLayout1 = new QVBoxLayout;

    vboxLayout1->addWidget(pushBatton1);
    vboxLayout1->addWidget(pushBatton2);
    vboxLayout1->addWidget(pushBatton3);
    vboxLayout1->addWidget(pushBatton4);


    QHBoxLayout *hboxLayout = new QHBoxLayout;

    hboxLayout->addLayout(gridLayout1);
    hboxLayout->addLayout(vboxLayout1);

    window->setLayout(hboxLayout);

    qDebug() << "MAX quint8" << std::numeric_limits<quint8>::max();
    qDebug() << "MIN quint8" << std::numeric_limits<quint8>::min();

    qDebug() << "MAX qint8" << std::numeric_limits<qint8>::max();
    qDebug() << "MIN qint8" << std::numeric_limits<qint8>::min();

    qDebug() << "MAX quint16" << std::numeric_limits<quint16>::max();
    qDebug() << "MIN quint16" << std::numeric_limits<quint16>::min();

    qDebug() << "MAX qint16" << std::numeric_limits<qint16>::max();
    qDebug() << "MIN qint16" << std::numeric_limits<qint16>::min();

    qDebug() << "MAX quint32" << std::numeric_limits<quint32>::max();
    qDebug() << "MIN quint32" << std::numeric_limits<quint32>::min();

    qDebug() << "MAX qint32" << std::numeric_limits<qint32>::max();
    qDebug() << "MIN qint32" << std::numeric_limits<qint32>::min();

    qDebug() << "MAX float" << std::numeric_limits<float>::max();
    qDebug() << "MIN float" << std::numeric_limits<float>::min();


    LCDataStrFormatU16 df(4, true, 16);

    qDebug() << "undefStateString()" << df.undefStateString();
    qDebug() << "toBytes()" << df.toBytes("ffff");

    quint16 a = 256;

    qDebug() << "toString("<< a << ")" << df.toString(QByteArray((char*)&a, 2));

    QObject::connect(pushBatton1, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button: \"Controller Start\"";
            sp_datasource->start();
        });

    QObject::connect(pushBatton2, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button: \"Controller Stop\"";
            sp_datasource->stop();
        });

    QObject::connect(pushBatton3, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button \"Activate True\"";
            labelRemReg0->setActive(true);
            labelRemReg1->setActive(true);
            labelRemReg2->setActive(true);
            labelRemReg3->setActive(true);
            labelRemReg4->setActive(true);
            labelRemReg5->setActive(true);
            labelRemReg6->setActive(true);
            labelRemReg7->setActive(true);
            labelRemReg8->setActive(true);
            labelRemReg9->setActive(true);

            lineEditRemReg0->setActive(true);
            lineEditRemReg1->setActive(true);
            lineEditRemReg2->setActive(true);
            lineEditRemReg3->setActive(true);
            lineEditRemReg4->setActive(true);
            lineEditRemReg5->setActive(true);
            lineEditRemReg6->setActive(true);
            lineEditRemReg7->setActive(true);
            lineEditRemReg8->setActive(true);
            lineEditRemReg9->setActive(true);

            labelRemData0->setActive(true);
            labelRemData1->setActive(true);
            labelRemData2->setActive(true);
            labelRemData3->setActive(true);
            labelRemData4->setActive(true);
            labelRemData5->setActive(true);

            lineEditRemData0->setActive(true);
            lineEditRemData1->setActive(true);
            lineEditRemData2->setActive(true);
            lineEditRemData3->setActive(true);
            lineEditRemData4->setActive(true);
            lineEditRemData5->setActive(true);

        });

    QObject::connect(pushBatton4, &QPushButton::clicked,
        [&](){
            qDebug() << "Push Button \"Activate False\"";
            labelRemReg0->setActive(false);
            labelRemReg1->setActive(false);
            labelRemReg2->setActive(false);
            labelRemReg3->setActive(false);
            labelRemReg4->setActive(false);
            labelRemReg5->setActive(false);
            labelRemReg6->setActive(false);
            labelRemReg7->setActive(false);
            labelRemReg8->setActive(false);
            labelRemReg9->setActive(false);

            lineEditRemReg0->setActive(false);
            lineEditRemReg1->setActive(false);
            lineEditRemReg2->setActive(false);
            lineEditRemReg3->setActive(false);
            lineEditRemReg4->setActive(false);
            lineEditRemReg5->setActive(false);
            lineEditRemReg6->setActive(false);
            lineEditRemReg7->setActive(false);
            lineEditRemReg8->setActive(false);
            lineEditRemReg9->setActive(false);

            labelRemData0->setActive(false);
            labelRemData1->setActive(false);
            labelRemData2->setActive(false);
            labelRemData3->setActive(false);
            labelRemData4->setActive(false);
            labelRemData5->setActive(false);

            lineEditRemData0->setActive(false);
            lineEditRemData1->setActive(false);
            lineEditRemData2->setActive(false);
            lineEditRemData3->setActive(false);
            lineEditRemData4->setActive(false);
            lineEditRemData5->setActive(false);
        });

    window->show();

    return app.exec();
}


//---------------------------------------------------------------------------------------------testModbusMasterSimulator
//int testModbusMasterSimulator(int argc, char *argv[])
//{
//    QCoreApplication app(argc, argv);
//    qDebug() << "-----------------------------------------START TEST0";
//    QTimer t1;

////    QThread *th1 = new QThread();//(obj);
//    QThread *th1 = new QThread();//(obj);
//    QThread *th2 = new QThread();//(obj);
//    QThread *th3 = new QThread();//(obj);
//    QThread *th4 = new QThread();//(obj);

////    QSharedPointer<modbus::LCQModbusMasterSimulator>
////            master_sim(new modbus::LCQModbusMasterSimulator(), doDelLater);
////    modbus::LCQModbusMasterSimulator *master_sim = new modbus::LCQModbusMasterSimulator();
////    master_sim->moveToThread(th4);
////    master_sim->connectToHost(QHostAddress("192.168.0.2"), 502);

////    modbus::LCQModbusMasterSimulator *master_sim = new modbus::LCQModbusMasterSimulator();

//    QSharedPointer<modbus::LCQModbusMasterTcp>
//            master_sim = modbus::LCQModbusMasterTcp::create();
//    master_sim->moveToThread(th4);
//    master_sim->connectToHost(QHostAddress("192.168.0.2"), 502);
//    th4->start();

//    master_sim = modbus::LCQModbusMasterTcp::create();
//master_sim->moveToThread(th4);
//master_sim->connectToHost(QHostAddress("192.168.0.2"), 502);

//    QObject::connect(&app, &QCoreApplication::aboutToQuit,
//         [&](){
//             qDebug() << "Application Quit";
//             th4->quit();
//             th4->wait();
//             master_sim.clear();
//         });

//    qDebug() << "-----------------------------------------START TEST1";
//    int pass_counter_timer1  = 0;
//    int pass_counter_thread1 = 0;
//    int pass_counter_thread2 = 0;
//    int pass_counter_thread3 = 0;

//    QObject::connect(th1, &QThread::started,
//        [&](){
//            lstd::quint8  devId  = 7;
//            lstd::quint16 addr   = 200;
//            lstd::quint16 length = 4;
//            lstd::quint16 regs[length];
//            forever{
//                qDebug() << "-----------------------------------------thread #1 pass =" << pass_counter_thread1;
//                qDebug() << "-----------------------------------------readHoldingRegisters";
//                switch(master_sim->readHoldingRegisters(devId, addr, length, regs))
//                {
//                case modbus::LCQModbusMasterTcp::EOperationStatus::UNREC_ERROR:
//                    qDebug() << "readHoldingRegisters EOperationStatus::UNREC";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_REQ:
//                    qDebug() << "readHoldingRegisters EOperationStatus::WRONG_REQ";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_RESP:
//                    qDebug() << "readHoldingRegisters EOperationStatus::WRONG_RESP";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::RESP_TIMEOUT:
//                    qDebug() << "readHoldingRegisters EOperationStatus::RESP_TIMEOUT";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::NO_CONNECTION:
//                    qDebug() << "readHoldingRegisters EOperationStatus::NO_CONNECTION";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::MODBUS_EXCEPTION:
//                    qDebug() << "readHoldingRegisters EOperationStatus::MODBUS_EXCEPTION";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::OK:
//                    qDebug() << "-----------------------------------------EOperationStatus::OK";
//                    for(int i = 0; i < length; i++)
//                    {
//                        qDebug() << "reg[" << i << "] = " << regs[i];
//                    }
//                    break;
//                default:
//                    break;
//                }
//                pass_counter_thread1++;
//                QThread::msleep(1000);
//            }
//        });

//    QObject::connect(th2, &QThread::started,
//        [&](){
//            forever{
//                qDebug() << "-----------------------------------------thread #2 pass =" << pass_counter_thread2;
//                qDebug() << "-----------------------------------------writeSingleRegister";
//                switch(master_sim->writeSingleRegister(7,200,pass_counter_thread2))
//                {
//                case modbus::LCQModbusMasterTcp::EOperationStatus::UNREC_ERROR:
//                    qDebug() << "writeSingleRegister EOperationStatus::UNREC";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_REQ:
//                    qDebug() << "writeSingleRegister EOperationStatus::WRONG_REQ";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_RESP:
//                    qDebug() << "writeSingleRegister EOperationStatus::WRONG_RESP";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::RESP_TIMEOUT:
//                    qDebug() << "writeSingleRegister EOperationStatus::RESP_TIMEOUT";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::NO_CONNECTION:
//                    qDebug() << "writeSingleRegister EOperationStatus::NO_CONNECTION";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::MODBUS_EXCEPTION:
//                    qDebug() << "writeSingleRegister EOperationStatus::MODBUS_EXCEPTION";
//                    break;
//                case modbus::LCQModbusMasterTcp::EOperationStatus::OK:
//                    qDebug() << "writeSingleRegister EOperationStatus::EOperationStatus::OK";
//                    break;
//                default:
//                    break;
//                }
//                pass_counter_thread2++;
//                QThread::msleep(1000);
//            }
//        });

//    QObject::connect(th3, &QThread::started,
//        [&](){
//            forever{
//                qDebug() << "-----------------------------------------thread #3 pass =" << pass_counter_thread3;
//                master_sim->writeMultipleRegisters(0,0,0,nullptr);
//                pass_counter_thread3++;
//                QThread::msleep(1000);

//                lstd::quint8  devId  = 7;
//                lstd::quint16 addr   = 202;
//                lstd::quint16 length = 4;
////                lstd::quint16 regs[length];

//                lstd::quint32 pass_counter = 0;
//                lstd::quint32 reg = 0;

//                forever{
//                    qDebug() << "-----------------------------------------thread #3 pass =" << pass_counter;
//                    qDebug() << "-----------------------------------------writeMultipleRegisters";
//                    switch(master_sim->writeMultipleRegisters(devId, addr, 2, (lstd::quint16*)(&reg)))
//                    {
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::UNREC_ERROR:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::UNREC";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_REQ:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::WRONG_REQ";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::WRONG_RESP:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::WRONG_RESP";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::RESP_TIMEOUT:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::RESP_TIMEOUT";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::NO_CONNECTION:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::NO_CONNECTION";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::MODBUS_EXCEPTION:
//                        qDebug() << "writeMultipleRegisters EOperationStatus::MODBUS_EXCEPTION";
//                        break;
//                    case modbus::LCQModbusMasterTcp::EOperationStatus::OK:
//                        qDebug() << "-----------------------------------------EOperationStatus::OK";
//                        break;
//                    default:
//                        break;
//                    }
//                    pass_counter++;
//                    reg += 10000;
//                    QThread::msleep(1000);
//                }
//            }
//        });

//    QObject::connect(&t1, &QTimer::timeout,
//            [&](){
//                qDebug() << "main timer t1 pass =" << pass_counter_timer1;
////                calcDelay(180000000);
////                master_sim.readHoldingRegisters(0,0,0,nullptr);
//                pass_counter_timer1++;
//            });


//    th1->start();
//    th2->start();
//    th3->start();
//    t1.start(250);



//    signal(SIGINT, exitApp);
//    return app.exec();
//}



//-----------------------------------------------------------------------------------------------------testGui1Simulator
//int testGui1Simulator(int argc, char *argv[])
//{
//    using namespace remgui;
//    QApplication app(argc, argv);
//QSharedPointer<modbus::LCQModbusMasterSimulator> mbmaster = modbus::LCQModbusMasterSimulator::create();
//QSharedPointer<modbus::LCQModbusDataSource> mbsource = modbus::LCQModbusDataSource::create(7, mbmaster);
////    QSharedPointer<modbus::LCQModbusMasterSimulator> mbmaster = modbus::LCQModbusMasterSimulator::create();
////    QSharedPointer<modbus::LCQModbusDataSource> mbsource(mbmaster);

//    mbsource.data()->addDataItemHoldingRegs("counter", 0,  1);
//    mbsource.data()->addDataItemHoldingRegs("Uint8",   1,  1);
//    mbsource.data()->addDataItemHoldingRegs("Int8",    2,  1);
//    mbsource.data()->addDataItemHoldingRegs("Uint16",  3,  1);
//    mbsource.data()->addDataItemHoldingRegs("Int16",   4,  1);
//    mbsource.data()->addDataItemHoldingRegs("Uint32",  5,  2);
//    mbsource.data()->addDataItemHoldingRegs("reg4",    6,  1);
//    mbsource.data()->addDataItemHoldingRegs("Int32",   7,  2);
//    mbsource.data()->addDataItemHoldingRegs("reg6",    8,  1);
//    mbsource.data()->addDataItemHoldingRegs("float32", 9,  2);
//    mbsource.data()->addDataItemHoldingRegs("reg8",    10, 1);

//    QObject::connect(&app, &QApplication::aboutToQuit,
//                     [&](){
//                         qDebug() << "Application Quit";
//                         mbsource.data()->stop();
//                     });

//    QWidget *window = new QWidget;

//    QLabel *labelRegName0  = new QLabel("REG0:");
//    QLabel *labelRegName1  = new QLabel("REG1:");
//    QLabel *labelRegName2  = new QLabel("REG2:");
//    QLabel *labelRegName3  = new QLabel("REG3:");
//    QLabel *labelRegName4  = new QLabel("REG4:");
//    QLabel *labelRegName5  = new QLabel("REG5:");
//    QLabel *labelRegName6  = new QLabel("REG6:");
//    QLabel *labelRegName7  = new QLabel("REG7:");
//    QLabel *labelRegName8  = new QLabel("REG8:");
//    QLabel *labelRegName9  = new QLabel("REG9:");
//    QLabel *labelRegName10 = new QLabel("REG10:");

//    QLabel *labelDevReg0  = new QLabel("REG0");
//    QLabel *labelDevReg1  = new QLabel("REG1");
//    QLabel *labelDevReg2  = new QLabel("REG2");
//    QLabel *labelDevReg3  = new QLabel("REG3");
//    QLabel *labelDevReg4  = new QLabel("REG4");
//    QLabel *labelDevReg5  = new QLabel("REG5");
//    QLabel *labelDevReg6  = new QLabel("REG6");
//    QLabel *labelDevReg7  = new QLabel("REG7");
//    QLabel *labelDevReg8  = new QLabel("REG8");
//    QLabel *labelDevReg9  = new QLabel("REG9");
//    QLabel *labelDevReg10 = new QLabel("REG10");

//    QLineEdit *lineDevRegEdit0  = new QLineEdit;
//    QLineEdit *lineDevRegEdit1  = new QLineEdit;
//    QLineEdit *lineDevRegEdit2  = new QLineEdit;
//    QLineEdit *lineDevRegEdit3  = new QLineEdit;
//    QLineEdit *lineDevRegEdit4  = new QLineEdit;
//    QLineEdit *lineDevRegEdit5  = new QLineEdit;
//    QLineEdit *lineDevRegEdit6  = new QLineEdit;
//    QLineEdit *lineDevRegEdit7  = new QLineEdit;
//    QLineEdit *lineDevRegEdit8  = new QLineEdit;
//    QLineEdit *lineDevRegEdit9  = new QLineEdit;
//    QLineEdit *lineDevRegEdit10 = new QLineEdit;

//    QSharedPointer<LCQDataStrFormatBase> formatterUint8(new LCDataStrFormatU8());
//    QSharedPointer<LCQDataStrFormatBase> formatterInt8(new LCDataStrFormatS8());
//    QSharedPointer<LCQDataStrFormatBase> formatterUint16(new LCDataStrFormatU16());
//    QSharedPointer<LCQDataStrFormatBase> formatterInt16(new LCDataStrFormatS16());
//    QSharedPointer<LCQDataStrFormatBase> formatterUint32(new LCDataStrFormatU32());
//    QSharedPointer<LCQDataStrFormatBase> formatterInt32(new LCDataStrFormatS32());
//    QSharedPointer<LCQDataStrFormatBase> formatterFloat32(new LCDataStrFormatF32());

//    LCQRemLabel *labelRemRegData0 = new LCQRemLabel("counter", mbsource, formatterUint16);
//    LCQRemLabel *labelRemRegData1 = new LCQRemLabel("Uint8",   mbsource, formatterUint8);
//    LCQRemLabel *labelRemRegData2 = new LCQRemLabel("Int8",    mbsource, formatterInt8);
//    LCQRemLabel *labelRemRegData3 = new LCQRemLabel("Uint16",  mbsource, formatterUint16);
//    LCQRemLabel *labelRemRegData4 = new LCQRemLabel("Int16",   mbsource, formatterInt16);
//    LCQRemLabel *labelRemRegData5 = new LCQRemLabel("Uint32",  mbsource, formatterUint32);
//    LCQRemLabel *labelRemRegData6 = new LCQRemLabel("Int32",   mbsource, formatterInt32);
//    LCQRemLabel *labelRemRegData7 = new LCQRemLabel("float32", mbsource, formatterFloat32);

//    LCQRemLineEdit *lineRemRegEdit0 = new LCQRemLineEdit("counter", mbsource, formatterUint16);
////    QLineEdit *lineRemRegEdit0 = new QLineEdit;
////    QLineEdit *lineRemRegEdit1 = new QLineEdit;
////    QLineEdit *lineRemRegEdit2 = new QLineEdit;
//    LCQRemLineEdit *lineRemRegEdit3 = new LCQRemLineEdit("Uint16",  mbsource, formatterUint16);
////    QLineEdit *lineRemRegEdit3 = new QLineEdit;
//    LCQRemLineEdit *lineRemRegEdit4 = new LCQRemLineEdit("Int16",   mbsource, formatterInt16);
////    QLineEdit *lineRemRegEdit4 = new QLineEdit;
//    LCQRemLineEdit *lineRemRegEdit5 = new LCQRemLineEdit("Uint32",  mbsource, formatterUint32);
////    QLineEdit *lineRemRegEdit5 = new QLineEdit;
//    LCQRemLineEdit *lineRemRegEdit6 = new LCQRemLineEdit("Int32",   mbsource, formatterInt32);
////    QLineEdit *lineRemRegEdit6 = new QLineEdit;
//    LCQRemLineEdit *lineRemRegEdit7 = new LCQRemLineEdit("float32", mbsource, formatterFloat32);
////    QLineEdit *lineRemRegEdit7 = new QLineEdit;

//    QPushButton *pushBatton1 = new QPushButton("Controller Start");
//    QPushButton *pushBatton2 = new QPushButton("Controller Stop");
//    QPushButton *pushBatton3 = new QPushButton("Activate true");
//    QPushButton *pushBatton4 = new QPushButton("Activate false");
//    QPushButton *pushBatton5 = new QPushButton("Test Read");


//    QGridLayout *gridLayout1 = new QGridLayout;

//    int nc = 0;
//    gridLayout1->addWidget(labelRegName0,  0,  nc);
//    gridLayout1->addWidget(labelRegName1,  1,  nc);
//    gridLayout1->addWidget(labelRegName2,  2,  nc);
//    gridLayout1->addWidget(labelRegName3,  3,  nc);
//    gridLayout1->addWidget(labelRegName4,  4,  nc);
//    gridLayout1->addWidget(labelRegName5,  5,  nc);
//    gridLayout1->addWidget(labelRegName6,  6,  nc);
//    gridLayout1->addWidget(labelRegName7,  7,  nc);
//    gridLayout1->addWidget(labelRegName8,  8,  nc);
//    gridLayout1->addWidget(labelRegName9,  9,  nc);
//    gridLayout1->addWidget(labelRegName10, 10, nc);

//    nc++;
//    gridLayout1->addWidget(labelDevReg0,  0,  nc);
//    gridLayout1->addWidget(labelDevReg1,  1,  nc);
//    gridLayout1->addWidget(labelDevReg2,  2,  nc);
//    gridLayout1->addWidget(labelDevReg3,  3,  nc);
//    gridLayout1->addWidget(labelDevReg4,  4,  nc);
//    gridLayout1->addWidget(labelDevReg5,  5,  nc);
//    gridLayout1->addWidget(labelDevReg6,  6,  nc);
//    gridLayout1->addWidget(labelDevReg7,  7,  nc);
//    gridLayout1->addWidget(labelDevReg8,  8,  nc);
//    gridLayout1->addWidget(labelDevReg9,  9,  nc);
//    gridLayout1->addWidget(labelDevReg10, 10, nc);

//    nc++;
//    gridLayout1->addWidget(lineDevRegEdit0,  0,  nc);
//    gridLayout1->addWidget(lineDevRegEdit1,  1,  nc);
//    gridLayout1->addWidget(lineDevRegEdit2,  2,  nc);
//    gridLayout1->addWidget(lineDevRegEdit3,  3,  nc);
//    gridLayout1->addWidget(lineDevRegEdit4,  4,  nc);
//    gridLayout1->addWidget(lineDevRegEdit5,  5,  nc);
//    gridLayout1->addWidget(lineDevRegEdit6,  6,  nc);
//    gridLayout1->addWidget(lineDevRegEdit7,  7,  nc);
//    gridLayout1->addWidget(lineDevRegEdit8,  8,  nc);
//    gridLayout1->addWidget(lineDevRegEdit9,  9,  nc);
//    gridLayout1->addWidget(lineDevRegEdit10, 10, nc);

//    nc++;
//    gridLayout1->addWidget(new QLabel("counter"),   0,  nc);//counter",
//    gridLayout1->addWidget(new QLabel("Uint8"),     1,  nc);//Uint8",
//    gridLayout1->addWidget(new QLabel("Int8"),      2,  nc);//Int8",
//    gridLayout1->addWidget(new QLabel("Uint16"),    3,  nc);//Uint16",
//    gridLayout1->addWidget(new QLabel("Int16"),     4,  nc);//Int16",
//    gridLayout1->addWidget(new QLabel("Uint32"),    5,  nc);//Uint32",
//    gridLayout1->addWidget(new QLabel(""),          6,  nc);
//    gridLayout1->addWidget(new QLabel("Int32"),     7,  nc);//Int32",
//    gridLayout1->addWidget(new QLabel(""),          8,  nc);
//    gridLayout1->addWidget(new QLabel("float32"),   9,  nc);//float32",
//    gridLayout1->addWidget(new QLabel(""),          10, nc);

//    nc++;
//    gridLayout1->addWidget(labelRemRegData0, 0,  nc);//counter",
//    gridLayout1->addWidget(labelRemRegData1, 1,  nc);//Uint8",
//    gridLayout1->addWidget(labelRemRegData2, 2,  nc);//Int8",
//    gridLayout1->addWidget(labelRemRegData3, 3,  nc);//Uint16",
//    gridLayout1->addWidget(labelRemRegData4, 4,  nc);//Int16",
//    gridLayout1->addWidget(labelRemRegData5, 5,  nc);//Uint32",
//    gridLayout1->addWidget(new QLabel(""),   6,  nc);
//    gridLayout1->addWidget(labelRemRegData6, 7,  nc);//Int32",
//    gridLayout1->addWidget(new QLabel(""),   8,  nc);
//    gridLayout1->addWidget(labelRemRegData7, 9,  nc);//float32",
//    gridLayout1->addWidget(new QLabel(""),   10, nc);

//    nc++;
//    gridLayout1->addWidget(lineRemRegEdit0, 0,  nc);
//    gridLayout1->addWidget(new QLabel(""),  1,  nc);
//    gridLayout1->addWidget(new QLabel(""),  2,  nc);
//    gridLayout1->addWidget(lineRemRegEdit3, 3,  nc);
//    gridLayout1->addWidget(lineRemRegEdit4, 4,  nc);
//    gridLayout1->addWidget(lineRemRegEdit5, 5,  nc);
//    gridLayout1->addWidget(new QLabel(""),  6,  nc);
//    gridLayout1->addWidget(lineRemRegEdit6, 7,  nc);
//    gridLayout1->addWidget(new QLabel(""),  8,  nc);
//    gridLayout1->addWidget(lineRemRegEdit7, 9,  nc);
//    gridLayout1->addWidget(new QLabel(""),  10, nc);


//    QVBoxLayout *vboxLayout1 = new QVBoxLayout;

//    vboxLayout1->addWidget(pushBatton1);
//    vboxLayout1->addWidget(pushBatton2);
//    vboxLayout1->addWidget(pushBatton3);
//    vboxLayout1->addWidget(pushBatton4);
//    vboxLayout1->addWidget(pushBatton5);

//    QHBoxLayout *hboxLayout = new QHBoxLayout;

//    hboxLayout->addLayout(gridLayout1);
//    hboxLayout->addLayout(vboxLayout1);

//    window->setLayout(hboxLayout);

//    QObject::connect(mbmaster.data(), &modbus::LCQModbusMasterSimulator::dataChange,
//        [&](){
//            labelDevReg0->  setText(QString::number(mbmaster->getRegister(0) ));
//            labelDevReg1->  setText(QString::number(mbmaster->getRegister(1) ));
//            labelDevReg2->  setText(QString::number(mbmaster->getRegister(2) ));
//            labelDevReg3->  setText(QString::number(mbmaster->getRegister(3) ));
//            labelDevReg4->  setText(QString::number(mbmaster->getRegister(4) ));
//            labelDevReg5->  setText(QString::number(mbmaster->getRegister(5) ));
//            labelDevReg6->  setText(QString::number(mbmaster->getRegister(6) ));
//            labelDevReg7->  setText(QString::number(mbmaster->getRegister(7) ));
//            labelDevReg8->  setText(QString::number(mbmaster->getRegister(8) ));
//            labelDevReg9->  setText(QString::number(mbmaster->getRegister(9) ));
//            labelDevReg10-> setText(QString::number(mbmaster->getRegister(10)));

//        });

//    lineDevRegEdit1->setValidator(formatterUint8.data()->validator());
//    QObject::connect(lineDevRegEdit1, &QLineEdit::returnPressed,
//                     [&](){
//                         mbmaster->setRegister(1, (quint16)lineDevRegEdit1->text().toInt());
//                     });

//    lineDevRegEdit2->setValidator(formatterInt8.data()->validator());
//    QObject::connect(lineDevRegEdit2, &QLineEdit::returnPressed,
//                     [&](){
//                            mbmaster->setRegister(2, (quint16)lineDevRegEdit2->text().toInt());
//                     });

//    lineDevRegEdit3->setValidator(formatterUint16.data()->validator());
//    QObject::connect(lineDevRegEdit3, &QLineEdit::returnPressed,
//                     [&](){
//                         mbmaster->setRegister(3, (quint16)lineDevRegEdit3->text().toInt());
//                     });

//    lineDevRegEdit4->setValidator(formatterInt16.data()->validator());
//    QObject::connect(lineDevRegEdit4, &QLineEdit::returnPressed,
//                     [&](){
//                        mbmaster->setRegister(4, (quint16)lineDevRegEdit4->text().toInt());
//                     });

//    lineDevRegEdit5->setValidator(formatterUint32.data()->validator());
//    QObject::connect(lineDevRegEdit5, &QLineEdit::returnPressed,
//                     [&](){
//                         quint32 r = lineDevRegEdit5->text().toUInt();
//                         mbmaster->setRegister(5, ((quint16*)&r)[0] );
//                         mbmaster->setRegister(6, ((quint16*)&r)[1] );
//                     });
//    lineDevRegEdit6->setValidator(new QIntValidator(0, 65535, lineDevRegEdit6));
//    QObject::connect(lineDevRegEdit6, &QLineEdit::returnPressed,
//                     [&](){
//                         mbmaster->setRegister(6, (quint16)lineDevRegEdit6->text().toInt());
//                     });
//    lineDevRegEdit7->setValidator(formatterInt32.data()->validator());
//    QObject::connect(lineDevRegEdit7, &QLineEdit::returnPressed,
//                     [&](){
//                        qint32 r = lineDevRegEdit7->text().toInt();
//                        mbmaster->setRegister(7, ((quint16*)&r)[0] );
//                        mbmaster->setRegister(8, ((quint16*)&r)[1] );
//                     });
//    lineDevRegEdit8->setValidator(new QIntValidator(0, 65535, lineDevRegEdit8));
//    QObject::connect(lineDevRegEdit8, &QLineEdit::returnPressed,
//                     [&](){
//                         mbmaster->setRegister(8, (quint16)lineDevRegEdit8->text().toInt());
//                     });
//    lineDevRegEdit9->setValidator(formatterFloat32.data()->validator());
//    QObject::connect(lineDevRegEdit9, &QLineEdit::returnPressed,
//                     [&](){
//                            float r = lineDevRegEdit9->text().toFloat();
//                            mbmaster->setRegister(9, ((quint16*)&r)[0] );
//                            mbmaster->setRegister(10, ((quint16*)&r)[1] );
//                     });
//    lineDevRegEdit10->setValidator(new QIntValidator(0, 65535, lineDevRegEdit10));
//    QObject::connect(lineDevRegEdit10, &QLineEdit::returnPressed,
//                     [&](){
//                         mbmaster->setRegister(10, (quint16)lineDevRegEdit10->text().toInt());
//                     });


//    qDebug() << "MAX quint8" << std::numeric_limits<quint8>::max();
//    qDebug() << "MIN quint8" << std::numeric_limits<quint8>::min();

//    qDebug() << "MAX qint8" << std::numeric_limits<qint8>::max();
//    qDebug() << "MIN qint8" << std::numeric_limits<qint8>::min();

//    qDebug() << "MAX quint16" << std::numeric_limits<quint16>::max();
//    qDebug() << "MIN quint16" << std::numeric_limits<quint16>::min();

//    qDebug() << "MAX qint16" << std::numeric_limits<qint16>::max();
//    qDebug() << "MIN qint16" << std::numeric_limits<qint16>::min();

//    qDebug() << "MAX quint32" << std::numeric_limits<quint32>::max();
//    qDebug() << "MIN quint32" << std::numeric_limits<quint32>::min();

//    qDebug() << "MAX qint32" << std::numeric_limits<qint32>::max();
//    qDebug() << "MIN qint32" << std::numeric_limits<qint32>::min();

//    qDebug() << "MAX float" << std::numeric_limits<float>::max();
//    qDebug() << "MIN float" << std::numeric_limits<float>::min();


//    LCDataStrFormatU16 df(4, true, 16);

//    qDebug() << "undefStateString()" << df.undefStateString();
//    qDebug() << "toBytes()" << df.toBytes("ffff");

//    quint16 a = 256;

//    qDebug() << "toString("<< a << ")" << df.toString(QByteArray((char*)&a, 2));

//    LCQRemoteDataReader rem_read("counter", mbsource);

////    QObject::connect(pushBatton5, &QPushButton::clicked,
////        [&](){
////            qDebug() << "Push Button \"Test Read\"";
//////            rem_read.readData();
////        });

//    QObject::connect(pushBatton5,
//                     &QPushButton::clicked,
//                     &rem_read,
//                     &LCQRemoteDataReader::readRequest);

//    QObject::connect(&rem_read, &LCQRemoteDataReader::dataIsRead,
//                     [&](QSharedPointer<QByteArray> _data,
//                        LCQRemoteDataSourceBase::EDataStatus _status)
//                        {
//                            if(_status == LCQRemoteDataSourceBase::EDataStatus::DS_OK)
//                                lineDevRegEdit0->setText(formatterUint16->toString(*_data));
//                            else
//                                lineDevRegEdit0->setText("####");
//                        });

//    QObject::connect(pushBatton1, &QPushButton::clicked,
//        [&](){
//            qDebug() << "Push Button: \"Controller Start\"";
//            mbsource->start();
//        });

//    QObject::connect(pushBatton2, &QPushButton::clicked,
//        [&](){
//            qDebug() << "Push Button: \"Controller Stop\"";
//            mbsource->stop();
//        });

//    QObject::connect(pushBatton3, &QPushButton::clicked,
//        [&](){
//            qDebug() << "Push Button \"Activate True\"";
//            labelRemRegData0->setActive(true);
//            labelRemRegData1->setActive(true);
//            labelRemRegData2->setActive(true);
//            labelRemRegData3->setActive(true);
//            labelRemRegData4->setActive(true);
//            labelRemRegData5->setActive(true);
//            labelRemRegData6->setActive(true);
//            labelRemRegData7->setActive(true);

//            lineRemRegEdit0->setActive(true);
//            lineRemRegEdit3->setActive(true);
//            lineRemRegEdit4->setActive(true);
//            lineRemRegEdit5->setActive(true);
//            lineRemRegEdit6->setActive(true);
//            lineRemRegEdit7->setActive(true);

//        });

//    QObject::connect(pushBatton4, &QPushButton::clicked,
//        [&](){
//            qDebug() << "Push Button \"Activate False\"";
//            labelRemRegData0->setActive(false);
//            labelRemRegData1->setActive(false);
//            labelRemRegData2->setActive(false);
//            labelRemRegData3->setActive(false);
//            labelRemRegData4->setActive(false);
//            labelRemRegData5->setActive(false);
//            labelRemRegData6->setActive(false);
//            labelRemRegData7->setActive(false);

//            lineRemRegEdit0->setActive(false);
//            lineRemRegEdit3->setActive(false);
//            lineRemRegEdit4->setActive(false);
//            lineRemRegEdit5->setActive(false);
//            lineRemRegEdit6->setActive(false);
//            lineRemRegEdit7->setActive(false);
//        });

//    window->show();

//    return app.exec();
//}

