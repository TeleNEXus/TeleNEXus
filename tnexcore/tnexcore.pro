QT += gui
QT += widgets
QT += network
#QT += serialport
#QT += serialbus
QT += xml
QT += qml 

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

INCLUDEPATH +=                          \
../common/interfaces                    \
../common/interfaces/remotedata         \
../common/interfaces/xmlbuilders        \
../common/interfaces/keyboard           \
../common/                              \
./xml                                   \
./dataformatters                        \
./jscripts/jscriptservice               \
./jscripts/jscriptservice/jsappservice  \
./jscripts/jsformatter                  \
./jscripts                              \
./keyboard


SOURCES += \
../../common/lqvalidatoru32.cpp \
dataformatters/lcformatterbase.cpp \
dataformatters/lcformatterbitfield.cpp \
dataformatters/lcformatterbits.cpp \
dataformatters/lcformatterbool.cpp \
dataformatters/lcformatterf32.cpp \
dataformatters/lcformatterhex.cpp \
dataformatters/lcformatterintbase.cpp \
dataformatters/lcformatters16.cpp \
dataformatters/lcformatters32.cpp \
dataformatters/lcformatters8.cpp \
dataformatters/lcformatteru16.cpp \
dataformatters/lcformatteru32.cpp \
dataformatters/lcformatteru8.cpp \
dataformatters/lcformatterstring.cpp \
dataformatters/lcformattertextstream.cpp \
dataformatters/lcxmlformatterfactory.cpp \
jscripts/jsformatter/lcjsformatter.cpp \
jscripts/jsformatter/lcqjsformatterinterface.cpp \
jscripts/jscriptservice/lcjscriptservice.cpp \
jscripts/jscriptservice/lcqjscriptservicehiden.cpp \
jscripts/jscriptservice/cqjsfilebase.cpp \
jscripts/jscriptservice/cqjstextfile.cpp \
jscripts/jscriptservice/cqjsbinaryfile.cpp \
jscripts/jscriptservice/cqjsprocess.cpp \
jscripts/jscriptservice/jsappservice/lcqjsappservice.cpp \
jscripts/jscriptservice/jsappservice/lcqreadfromsourcereq.cpp \
jscripts/jscriptservice/jsappservice/lcqwritetosourcereq.cpp \
xml/lcxmlcommon.cpp \
xml/tnex.cpp \
xml/lcxmlbuildersloader.cpp \
xml/lcxmllayoutbuilders.cpp \
xml/lcxmlremotedatasourcebuilders.cpp \
xml/lcxmlwidgetbuilders.cpp \
xml/lcxmlwindows.cpp \
xml/lcxmlfonts.cpp \
xml/uploadjscripts.cpp \
xml/uploaddataformatters.cpp \
xml/uploadkeyboards.cpp \
keyboard/lckeyboard.cpp \
main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
../common/lqextendevent.h \
../common/lqvalidatoru32.h \
../common/interfaces/LIApplication.h \
../common/interfaces/remotedata/LIRemoteDataReader.h \
../common/interfaces/remotedata/LIRemoteDataSource.h \
../common/interfaces/remotedata/LIRemoteDataWriter.h \
../common/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
../common/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
../common/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
../common/interfaces/LIWindow.h \
../common/interfaces/LIDataFormatter.h \
../common/interfaces/LIJScriptService.h \
../common/interfaces/keyboard/LIKeyboard.h \
../common/interfaces/keyboard/LIKeyboardListener.h \
dataformatters/lcformatterbase.h \
dataformatters/lcformatterbitfield.h \
dataformatters/lcformatterbits.h \
dataformatters/lcformatterbool.h \
dataformatters/lcformatterf32.h \
dataformatters/lcformatterhex.h \
dataformatters/lcformatterintbase.h \
dataformatters/lcformatters16.h \
dataformatters/lcformatters32.h \
dataformatters/lcformatters8.h \
dataformatters/lcformatteru16.h \
dataformatters/lcformatteru32.h \
dataformatters/lcformatteru8.h \
dataformatters/lcformatterstring.h \
dataformatters/lcformattertextstream.cpp \
dataformatters/lcxmlformatterfactory.h \
jscripts/jsformatter/lcjsformatter.h \
jscripts/jsformatter/lcqjsformatterinterface.h \
jscripts/jscriptservice/lcjscriptservice.h \
jscripts/jscriptservice/lcqjscriptservicehiden.h \
jscripts/jscriptservice/cqjsfilebase.h \
jscripts/jscriptservice/cqjstextfile.h \
jscripts/jscriptservice/cqjsbinaryfile.h \
jscripts/jscriptservice/cqjsprocess.h \
jscripts/jscriptservice/jsappservice/lcqjsappservice.h \
jscripts/jscriptservice/jsappservice/lcqreadfromsourcereq.h \
jscripts/jscriptservice/jsappservice/lcqwritetosourcereq.h \
xml/lcxmlcommon.h \
xml/tnex.h \
xml/lcxmlbuildersloader.h \
xml/lcxmllayoutbuilders.h \
xml/lcxmlremotedatasourcebuilders.h \
xml/lcxmlwidgetbuilders.h \
xml/lcxmlwindows.h \
xml/lcxmlfonts.h \
xml/uploadjscripts.h \
xml/uploadkeyboards.h \
keyboard/lckeyboard.h \


