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
../tnexcommon/interfaces                    \
../tnexcommon/interfaces/remotedata         \
../tnexcommon/interfaces/xmlbuilders        \
../tnexcommon/interfaces/keyboard           \
../tnexcommon/                              \
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


SOURCES += \
basiclayouts/commonbuilderslayout.cpp \
basiclayouts/lcxmlboxlayoutbuilder.cpp  \
basiclayouts/lcxmlgridlayoutbuilder.cpp \
basiclayouts/basiclayoutbuilders.cpp \
basicwidgets/widgets/lcqlistwidget.cpp \
basicwidgets/widgets/lcqremcombobox.cpp \
basicwidgets/widgets/lcqremlabel.cpp \
basicwidgets/widgets/lcqremcombolabel.cpp \
basicwidgets/widgets/lcqstackedwidget.cpp \
basicwidgets/builders/lcxmlbuilderbase.cpp \
basicwidgets/builders/lcxmlremlabelbuilder.cpp \
basicwidgets/builders/lcxmlremlineeditbuilder.cpp \
basicwidgets/builders/lcxmltextlabelbuilder.cpp \
basicwidgets/builders/lcxmlwidgetbuilder.cpp \
basicwidgets/builders/lcxmlremcomboboxbuilder.cpp \
basicwidgets/builders/lcxmlremcombolabelbuilder.cpp \
basicwidgets/builders/lcxmltablewidgetbuilder.cpp \
basicwidgets/builders/lcxmltabwidgetbuilder.cpp \
basicwidgets/builders/lcxmlscrollareabuilder.cpp \
basicwidgets/builders/lcxmlbuttonbuilder.cpp \
basicwidgets/builders/lcxmllistwidgetbuilder.cpp \
basicwidgets/builders/lcbuilderscommon.cpp \
basicwidgets/builders/lcqwidgetvisiblecontrol.cpp \
basicwidgets/builders/lcxmlstackedwidgetbuilder.cpp \
basicwidgets/builders/lcxmlsplitterbuilder.cpp \
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
common/xmlcommon.cpp \
xml/xmlbuilders.cpp \
xml/xmlfonts.cpp \
xml/xmljscripts.cpp \
xml/xmldataformatters.cpp \
xml/xmldatasources.cpp \
xml/xmlkeyboards.cpp \
xml/xmlwindows.cpp \
xml/xmlpluginpathes.cpp \
keyboard/lckeyboard.cpp \
window/lcwindow.cpp \
applicationinterface.cpp \
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
basiclayouts/commonbuilderslayout.h \
basiclayouts/lcxmlboxlayoutbuilder.h    \
basiclayouts/lcxmlgridlayoutbuilder.h   \
basicwidgets/widgets/lcqlistwidget.h \
basicwidgets/widgets/lcqremcombobox.h \
basicwidgets/widgets/lcqremcombolabel.h \
basicwidgets/widgets/lcqremlabel.h \
basicwidgets/widgets/lcqstackedwidget.h \
basicwidgets/builders/lcxmlbuilderbase.h \
basicwidgets/builders/lcxmlremlineeditbuilder.h \
basicwidgets/builders/lcxmlremlabelbuilder.h \
basicwidgets/builders/lcxmlremlineeditbuilder.h \
basicwidgets/builders/lcxmltextlabelbuilder.h \
basicwidgets/builders/lcxmlwidgetbuilder.h \
basicwidgets/builders/lcxmlremcomboboxbuilder.h \
basicwidgets/builders/lcxmlremcombolabelbuilder.h \
basicwidgets/builders/lcxmltablewidgetbuilder.h \
basicwidgets/builders/lcxmltabwidgetbuilder.h \
basicwidgets/builders/lcxmlscrollareabuilder.h \
basicwidgets/builders/lcxmlbuttonbuilder.h \
basicwidgets/builders/lcxmllistwidgetbuilder.h \
basicwidgets/builders/lcbuilderscommon.h \
basicwidgets/builders/lcqwidgetvisiblecontrol.h \
basicwidgets/builders/lcxmlstackedwidgetbuilder.h \
basicwidgets/builders/lcxmlsplitterbuilder.h \
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
common/xmlcommon.h \
xml/xmlbuilders.h \
xml/xmlfonts.h \
xml/xmldataformatters.h \
xml/xmldatasources.h \
xml/xmljscripts.h \
xml/xmlkeyboards.h \
xml/xmlwindows.h \
xml/xmlpluginpathes.h \
keyboard/lckeyboard.h \
window/lcwindow.h \
applicationinterface.h \


