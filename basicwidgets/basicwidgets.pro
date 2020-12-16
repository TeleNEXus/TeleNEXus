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
    ./common                            \
    ./builders                          \
    ./widgets                           \


SOURCES += \
    ../../common/lqvalidatoru32.cpp \
    basicwidgets.cpp \
    widgets/lcqremlabel.cpp \
    widgets/lcqremlineedit.cpp \
    widgets/lcqremcombobox.cpp \
    widgets/lcqremcombolabel.cpp \
    widgets/lcqremwritebutton.cpp \
    builders/lcxmlbuilderbase.cpp \
    builders/lcxmlremlabelbuilder.cpp \
    builders/lcxmlremlineeditbuilder.cpp \
    builders/lcxmltextlabelbuilder.cpp \
    builders/lcxmlwidgetbuilder.cpp \
    builders/lcxmlremcomboboxbuilder.cpp \
    builders/lcxmlremcombolabelbuilder.cpp \
    builders/lcxmltablewidgetbuilder.cpp \
    builders/lcxmltabwidgetbuilder.cpp \
    builders/lcxmlswitchwidgetslistbuilder.cpp \
    builders/lcxmlscrollareabuilder.cpp \
    builders/lcxmlbuttonbuilder.cpp \
    builders/lcbuilderscommon.cpp \


HEADERS += \
    ../../common/interfaces/LIApplication.h \
    ../../common/interfaces/LIDataFormatter.h \
    ../../common/interfaces/remotedata/LIRemoteDataReader.h \
    ../../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../../common/interfaces/remotedata/LIRemoteDataWriter.h \
    ../../common/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
    ../../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    ../../common/lqextendevent.h \
    ../../common/lqvalidatoru32.h \
    basicwidgets_global.h \
    basicwidgets.h \
    common/LIMovieAccess.h \
    widgets/lcqremlabel.h \
    widgets/lcqremlineedit.h \
    widgets/lcqremcombobox.h \
    widgets/lcqremcombolabel.h \
    widgets/lcqremwritebutton.h \
    builders/lcxmlbuilderbase.h \
    builders/lcxmlremlineeditbuilder.h \
    builders/lcxmlremlabelbuilder.h \
    builders/lcxmlremlineeditbuilder.h \
    builders/lcxmltextlabelbuilder.h \
    builders/lcxmlwidgetbuilder.h \
    builders/lcxmlremcomboboxbuilder.h \
    builders/lcxmlremcombolabelbuilder.h \
    builders/lcxmltablewidgetbuilder.h \
    builders/lcxmltabwidgetbuilder.h \
    builders/lcxmlswitchwidgetslistbuilder.h \
    builders/lcxmlscrollareabuilder.h \
    builders/lcxmlbuttonbuilder.h \
    builders/lcbuilderscommon.h \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
