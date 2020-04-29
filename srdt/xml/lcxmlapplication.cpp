#include "lcxmlapplication.h"
#include "lcxmlwidgetcreatorsmap.h"

#include "xmlwidgets/lcxmlwidget.h"
#include "xmlwidgets/lcxmllabel.h"
#include "xmlwidgets/lcxmlremlabel.h"
#include "xmlwidgets/lcxmlremlineedit.h"
#include "lcxmlremotedatasourcemap.h"
#include "xmldatasources/lcxmlmodbussources.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QWidget>

#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>

static QString  __xmlMainFileName;
static QString  __xmlMainFileWay;
static QDir     __xmlMainFileDir;

static const struct
{
    QString xmlMainFileName = "xmlfile";
    QString xmlMainFilePath = "xmlpath";
} __appOptionsName;

static const struct
{
    QString root            = "application";
    QString modbussources   = "modbussources";
    QString widgets         = "widgets";
} __appXmlTags;

static const struct
{
    QString file            = "file";
}__appXmlCommotTagsAttr;

//----------------------------------------------------------------------------------------------------------------------
static void initWidgetCreatorsMap();
static bool buildWidgets(QString _fileName);

//======================================================================================================================
LCXmlApplication::LCXmlApplication()
{

}
//----------------------------------------------------------------------------------------------------------------------
LCXmlApplication& LCXmlApplication::instance()
{
    static LCXmlApplication inst;
    return inst;
}
//----------------------------------------------------------------------------------------------------------------------
const QString& LCXmlApplication::getXmlMainFileName()
{
    return __xmlMainFileName;
}

//----------------------------------------------------------------------------------------------------------------------
const QString& LCXmlApplication::getXmlMainFileWay()
{
    return __xmlMainFileWay;
}

//----------------------------------------------------------------------------------------------------------------------
const QDir& LCXmlApplication::getXmlMainFileDir()
{
    return __xmlMainFileDir;
}


int LCXmlApplication::exec(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    QFileInfo fi(argv[0]);

    QCommandLineParser parser;

    QCommandLineOption fileName(__appOptionsName.xmlMainFileName,
                                QStringLiteral("Main XML filename."),
                                __appOptionsName.xmlMainFileName,
                                QStringLiteral("main.xml"));

    QCommandLineOption pathName(__appOptionsName.xmlMainFilePath,
                                QStringLiteral("Root path XML files"),
                                __appOptionsName.xmlMainFilePath,
                                fi.absolutePath());

    parser.addOption(fileName);
    parser.addOption(pathName);

    parser.parse(QApplication::arguments());


    {
        QString file = parser.value(__appOptionsName.xmlMainFileName);
        QString path = parser.value(__appOptionsName.xmlMainFilePath);

        QDir dir(path);

        fi.setFile(path + "/" + file);
        if(dir.exists() && fi.exists())
        {
            __xmlMainFileName    = fi.fileName();
            __xmlMainFileWay     = fi.absolutePath() + "/";
            __xmlMainFileDir     = fi.absoluteDir();
        }
        else
        {
            qDebug() << "Main XML file not found";
            qDebug() << "File location setting: " << path + "/" + file;
            parser.showHelp(-1);
        }
    }

    initWidgetCreatorsMap();

    QFile file(__xmlMainFileWay + __xmlMainFileName);

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Application: parse error at line:" << errorLine <<
                        " column:" << errorColumn << " msg: " << errorStr;
        qDebug() << "Exit programm";
        return -1;
    }

    QDomElement rootElement = domDoc.documentElement();

    if(rootElement.tagName() != __appXmlTags.root)
    {
        qDebug() << "Application: wrong root element";
        qDebug() << "Exit programm";
        return -1;
    }

    QDomNode childNode = rootElement.firstChild();

    if(childNode.isNull())
    {
        qDebug() << "Application: no child elements";
        qDebug() << "Exit programm";
        return -1;
    }

    QDomNodeList modbusSourcesNodes = rootElement.elementsByTagName(__appXmlTags.modbussources);
    if(modbusSourcesNodes.isEmpty())
    {
        qDebug() << "Application: no modbus sources element";
        qDebug() << "Exit programm";
        return -1;
    }

    QString sourcesFile = modbusSourcesNodes.at(0).toElement().attribute(__appXmlCommotTagsAttr.file);

    if(sourcesFile.isNull())
    {
        qDebug() << "Application: no modbus sources file attribute";
        qDebug() << "Exit programm";
        return -1;
    }

    LTDataSources smap = LCXmlModbusSources().create(sourcesFile);

    if(smap.isEmpty())
    {
        qDebug() << "Application: no modbus sources";
    }

    LCXmlRemoteDataSourceMap::instace().addRemoteDataSorce(smap);

    QDomNodeList widgetsNodes = rootElement.elementsByTagName(__appXmlTags.widgets);

    if(widgetsNodes.isEmpty())
    {
        qDebug() << "Application: no widgets file";
        qDebug() << "Exit programm";
        return -1;
    }

    QString strBuff = widgetsNodes.at(0).toElement().attribute(__appXmlCommotTagsAttr.file);

    if(strBuff.isNull())
    {
        qDebug() << "Application: wrong widgets file";
        qDebug() << "Exit programm";
        return -1;
    }

    if(!buildWidgets(strBuff))
    {
        qDebug() << "Exit programm";
        return -1;
    }


    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){
            qDebug() << "QApplication::aboutToQuit";
    });

    return app.exec();
}

//----------------------------------------------------------------------------------------------------------------------
static void initWidgetCreatorsMap()
{
    LCXmlWidgetCreatorInterface *wcr;
    wcr = new xmlwidgetcreators::LCXmlWidget;
    LCXmlWidgetCreatorsMap::instace().addCreator("widget",       wcr);

    wcr = new xmlwidgetcreators::LCXmlLabel;
    LCXmlWidgetCreatorsMap::instace().addCreator("label",        wcr);

    wcr = new xmlwidgetcreators::LCXmlRemLabel;
    LCXmlWidgetCreatorsMap::instace().addCreator("datalabel",    wcr);

    wcr = new xmlwidgetcreators::LCXmlRemLineEdit;
    LCXmlWidgetCreatorsMap::instace().addCreator("datalineedit", wcr);
}

//----------------------------------------------------------------------------------------------------------------------
static bool buildWidgets(QString _fileName)
{
    QFile file(__xmlMainFileWay + _fileName);
    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Widgets: parse error at line:"
                 << errorLine << " column:" << errorColumn << " msg: " << errorStr;
        return false;
    }
    else
    {
        QDomElement rootElement = domDoc.documentElement();

        LCXmlWidgetCreatorInterface* cr = LCXmlWidgetCreatorsMap::instace().getCreator(rootElement.tagName());
        if(cr)
        {
            QWidget* w = cr->create(rootElement);
            if(w)
            {
                w->show();
            }
        }
    }

    return true;
}
