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

INCLUDEPATH +=                                  \
        ../../common/interfaces                 \
        ../../common/interfaces/remotedata      \
        ../../common/interfaces/xmlbuilders     \
        ../../common/                           \
        remgui/                                 \
        xml/                                    \

SOURCES += \
        ../../common/lqvalidatoru32.cpp \
        lctestdllclass.cpp \
        remgui/lcqremlabel.cpp \
        remgui/lcqremlineedit.cpp\
        xml/lcxmlapplication.cpp \
        xml/lcxmlbuildersloader.cpp \
        xml/lcxmllayoutsfactory.cpp \
        xml/lcxmlremotedatasourcebuilders.cpp \
        xml/lcxmlremotedatasourcemap.cpp \
        xml/lcxmlwidgetbuilders.cpp \
        xml/lcxmlwidgetcreatorsmap.cpp \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../common/interfaces/LIApplication.h \
    ../../common/interfaces/remotedata/LIRemoteDataReadListener.h \
    ../../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../../common/interfaces/remotedata/LIRemoteDataWriteListner.h \
    ../../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../common/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
    ../../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../../common/lqvalidatoru32.h \
    lctestdllclass.h    \
    testdllinterface.h  \
    ../../common/lqextendevent.h \
    remgui/lcqremlabel.h \
    remgui/lcqremlineedit.h \
    remgui/lremgui.h \
    xml/LCXmlWidgetCreatorsMapInterface.h \
    xml/lcxmlapplication.h \
    xml/lcxmlbuildersloader.h \
    xml/lcxmllayoutsfactory.h \
    xml/lcxmlremotedatasourcebuilders.h \
    xml/lcxmlremotedatasourcemap.h \
    xml/lcxmlstddataformatterfactory.h \
    xml/lcxmlwidgetbuilders.h \
    xml/lcxmlwidgetcreatorinterface.h \
    xml/lcxmlwidgetcreatorsmap.h \

