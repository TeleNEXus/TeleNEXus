QT += gui
QT += widgets
QT += network
QT += serialport
QT += serialbus
QT += xml

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=                  \
        modbus/                 \
        common/                 \
        remgui/                 \
        remgui/datastrformat/   \
        xml/                    \
        remotedatainterface/


SOURCES += \
        common/lcqremotedatareader.cpp \
        common/lcqremotedatasourcebase.cpp \
        common/lcqremotedatawriter.cpp \
        common/lcqu32validator.cpp \
        modbus/lcqmodbusdatareader.cpp \
        modbus/lcqmodbusdatasource.cpp \
        modbus/lcmodbusmasterbase.cpp \
        modbus/lcqmodbusdatawriter.cpp \
        modbus/lcqmodbusmasterrtu.cpp \
        modbus/lcqmodbusmastertcp.cpp \
        lctestdllclass.cpp \
        cqclientecho2.cpp \
        cqclientechoblocking1.cpp \
        cqclienttcpecho.cpp \
        cqmythread.cpp \
        cqsocketclient1.cpp \
        cqtestimer.cpp \
        lcxmltestwidgetcreators.cpp \
        lqtestsaxhandler.cpp \
        remgui/datastrformat/lcdatastrformatbase.cpp \
        remgui/datastrformat/lcdatastrformatbool.cpp \
        remgui/datastrformat/lcdatastrformatf32.cpp \
        remgui/datastrformat/lcdatastrformathex.cpp \
        remgui/datastrformat/lcdatastrformatintbase.cpp \
        remgui/datastrformat/lcdatastrformats16.cpp \
        remgui/datastrformat/lcdatastrformats32.cpp \
        remgui/datastrformat/lcdatastrformats8.cpp \
        remgui/datastrformat/lcdatastrformatu16.cpp \
        remgui/datastrformat/lcdatastrformatu32.cpp \
        remgui/datastrformat/lcdatastrformatu8.cpp \
        remgui/lcqremlabel.cpp \
        remgui/lcqremlineedit.cpp\
        main.cpp \
        xml/lcxmlapplication.cpp \
        xml/lcxmllayoutsfactory.cpp \
        xml/lcxmlremotedatasourcemap.cpp \
        xml/lcxmlstddataformatterfactory.cpp \
        xml/lcxmlwidgetcreatorbase.cpp \
        xml/lcxmlwidgetcreatorsmap.cpp \
        xmldatasources/lcxmlmodbussources.cpp \
        xmldatasources/lcxmlremotedatasourcecreatebase.cpp \
        xmlwidgets/lcxmllabel.cpp \
        xmlwidgets/lcxmlremlabel.cpp \
        xmlwidgets/lcxmlremlineedit.cpp \
        xmlwidgets/lcxmlwidget.cpp




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    common/lcqremotedatareader.h \
    common/lcqremotedatasourcebase.h \
    common/lcqremotedatawriter.h \
    common/lcqu32validator.h \
    common/lqextendevent.h \
    cqclientecho2.h \
    cqclientechoblocking1.h \
    cqclienttcpecho.h \
    cqmythread.h \
    cqsocketclient1.h \
    cqtestimer.h \
    lctestdllclass.h \
    lcxmltestwidgetcreators.h \
    lqtestsaxhandler.h \
    modbus/lcmodbusmasterbase.h \
    modbus/lcqmodbusdatareader.h \
    modbus/lcqmodbusdatawriter.h \
    modbus/lcqmodbusmasterrtu.h \
    modbus/lcqmodbusmastertcp.h \
    modbus/lmodbusdefs.h \
    modbus/lcqmodbusdatasource.h \
    remgui/datastrformat/lcdatastrformatbase.h \
    remgui/datastrformat/lcdatastrformatbool.h \
    remgui/datastrformat/lcdatastrformatf32.h \
    remgui/datastrformat/lcdatastrformathex.h \
    remgui/datastrformat/lcdatastrformatintbase.h \
    remgui/datastrformat/lcdatastrformats16.h \
    remgui/datastrformat/lcdatastrformats32.h \
    remgui/datastrformat/lcdatastrformats8.h \
    remgui/datastrformat/lcdatastrformatu16.h \
    remgui/datastrformat/lcdatastrformatu32.h \
    remgui/datastrformat/lcdatastrformatu8.h \
    remgui/lcqremlabel.h \
    remgui/lcqremlineedit.h \
    remgui/lremgui.h \ \
    remotedatainterface/LCRemoteDataReadListnerInterface.h \
    remotedatainterface/LCRemoteDataReaderInterface.h \
    remotedatainterface/LCRemoteDataSourceInterface.h \
    remotedatainterface/LCRemoteDataWriteListnerInterface.h \
    remotedatainterface/LCRemoteDataWriterInterface.h \
    testdllinterface.h \
    xml/lcxmlapplication.h \
    xml/lcxmllayoutsfactory.h \
    xml/lcxmlremotedatasourcemap.h \
    xml/lcxmlstddataformatterfactory.h \
    xml/lcxmlwidgetcreatorbase.h \
    xml/lcxmlwidgetcreatorsmap.h \
    xmldatasources/lcxmlmodbussources.h \
    xmldatasources/lcxmlremotedatasourcecreatebase.h \
    xmlwidgets/lcxmllabel.h \
    xmlwidgets/lcxmlremlabel.h \
    xmlwidgets/lcxmlremlineedit.h \
    xmlwidgets/lcxmlwidget.h

