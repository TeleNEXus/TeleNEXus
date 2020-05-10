#include "lcxmlapplication.h"
#include "lcxmlwidgetcreatorsmap.h"
#include "lcxmlremotedatasourcemap.h"
#include "LIApplication.h"
#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmllayoutbuilders.h"
#include "lcxmlwidgetbuilders.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QWidget>

#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>

static QString  sl_xmlMainFileName;
static QString  sl_xmlMainFileWay;
static QDir     sl_xmlMainFileDir;

static const struct
{
    QString xmlMainFileName = "xmlfile";
    QString xmlMainFilePath = "xmlpath";
} __appOptionsName;

//static const struct
//{
//    QString application     = "APPLICATION";
//    QString sourcebuilders  = "SOURCEBUILDERS";
//    QString sources         = "SOURCES";
//    QString modbussources   = "modbussources";
//    QString widgets         = "widgets";
//} __appXmlTags;

//static const struct
//{
//    QString file            = "file";
//}__appXmlCommonTagsAttr;

//======================================================================================================================
class CApplicationInterface : public LIApplication
{
public:
    CApplicationInterface(){}
    virtual QString getProjectPath() const override { return sl_xmlMainFileWay;}
    virtual QDir getProjectDir() const override {return sl_xmlMainFileDir;}

    virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> getDataSourceBuilder(const QString& _name) const override
    {
        return LCXmlRemoteDataSourceBuilders::instance().getBuilder(_name);
    }

    QSharedPointer<LIRemoteDataSource> getDataSource(const QString& _name) const override
    {
        return LCXmlRemoteDataSourceMap::instace().getRemoteDataSorce(_name);
    }

    QSharedPointer<LIXmlLayoutBuilder> getLayoutBuilder(const QString& _name) const override
    {
        return LCXmlLayoutBuilders::instance().getBuilder(_name);
    }

    QSharedPointer<LIXmlWidgetBuilder> getWidgetBuilder(const QString& _name) const override
    {
        return LCXmlWidgetBuilders::instance().getBuilder(_name);
    }
};

static CApplicationInterface sl_appInterface;

//======================================================================================================================
const LCXmlApplication::SBaseTagsNames      LCXmlApplication::mBaseTagNames;
const LCXmlApplication::SBaseAttributeNames LCXmlApplication::mBaseAttributeNames;

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
    return sl_xmlMainFileName;
}

//----------------------------------------------------------------------------------------------------------------------
const QString& LCXmlApplication::getXmlMainFileWay()
{
    return sl_xmlMainFileWay;
}

//----------------------------------------------------------------------------------------------------------------------
const QDir& LCXmlApplication::getXmlMainFileDir()
{
    return sl_xmlMainFileDir;
}

//----------------------------------------------------------------------------------------------------------------------
const QString& LCXmlApplication::getApplicationDirPath()
{
    static QString path = QCoreApplication::applicationDirPath();
    return path;
}


//----------------------------------------------------------------------------------------------------------------------
static void addSources(const QDomElement& _element);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWidgets(const QDomElement& _rootElement);
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
            sl_xmlMainFileName    = fi.fileName();
            sl_xmlMainFileWay     = fi.absolutePath() + "/";
            sl_xmlMainFileDir     = fi.absoluteDir();
        }
        else
        {
            qDebug() << "Main XML file not found";
            qDebug() << "File location setting: " << path + "/" + file;
            parser.showHelp(-1);
        }
    }


    QFile file(sl_xmlMainFileWay + sl_xmlMainFileName);

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Application: parse error at line:" << errorLine <<
                        " column:" << errorColumn << " msg: " << errorStr;
        qDebug() << "Exit programm";
        return -1;
    }

    QDomElement rootElement = domDoc.documentElement();

    if(rootElement.tagName() != mBaseTagNames.rootTag)
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

    //----------------------------------------------------
    addSources(rootElement);
    //----------------------------------------------------
    addLayoutsBuilders(rootElement);
    //----------------------------------------------------
    addWidgetsBuilders(rootElement);
    //----------------------------------------------------
    addWidgets(rootElement);

    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){
            qDebug() << "QApplication::aboutToQuit";
    });

    return app.exec();
}


//======================================================================================================================
static void addSources(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.sourceBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlRemoteDataSourceBuilders::instance().load(nodes.at(0).toElement(), sl_xmlMainFileWay, sl_xmlMainFileWay);
    }
    else
    {
        return;
    }

    //Добавление источников данных.
    QDomNode node = _rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.sources).at(0).firstChild();

    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement el = node.toElement();
            QSharedPointer<LIXmlRemoteDataSourceBuilder> builder =
                    LCXmlRemoteDataSourceBuilders::instance().getBuilder(el.tagName());
            if(!builder.isNull())
            {
                LQDataSources sources = builder->build(el, sl_appInterface);
                LCXmlRemoteDataSourceMap::instace().addRemoteDataSorce(sources);
            }
        }
        node = node.nextSibling();
    }
}

//======================================================================================================================
static void addLayoutsBuilders(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.layoutBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlLayoutBuilders::instance().load(nodes.at(0).toElement(), sl_xmlMainFileWay, sl_xmlMainFileWay);
    }
}

//======================================================================================================================
static void addWidgetsBuilders(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.widgetBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlWidgetBuilders::instance().load(nodes.at(0).toElement(), sl_xmlMainFileWay, sl_xmlMainFileWay);
    }
}

//======================================================================================================================
static void addWidgets(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomElement element = _rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.widgets).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(LCXmlApplication::mBaseAttributeNames.file);

    if(!attrFile.isNull())
    {
        QFile file(sl_xmlMainFileWay + attrFile);

        QDomDocument domDoc;
        QString errorStr;
        int errorLine;
        int errorColumn;

        if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
        {
            qDebug() << "Parse file "<< file.fileName() << " error at line:" << errorLine <<
                            " column:" << errorColumn << " msg: " << errorStr;
            return;
        }

        element = domDoc.documentElement();

        if(element.tagName() != LCXmlApplication::mBaseTagNames.widgets)
        {
            return;
        }
    }

    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            auto element = node.toElement();
            auto builder = LCXmlWidgetBuilders::instance().getBuilder(element.tagName());
            if(!builder.isNull())
            {
                auto widget = builder->build(element, sl_appInterface);
                if(widget)
                {
                    widget->show();
                }
                break;
            }
        }
        node = node.nextSibling();
    }
}

