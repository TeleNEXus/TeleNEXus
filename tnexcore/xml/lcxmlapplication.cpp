#include "lcxmlapplication.h"

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

#include <QStringList>

static QString  sl_xmlMainFileName;
static QString  sl_xmlMainFilePath;
static QDir     sl_xmlMainFileDir;

static QStringList sl_PlaginLibPaths;

static const struct
{
    QString xmlMainFileName = "xmlfile";
    QString xmlMainFilePath = "xmlpath";
} __appOptionsName;

using TQRemoteDataSourceMap = QMap<QString, QSharedPointer<LIRemoteDataSource>>;
TQRemoteDataSourceMap sl_RemoteDataSourceMap;

//------------------------------------------------------------------------------
static QDomDocument staticLoadDomElement(const QString& _fileName);
static void addPlaginLibPathes(const QDomElement& _rootElement);
static void addSourceBuilders(const QDomElement& _rootElement);
static void addSources(const QDomElement& _element);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWidgets(const QDomElement& _rootElement);

//==============================================================================
class CApplicationInterface : public LIApplication
{
public:
    CApplicationInterface(){}
    virtual QString getProjectPath() const override { 
        return sl_xmlMainFilePath;}
    virtual QDir getProjectDir() const override {return sl_xmlMainFileDir;}

    virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
        getDataSourceBuilder(
            const QString& _name) const override
    {
        return LCXmlRemoteDataSourceBuilders::instance().getBuilder(_name);
    }

    QSharedPointer<LIRemoteDataSource> 
        getDataSource(const QString& _name) const override
    {
        TQRemoteDataSourceMap::iterator it = sl_RemoteDataSourceMap.find(_name);
        if(it == sl_RemoteDataSourceMap.end()) return nullptr;
        return it.value();
    }

    QSharedPointer<LIXmlLayoutBuilder> 
        getLayoutBuilder(const QString& _name) const override
    {
        return LCXmlLayoutBuilders::instance().getBuilder(_name);
    }

    QSharedPointer<LIXmlWidgetBuilder> 
        getWidgetBuilder(const QString& _name) const override
    {
        return LCXmlWidgetBuilders::instance().getBuilder(_name);
    }

    virtual QDomDocument getDomDocument(const QString& _fileName) const override
    {
        return staticLoadDomElement( _fileName);
    }
};

static CApplicationInterface sl_appInterface;

//==============================================================================
const LCXmlApplication::SBaseTagsNames      LCXmlApplication::mBaseTagNames;
const LCXmlApplication::SBaseAttributeNames LCXmlApplication::mBaseAttributeNames;
//------------------------------------------------------------------------------
LCXmlApplication::LCXmlApplication()
{

}

//------------------------------------------------------------------------------
LCXmlApplication& LCXmlApplication::instance()
{
    static LCXmlApplication inst;
    return inst;
}

//------------------------------------------------------------------------------
const QString& LCXmlApplication::getXmlMainFileName()
{
    return sl_xmlMainFileName;
}

//------------------------------------------------------------------------------
const QString& LCXmlApplication::getXmlMainFileWay()
{
    return sl_xmlMainFilePath;
}

//------------------------------------------------------------------------------
const QDir& LCXmlApplication::getXmlMainFileDir()
{
    return sl_xmlMainFileDir;
}

//------------------------------------------------------------------------------
const QString& LCXmlApplication::getApplicationDirPath()
{
    static QString path = QCoreApplication::applicationDirPath();
    return path;
}

//------------------------------------------------------------------------------
int LCXmlApplication::exec(int argc, char *argv[])
{

    QApplication app(argc, argv);
    qDebug() << QCoreApplication::applicationFilePath();
    qDebug() << QCoreApplication::applicationDirPath();

    auto lp = QCoreApplication::libraryPaths();
    auto it = lp.begin();
    while(it != lp.end())
    {
        qDebug() << "libraryPaths: " << *it;
        it++;
    }

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
            sl_xmlMainFilePath    = fi.absolutePath() + "/";
            sl_xmlMainFileDir     = fi.absoluteDir();
        }
        else
        {
            qDebug() << "Main XML file not found";
            qDebug() << "File location setting: " << path + "/" + file;
            parser.showHelp(-1);
        }
    }


    QFile file(sl_xmlMainFilePath + sl_xmlMainFileName);

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

    addPlaginLibPathes(rootElement);
    //----------------------------------------------------
    addSourceBuilders(rootElement);
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


