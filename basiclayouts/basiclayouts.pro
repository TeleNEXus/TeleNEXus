QT += gui
QT += widgets
QT += xml

TEMPLATE = lib
DEFINES += BASICLAYOUTS_LIBRARY

CONFIG += c++11

RC_FILE     = basiclayouts_resource.rc

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
    ../../common/interfaces/xmlbuilders \
    ../../common/interfaces/remotedata  \
    ../../common/interfaces             \
    builders                            \

SOURCES += \
    basiclayouts.cpp \
    builders/common.cpp \
    builders/lcxmlboxlayoutbuilder.cpp \
    builders/lcxmlgridlayoutbuilder.cpp \
    builders/lcxmllayoutbuilderbase.cpp

HEADERS += \
    ../../common/interfaces/LIApplication.h \
    ../../common/interfaces/remotedata/LIRemoteDataSource.h \
    ../../common/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
    ../../common/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
    ../../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
    basiclayouts_global.h \
    basiclayouts.h \
    builders/common.h \
    builders/lcxmlboxlayoutbuilder.h \
    builders/lcxmlgridlayoutbuilder.h \
    builders/lcxmllayoutbuilderbase.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
