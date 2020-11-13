QT += gui
QT += widgets
QT += network
#QT += serialport
#QT += serialbus
QT += xml

CONFIG += console c++11
#CONFIG += c++11
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

INCLUDEPATH +=                                  \
        ../common/interfaces                 \
        ../common/interfaces/remotedata      \
        ../common/interfaces/xmlbuilders     \
        ../common/                           \
        xml/                                    \

SOURCES += \
        ../../common/lqvalidatoru32.cpp \
        xml/lcxmlapplication.cpp \
        xml/lcxmlbuildersloader.cpp \
        xml/lcxmllayoutbuilders.cpp \
        xml/lcxmlremotedatasourcebuilders.cpp \
        xml/lcxmlwidgetbuilders.cpp \
        xml/lcxmlwindows.cpp \
        xml/lcxmlfonts.cpp \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../common/lqextendevent.h \
    ../common/lqvalidatoru32.h \
    ../common/interfaces/LIApplication.h \
    ../common/interfaces/remotedata/LIRemoteDataReadListener.h \
    ../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../common/interfaces/remotedata/LIRemoteDataWriteListner.h \
    ../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../common/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
    ../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../common/interfaces/LIWindow.h \
    xml/lcxmlapplication.h \
    xml/lcxmlbuildersloader.h \
    xml/lcxmllayoutbuilders.h \
    xml/lcxmlremotedatasourcebuilders.h \
    xml/lcxmlwidgetbuilders.h \
    xml/lcxmlwindows.h \
    xml/lcxmlfonts.h \

