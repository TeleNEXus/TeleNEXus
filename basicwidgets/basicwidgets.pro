QT += gui
QT += widgets
QT += xml

TEMPLATE = lib
DEFINES += BASICWIDGETS_LIBRARY

CONFIG += c++11

RC_FILE     = basicwidgets_resource.rc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=                          \
    ../../common                        \
    ../../common/interfaces/xmlbuilders \
    ../../common/interfaces/remotedata  \
    ../../common/interfaces             \
    builders                            \
    widgets                             \
    widgets/dataformatters


SOURCES += \
    ../../common/lqvalidatoru32.cpp \
    basicwidgets.cpp \
    builders/lcxmlremotedatalabelbuilder.cpp \
    builders/lcxmlremotedatalineeditbuilder.cpp \
    builders/lcxmltextlabelbuilder.cpp \
    builders/lcxmlwidgetbuilder.cpp \
    widgets/dataformatters/lcstringdataformatterbase.cpp \
    widgets/dataformatters/lcstringdataformatterbool.cpp \
    widgets/dataformatters/lcstringdataformatterf32.cpp \
    widgets/dataformatters/lcstringdataformatterhex.cpp \
    widgets/dataformatters/lcstringdataformatterintbase.cpp \
    widgets/dataformatters/lcstringdataformatters16.cpp \
    widgets/dataformatters/lcstringdataformatters32.cpp \
    widgets/dataformatters/lcstringdataformatters8.cpp \
    widgets/dataformatters/lcstringdataformatteru16.cpp \
    widgets/dataformatters/lcstringdataformatteru32.cpp \
    widgets/dataformatters/lcstringdataformatteru8.cpp \
    widgets/dataformatters/lcxmlstddataformatterfactory.cpp \
    widgets/lcqremlabel.cpp \
    widgets/lcqremlineedit.cpp

HEADERS += \
    ../../common/interfaces/LIApplication.h \
    ../../common/interfaces/remotedata/LIRemoteDataReadListener.h \
    ../../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../../common/interfaces/remotedata/LIRemoteDataWriteListner.h \
    ../../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../common/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
    ../../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../../common/lqextendevent.h \
    ../../common/lqvalidatoru32.h \
    basicwidgets_global.h \
    basicwidgets.h \
    builders/lcxmlremotedatalabelbuilder.h \
    builders/lcxmlremotedatalineeditbuilder.h \
    builders/lcxmltextlabelbuilder.h \
    builders/lcxmlwidgetbuilder.h \
    widgets/dataformatters/LIStringDataFormatter.h \
    widgets/dataformatters/lcstringdataformatterbase.h \
    widgets/dataformatters/lcstringdataformatterbool.h \
    widgets/dataformatters/lcstringdataformatterf32.h \
    widgets/dataformatters/lcstringdataformatterhex.h \
    widgets/dataformatters/lcstringdataformatterintbase.h \
    widgets/dataformatters/lcstringdataformatters16.h \
    widgets/dataformatters/lcstringdataformatters32.h \
    widgets/dataformatters/lcstringdataformatters8.h \
    widgets/dataformatters/lcstringdataformatteru16.h \
    widgets/dataformatters/lcstringdataformatteru32.h \
    widgets/dataformatters/lcstringdataformatteru8.h \
    widgets/dataformatters/lcxmlstddataformatterfactory.h \
    widgets/lcqremlabel.h \
    widgets/lcqremlineedit.h \
    widgets/lremgui.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