//==============================================================================
static void addPlaginLibPathes(const QDomElement& _rootElement)
{
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.plaginPaths);

    if(!nodes.isEmpty())
    {
        nodes = nodes.at(0).toElement().elementsByTagName(
                LCXmlApplication::mBaseTagNames.add);
        if(!nodes.isEmpty())
        {
            for(int i = 0; i < nodes.size(); i++)
            {

                QString path = nodes.at(i).toElement().attribute(
                        LCXmlApplication::mBaseAttributeNames.path);
                if(path.isNull()) continue;
                QDir dir(path);
                if(dir.isAbsolute())
                {
                    sl_PlaginLibPaths << dir.path();
                    continue;
                }

                path = QDir::currentPath();
                QDir::setCurrent(sl_xmlMainFilePath);
                sl_PlaginLibPaths << dir.absolutePath();
                QDir::setCurrent(path);
            }
        }
    }

    sl_PlaginLibPaths << 
        QCoreApplication::applicationDirPath() + 
        QStringLiteral("/plaginlibs");

    qDebug() << "PlaginLib paths:";
    for(int i = 0; i < sl_PlaginLibPaths.size(); i++)
    {
        qDebug() << "\t\t" << sl_PlaginLibPaths.at(i);
    }

}

//==============================================================================
static void addSourceBuilders(const QDomElement& _rootElement)
{
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.sourceBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlRemoteDataSourceBuilders::instance().load(
                nodes.at(0).toElement(), sl_xmlMainFilePath, sl_PlaginLibPaths);
    }
}

//==============================================================================
static void addSources(const QDomElement& _rootElement)
{
    if(LCXmlRemoteDataSourceBuilders::instance().noItems()) return;

    QDomElement element = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.sources).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(
            LCXmlApplication::mBaseAttributeNames.file);

    if(!attrFile.isNull())
    {
        element = staticLoadDomElement(attrFile).documentElement();

        if(element.tagName() != LCXmlApplication::mBaseTagNames.sources)
        {
            return;
        }
    }

    //Добавление источников данных.
    QDomNode node = element.firstChild();

    while(!node.isNull())
    {
        QDomElement el = node.toElement();
        auto builder = LCXmlRemoteDataSourceBuilders::instance().getBuilder(
                el.tagName());

        if(!builder.isNull())
        {
            auto sources = builder->build(el, sl_appInterface);
            auto it = sources.begin();
            while(it != sources.end())
            {
                if(sl_RemoteDataSourceMap.find(it.key()) != sources.end())
                {
                    sl_RemoteDataSourceMap.insert(it.key(), it.value());
                }
                it++;
            }
        }
        node = node.nextSibling();
    }
}

//==============================================================================
static void addLayoutsBuilders(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.layoutBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlLayoutBuilders::instance().load(nodes.at(0).toElement(), 
                sl_xmlMainFilePath, sl_PlaginLibPaths);
    }
}

//==============================================================================
static void addWidgetsBuilders(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.widgetBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlWidgetBuilders::instance().load(nodes.at(0).toElement(), 
                sl_xmlMainFilePath, sl_PlaginLibPaths);
    }
}

//==============================================================================
static void addWidgets(const QDomElement& _rootElement)
{

    QDomElement element = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTagNames.widgets).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(
            LCXmlApplication::mBaseAttributeNames.file);

    if(!attrFile.isNull())
    {
        element = staticLoadDomElement(attrFile).documentElement();

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
            auto builder = 
                LCXmlWidgetBuilders::instance().getBuilder(element.tagName());
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

//==============================================================================
static QDomDocument staticLoadDomElement(const QString& _fileName)
{
    QFile file(sl_xmlMainFilePath + _fileName);

    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << 
            "Parse file "       << file.fileName() << 
            " error at line:"   << errorLine <<
            " column:"          << errorColumn << 
            " msg: "            << errorStr;
    }
    return domDoc;
}
