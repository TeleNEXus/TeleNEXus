QT -= gui
QT += network
QT += serialport
QT += serialbus
QT += xml

TEMPLATE = lib
DEFINES += MBSOURCE_LIBRARY

CONFIG += c++11

RC_FILE     = modbussource_resource.rc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=                                  \
    ../../tnexcommon                            \
    ../../tnexcommon/interfaces                 \
    ../../tnexcommon/interfaces/remotedata      \
    ../../tnexcommon/interfaces/xmlbuilders     \
    builders                                    \
    modbus

HEADERS += \
    ../../tnexcommon/interfaces/LIApplication.h \
    ../../tnexcommon/interfaces/remotedata/LIRemoteDataReader.h \
    ../../tnexcommon/interfaces/remotedata/LIRemoteDataSource.h \
    ../../tnexcommon/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../tnexcommon/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
    ../../tnexcommon/lqextendevent.h \
    modbus/lcmemoryreadset.h \
    modbus/lmodbusdefs.h \
    modbus/lcmodbusdatamap.h \
    modbus/lqmodbusdatareader.h \
    modbus/lqmodbusdatasource.h \
    modbus/lqmodbusdatawriter.h \
    modbus/lqmodbusmasterbase.h \
    modbus/lqmodbusmasterrtu.h  \
    modbus/lqmodbusmastertcp.h  \
    modbus/lqreadsyncreq.h \
    modbus/lqwritesyncreq.h \
    builders/lcxmlmodbussourcebuilder.h  \
    modbussource.h \
    modbussource_global.h

SOURCES += \
    modbus/lcmemoryreadset.cpp \
    modbus/lcmodbusdatamap.cpp \
    modbus/lqmodbusdatareader.cpp \
    modbus/lqmodbusdatasource.cpp \
    modbus/lqmodbusdatawriter.cpp \
    modbus/lqmodbusmasterbase.cpp \
    modbus/lqmodbusmasterrtu.cpp  \
    modbus/lqmodbusmastertcp.cpp  \
    modbus/lqreadsyncreq.cpp \
    modbus/lqwritesyncreq.cpp \
    builders/lcxmlmodbussourcebuilder.cpp \
    modbussource.cpp \
    version.h 

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
