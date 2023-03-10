QT += gui
QT += widgets
QT += xml

TEMPLATE = lib
DEFINES += BASICWIDGETS_LIBRARY

CONFIG += c++11

RC_FILE     = examplewidgets_resource.rc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=                                      \
    ../../../../tnexcommon                          \
    ../../../../tnexcommon/interfaces/xmlbuilders   \
    ../../../../tnexcommon/interfaces/remotedata    \
    ../../../../tnexcommon/interfaces/keyboard      \
    ../../../../tnexcommon/interfaces               \
    ./builders                                      \
    ./widgets                                       \

SOURCES += \
    examplewidgets.cpp \
    builders/lcxmllcdnumberbuilder.cpp \
    widgets/lcqdatalcdnumber.cpp \

HEADERS += \
    ../../../../tnexcommon/interfaces/LIApplication.h \
    ../../../../tnexcommon/interfaces/LIDataFormatter.h \
    ../../../../tnexcommon/interfaces/remotedata/LIRemoteDataReader.h \
    ../../../../tnexcommon/interfaces/remotedata/LIRemoteDataSource.h \
    ../../../../tnexcommon/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../../../tnexcommon/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
    ../../../../tnexcommon/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../../../../tnexcommon/interfaces/keyboard/LIKeyboard.h \
    ../../../../tnexcommon/interfaces/keyboard/LIKeyboardListener.h \
    examplewidgets_global.h \
    examplewidgets.h \
    builders/lcxmllcdnumberbuilder.h \
    widgets/lcqdatalcdnumber.h \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
