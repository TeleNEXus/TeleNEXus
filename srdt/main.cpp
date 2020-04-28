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
#include "lctestdllclass.h"

class CTestDllRecurse : public LCTestDllInterface
{
public:
    CTestDllRecurse(){}
    virtual ~CTestDllRecurse(){}
    virtual QString getString() override {return "LCTestDllInterface Test Dll String";}
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
    typedef QSharedPointer<LCTestDllInterface> (*TFct)();

    TFct fct = (TFct)(lib.resolve("getTestDllInterface"));

//    CTestDllRecurse tci;
    if(!fct)
    {
        qDebug()<< "No recognize function";
    }
    else
    {
        QSharedPointer<LCTestDllInterface> test = fct();
        qDebug()<< test->getString();
    }
    qDebug()<< "End programm";

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

