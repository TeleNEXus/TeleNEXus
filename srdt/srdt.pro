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
        ../common/interfaces/remotedata     \
        ../common/interfaces/xmlcreators    \
        ../common/                          \
        ../modbussource/                    \
                                            \
        remgui/                 \
        xml/                    \
        dataformatters/         \


SOURCES += \
        ../common/lqvalidatoru32.cpp \
        ../modbussource/lqmodbusdatareader.cpp \
        ../modbussource/lqmodbusdatasource.cpp \
        ../modbussource/lqmodbusdatawriter.cpp \
        ../modbussource/lqmodbusmasterbase.cpp \
        ../modbussource/lqmodbusmasterrtu.cpp \
        ../modbussource/lqmodbusmastertcp.cpp \
        lctestdllclass.cpp \
        dataformatters/lcstringdataformatterbase.cpp \
        dataformatters/lcstringdataformatterbool.cpp \
        dataformatters/lcstringdataformatterf32.cpp \
        dataformatters/lcstringdataformatterhex.cpp \
        dataformatters/lcstringdataformatterintbase.cpp \
        dataformatters/lcstringdataformatters16.cpp \
        dataformatters/lcstringdataformatters32.cpp \
        dataformatters/lcstringdataformatters8.cpp \
        dataformatters/lcstringdataformatteru16.cpp \
        dataformatters/lcstringdataformatteru32.cpp \
        dataformatters/lcstringdataformatteru8.cpp \
        remgui/lcqremlabel.cpp \
        remgui/lcqremlineedit.cpp\
        main.cpp \
        xml/lcxmlapplication.cpp \
        xml/lcxmllayoutsfactory.cpp \
        xml/lcxmlremotedatasourcemap.cpp \
        xml/lcxmlstddataformatterfactory.cpp \
        xml/lcxmlwidgetcreatorsmap.cpp \
        xmldatasources/lcxmlmodbussources.cpp \
        xmlwidgets/lcxmllabel.cpp \
        xmlwidgets/lcxmlremlabel.cpp \
        xmlwidgets/lcxmlremlineedit.cpp \
        xmlwidgets/lcxmlwidget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../common/interfaces/LIApplication.h \
    ../common/interfaces/remotedata/LIRemoteDataReadListener.h \
    ../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../common/interfaces/remotedata/LIRemoteDataWriteListner.h \
    ../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../common/interfaces/xmlcreators/LIXmlRemoteDataSourceCreator.h \
    ../common/lqvalidatoru32.h \
    ../modbussource/lqmodbusdatareader.h \
    ../modbussource/lqmodbusdatasource.h \
    ../modbussource/lqmodbusdatawriter.h \
    ../modbussource/lqmodbusmasterbase.h \
    ../modbussource/lqmodbusmasterrtu.h \
    ../modbussource/lqmodbusmastertcp.h \
    lctestdllclass.h    \
    testdllinterface.h  \
    ../common/lqextendevent.h \
    dataformatters/LCStringDataFormatterInterface.h \
    dataformatters/lcstringdataformatterbase.h \
    dataformatters/lcstringdataformatterbool.h \
    dataformatters/lcstringdataformatterf32.h \
    dataformatters/lcstringdataformatterhex.h \
    dataformatters/lcstringdataformatterintbase.h \
    dataformatters/lcstringdataformatters16.h \
    dataformatters/lcstringdataformatters32.h \
    dataformatters/lcstringdataformatters8.h \
    dataformatters/lcstringdataformatteru16.h \
    dataformatters/lcstringdataformatteru32.h \
    dataformatters/lcstringdataformatteru8.h \
    ../modbussource/lmodbusdefs.h \
    remgui/lcqremlabel.h \
    remgui/lcqremlineedit.h \
    remgui/lremgui.h \
    xml/LCXmlStdDataFormatterFactoryInterface.h \
    xml/LCXmlWidgetCreatorsMapInterface.h \
    xml/lcxmlapplication.h \
    xml/lcxmllayoutsfactory.h \
    xml/lcxmlremotedatasourcemap.h \
    xml/lcxmlstddataformatterfactory.h \
    xml/lcxmlwidgetcreatorinterface.h \
    xml/lcxmlwidgetcreatorsmap.h \
    xmldatasources/lcxmlmodbussources.h \
    xmlwidgets/lcxmllabel.h \
    xmlwidgets/lcxmlremlabel.h \
    xmlwidgets/lcxmlremlineedit.h \
    xmlwidgets/lcxmlwidget.h
