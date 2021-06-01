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

INCLUDEPATH +=                              \
    ../../../common                         \
    ../../../common/interfaces/xmlbuilders  \
    ../../../common/interfaces/remotedata   \
    ../../../common/interfaces/keyboard     \
    ../../../common/interfaces              \
    ./builders                              \

SOURCES += \
    examplewidgets.cpp \
    builders/lcxmlswitchwidgetslistbuilder.cpp \

HEADERS += \
    ../../../common/interfaces/LIApplication.h \
    ../../../common/interfaces/LIDataFormatter.h \
    ../../../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../../../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../../../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../../common/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
    ../../../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../../../common/interfaces/keyboard/LIKeyboard.h \
    ../../../common/interfaces/keyboard/LIKeyboardListener.h \
    examplewidgets_global.h \
    examplewidgets.h \
    builders/lcxmlswitchwidgetslistbuilder.h \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
