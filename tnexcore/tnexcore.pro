QT += gui
QT += widgets
QT += network
#QT += serialport
#QT += serialbus
QT += xml
QT += qml 

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
TARGET = tnex

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
../tnexcommon/interfaces                    \
../tnexcommon/interfaces/remotedata         \
../tnexcommon/interfaces/xmlbuilders        \
../tnexcommon/interfaces/keyboard           \
../tnexcommon/                              \
./localsource                           \
./security                              \
./window                                \
./xml                                   \
./common                                \
./dataformatters                        \
./jscripts/jscriptservice               \
./jscripts/jscriptservice/jsappservice  \
./jscripts/jsformatter                  \
./jscripts                              \
./keyboard                              \
./basiclayouts                          \
./basicwidgets                          \
./basicwidgets/builders                 \
./basicwidgets/widgets                  \
./basicwidgets/builders/events          \
./projectsource                         \


SOURCES += \
localsource/lcdataitemmap.cpp               \
localsource/lclocalsource.cpp               \
localsource/lcqlocaldatareader.cpp          \
localsource/lcqlocaldatawriter.cpp          \
localsource/lcqlocalsourcehiden.cpp         \
localsource/lcxmllocalsourcebuilder.cpp     \
localsource/lqreadsync.cpp                  \
localsource/lqwritesync.cpp                 \
security/lcsecurity.cpp \
security/lqsecurityfilter.cpp \
basiclayouts/commonbuilderslayout.cpp \
basiclayouts/lcxmlboxlayoutbuilder.cpp  \
basiclayouts/lcxmlgridlayoutbuilder.cpp \
basiclayouts/basiclayoutbuilders.cpp \
basicwidgets/widgets/lcqlistwidget.cpp \
basicwidgets/widgets/lcqdatacombobox.cpp \
basicwidgets/widgets/lcqdatalabel.cpp \
basicwidgets/widgets/lcqstackwidget.cpp \
basicwidgets/widgets/lcqpushlabel.cpp \
basicwidgets/widgets/lcqpushbutton.cpp \
basicwidgets/widgets/lcqdatacheckbox.cpp \
basicwidgets/widgets/lcqdatalineedit.cpp \
basicwidgets/builders/lqeventsfilter.cpp \
basicwidgets/builders/lcqwidgetvisiblecontrol.cpp \
basicwidgets/builders/lcxmlbuilderbase.cpp \
basicwidgets/builders/lcxmlframebuilder.cpp \
basicwidgets/builders/lcxmlgroupboxbuilder.cpp \
basicwidgets/builders/lcxmlbuttonbuilder.cpp \
basicwidgets/builders/lcxmllabelbuilder.cpp \
basicwidgets/builders/lcxmltablewidgetbuilder.cpp \
basicwidgets/builders/lcxmldatalineeditbuilder.cpp \
basicwidgets/builders/lcxmldatacomboboxbuilder.cpp \
basicwidgets/builders/lcxmltabwidgetbuilder.cpp \
basicwidgets/builders/lcxmlscrollareabuilder.cpp \
basicwidgets/builders/lcxmllistwidgetbuilder.cpp \
basicwidgets/builders/lcxmlstackwidgetbuilder.cpp \
basicwidgets/builders/lcxmlsplitterbuilder.cpp \
basicwidgets/builders/lcxmlstdactionbuilder.cpp \
basicwidgets/builders/lcxmlpushareabuilder.cpp \
basicwidgets/builders/lcxmlcheckboxbuilder.cpp \
basicwidgets/basicwidgetbuilders.cpp \
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
dataformatters/lcqvalidatoru32.cpp \
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
jscripts/jscriptcommon.cpp \
common/xmlcommon.cpp \
xml/xmlbuilders.cpp \
xml/xmljscripts.cpp \
xml/xmldataformatters.cpp \
xml/xmldatasources.cpp \
xml/xmlkeyboards.cpp \
xml/xmlwindows.cpp \
xml/xmlpluginpathes.cpp \
xml/xmlwidgetstyles.cpp \
xml/xmlsecurity.cpp \
xml/xmlglobalstyle.cpp \
keyboard/lckeyboard.cpp \
window/lcwindow.cpp \
applicationinterface.cpp \
projectsource/projectsource.cpp \
projectsource/lqrobuffer.cpp \
main.cpp \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
../tnexcommon/lqextendevent.h \
../tnexcommon/interfaces/LIApplication.h \
../tnexcommon/interfaces/remotedata/LIRemoteDataReader.h \
../tnexcommon/interfaces/remotedata/LIRemoteDataSource.h \
../tnexcommon/interfaces/remotedata/LIRemoteDataWriter.h \
../tnexcommon/interfaces/xmlbuilders/LIXmlRemoteDataSourceBuilder.h \
../tnexcommon/interfaces/xmlbuilders/LIXmlLayoutBuilder.h \
../tnexcommon/interfaces/xmlbuilders/LIXmlWidgetBuilder.h \
../tnexcommon/interfaces/LIWindow.h \
../tnexcommon/interfaces/LIDataFormatter.h \
../tnexcommon/interfaces/LIJScriptService.h \
../tnexcommon/interfaces/keyboard/LIKeyboard.h \
../tnexcommon/interfaces/keyboard/LIKeyboardListener.h \
localsource/lcdataitemmap.h                \
localsource/lclocalsource.h                \
localsource/lcqlocaldatareader.h           \
localsource/lcqlocaldatawriter.h           \
localsource/lcqlocalsourcehiden.h          \
localsource/lcxmllocalsourcebuilder.h      \
localsource/lqreadsync.h                  \
localsource/lqwritesync.h                 \
security/lcsecurity.h \
security/lqsecurityfilter.h \
basiclayouts/commonbuilderslayout.h \
basiclayouts/lcxmlboxlayoutbuilder.h    \
basiclayouts/lcxmlgridlayoutbuilder.h   \
basicwidgets/widgets/lcqlistwidget.h \
basicwidgets/widgets/lcqdatacombobox.h \
basicwidgets/widgets/lcqdatalabel.h \
basicwidgets/widgets/lcqstackwidget.h \
basicwidgets/widgets/lcqpushlabel.h \
basicwidgets/widgets/lcqpushbutton.h \
basicwidgets/widgets/lcqdatacheckbox.h \
basicwidgets/widgets/lcqdatalineedit.h \
basicwidgets/builders/lcxmlbuilderbase.h \
basicwidgets/builders/lcxmldatalineeditbuilder.h \
basicwidgets/builders/lcxmllabelbuilder.h \
basicwidgets/builders/lcxmldatalineeditbuilder.h \
basicwidgets/builders/lcxmlframebuilder.h \
basicwidgets/builders/lcxmlgroupboxbuilder.h \
basicwidgets/builders/lcxmldatacomboboxbuilder.h \
basicwidgets/builders/lcxmltablewidgetbuilder.h \
basicwidgets/builders/lcxmltabwidgetbuilder.h \
basicwidgets/builders/lcxmlscrollareabuilder.h \
basicwidgets/builders/lcxmlbuttonbuilder.h \
basicwidgets/builders/lcxmllistwidgetbuilder.h \
basicwidgets/builders/lcqwidgetvisiblecontrol.h \
basicwidgets/builders/lcxmlstackwidgetbuilder.h \
basicwidgets/builders/lcxmlsplitterbuilder.h \
basicwidgets/builders/lcxmlstdactionbuilder.h \
basicwidgets/builders/lcxmlpushareabuilder.h \
basicwidgets/builders/lcxmlcheckboxbuilder.h\
basicwidgets/builders/lqeventsfilter.h \
basicwidgets/basicwidgetbuilders.h \
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
dataformatters/lcqvalidatoru32.h \
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
jscripts/jscriptcommon.h \
common/xmlcommon.h \
xml/xmlbuilders.h \
xml/xmldataformatters.h \
xml/xmldatasources.h \
xml/xmljscripts.h \
xml/xmlkeyboards.h \
xml/xmlwindows.h \
xml/xmlpluginpathes.h \
xml/xmlwidgetstyles.h \
xml/xmlsecurity.h \
xml/xmlglobalstyle.h \
keyboard/lckeyboard.h \
window/lcwindow.h \
applicationinterface.h \
projectsource/projectsource.h \
projectsource/lqrobuffer.h \
version.h \


