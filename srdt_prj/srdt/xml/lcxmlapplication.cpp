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
    QString application     = "APPLICATION";
    QString sourcebuilders  = "SOURCEBUILDERS";
    QString sources         = "SOURCES";
    QString modbussources   = "modbussources";
    QString widgets         = "widgets";
} __appXmlTags;

static const struct
{
    QString file            = "file";
}__appXmlCommonTagsAttr;

//======================================================================================================================
class CApplicationInterface : public LIApplication
{
public:
    CApplicationInterface(){}
    virtual QString getProjectPath() const override { return __xmlMainFileWay;}
    virtual QDir getProjectDir() const override {return __xmlMainFileDir;}

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

static CApplicationInterface __appInterface;

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

//----------------------------------------------------------------------------------------------------------------------
const QString& LCXmlApplication::getApplicationDirPath()
{
    static QString path = QCoreApplication::applicationDirPath();
    return path;
}


//----------------------------------------------------------------------------------------------------------------------
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


    QFile file(__xmlMainFileWay + __xmlMainFileName);

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Application: parse error at line:" << errorLine <<
                        " column:" << errorColumn << " msg: " << errorStr;
        qDebug() << "Exit programm";
        return -1;
    }

    QDomElement rootElement = domDoc.documentElement();

    if(rootElement.tagName() != __appXmlTags.application)
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


    QDomNodeList nodes = rootElement.elementsByTagName(__appXmlTags.sourcebuilders);
    if(!nodes.isEmpty())
    {
        LCXmlRemoteDataSourceBuilders::instance().load(nodes.at(0).toElement(), __xmlMainFileWay, __xmlMainFileWay);
    }

    nodes = rootElement.elementsByTagName(__appXmlTags.sources);
    QDomNode node = nodes.at(0).toElement().firstChild();

    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement el = node.toElement();
            QSharedPointer<LIXmlRemoteDataSourceBuilder> builder =
                    LCXmlRemoteDataSourceBuilders::instance().getBuilder(el.tagName());
            if(!builder.isNull())
            {
                LQDataSources sources = builder->create(el, __appInterface);
                LCXmlRemoteDataSourceMap::instace().addRemoteDataSorce(sources);
            }
        }
        node = node.nextSibling();
    }

    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){
            qDebug() << "QApplication::aboutToQuit";
    });

    return app.exec();
}
