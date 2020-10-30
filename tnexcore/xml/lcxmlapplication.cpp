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
static void addWindows(const QDomElement& _rootElement);

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
const LCXmlApplication::SBaseTags LCXmlApplication::mBaseTags;
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

    if(rootElement.tagName() != mBaseTags.rootTag)
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
    addWindows(rootElement);


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
            LCXmlApplication::mBaseTags.plaginPaths.name);

    if(!nodes.isEmpty())
    {
        nodes = nodes.at(0).toElement().elementsByTagName(
                LCXmlApplication::mBaseTags.add);
        if(!nodes.isEmpty())
        {
            for(int i = 0; i < nodes.size(); i++)
            {

                QString path = nodes.at(i).toElement().attribute(
                        LCXmlApplication::mBaseTags.plaginPaths.attrs.path);
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
            LCXmlApplication::mBaseTags.sourceBuilders);

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
            LCXmlApplication::mBaseTags.sources.name).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(
            LCXmlApplication::mBaseTags.sources.attrs.file);

    if(!attrFile.isNull())
    {
        element = staticLoadDomElement(attrFile).documentElement();

        if(element.tagName() != LCXmlApplication::mBaseTags.sources.name)
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
            LCXmlApplication::mBaseTags.layoutBuilders);

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
            LCXmlApplication::mBaseTags.widgetBuilders);

    if(!nodes.isEmpty())
    {
        LCXmlWidgetBuilders::instance().load(nodes.at(0).toElement(), 
                sl_xmlMainFilePath, sl_PlaginLibPaths);
    }
}

//==============================================================================
static void createWindow(const QDomElement &_element, 
        const QString& _title, const QString& _id,
        const QString& _show);

static void addWindows(const QDomElement& _rootElement)
{
    
    for(auto node = 
            _rootElement.firstChildElement(
                LCXmlApplication::mBaseTags.window.name); 

            !node.isNull(); 

            node = node.nextSiblingElement(
                LCXmlApplication::mBaseTags.window.name))
    {
        QDomElement el = node.toElement();
        QString attr_title = 
            el.attribute(LCXmlApplication::mBaseTags.window.attrs.title);
        QString attr_id = 
            el.attribute(LCXmlApplication::mBaseTags.window.attrs.id);
        QString attr_file = 
            el.attribute(LCXmlApplication::mBaseTags.window.attrs.file);
        QString attr_show = 
            el.attribute(LCXmlApplication::mBaseTags.window.attrs.show.name);
        
        if(!attr_file.isNull())
        {
            el = staticLoadDomElement(attr_file).documentElement();

            if(el.tagName() != LCXmlApplication::mBaseTags.window.name)
            {
                return;
            }
        }
        createWindow(el, attr_title, attr_id, attr_show);
    }
}

//==============================================================================
static void createWindow(const QDomElement &_element, 
        const QString& _title, const QString& _id, 
        const QString& _show)
{
    for(auto node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSibling())
    {
        if(node.isElement())
        {
            auto el = node.toElement();
            auto builder = 
                LCXmlWidgetBuilders::instance().getBuilder(el.tagName());
            if(!builder.isNull())
            {
                auto widget = builder->build(el, sl_appInterface);
                if(widget)
                {
                    QString title;
                    if(!_title.isNull())
                    {
                        title = _title;
                    }
                    else if(!_id.isNull())
                    {
                        title = _id;
                    }
                    else
                    {
                        title = el.tagName();
                    }

                    widget->setWindowTitle(title);

                    if(_show == 
                            LCXmlApplication::
                            mBaseTags.window.attrs.show.vals.yes)
                    {
                        widget->show();
                    }
                }
                break;
            }
        }
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
