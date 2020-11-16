#include "lcxmlapplication.h"

#include "LIApplication.h"
#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmllayoutbuilders.h"
#include "lcxmlwidgetbuilders.h"
#include "lcxmlwindows.h"
#include "lcxmlfonts.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QWidget>

#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>

#include <QStringList>

static QString      __slXmlMainFileName;
static QString      __slXmlMainFilePath;
static QDir         __slXmlMainFileDir;

static QStringList  __slPlaginLibPaths;

static const struct
{
    QString xmlMainFileName = "xmlfile";
    QString xmlMainFilePath = "xmlpath";
} __slAppOptionsName;

QMap<QString, QSharedPointer<LIRemoteDataSource>> __slRemoteDataSourceMap;

//------------------------------------------------------------------------------
static QDomDocument staticLoadDomElement(const QString& _fileName);
static void addPlaginLibPathes(const QDomElement& _rootElement);
static void addSourceBuilders(const QDomElement& _rootElement);
static void addSources(const QDomElement& _element);
static void addFonts(const QDomElement& _rootElement);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWindows(const QDomElement& _rootElement);

//==============================================================================
class CApplicationInterface : public LIApplication
{
public:
  CApplicationInterface(){}
  virtual QString getProjectPath() const override { 
    return __slXmlMainFilePath;}
  virtual QDir getProjectDir() const override {return __slXmlMainFileDir;}

  virtual QSharedPointer<LIXmlRemoteDataSourceBuilder> 
    getDataSourceBuilder(
        const QString& _name) const override
    {
      return LCXmlRemoteDataSourceBuilders::instance().getBuilder(_name);
    }

  QSharedPointer<LIRemoteDataSource> 
    getDataSource(const QString& _name) const override
    {
      auto it = __slRemoteDataSourceMap.find(_name);
      if(it == __slRemoteDataSourceMap.end()) return nullptr;
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

  virtual QDomDocument getDomDocument(
      const QString& _fileName) const override
  {
    return staticLoadDomElement( _fileName);
  }

  virtual QSharedPointer<LIWindow> getWindow(
      const QString& _windowId) const override
  {
    return LCXmlWindows::instance().getWindow(_windowId);
  }

  virtual QString getFontStyle(const QString& _fontId) const override
  {
    return LCXmlFonts::instance().getFontStyle(_fontId);
  }
};

static CApplicationInterface __slAppInterface;

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

    QCommandLineOption fileName(__slAppOptionsName.xmlMainFileName,
                                QStringLiteral("Main XML filename."),
                                __slAppOptionsName.xmlMainFileName,
                                QStringLiteral("main.xml"));

    QCommandLineOption pathName(__slAppOptionsName.xmlMainFilePath,
                                QStringLiteral("Root path XML files"),
                                __slAppOptionsName.xmlMainFilePath,
                                fi.absolutePath());

    parser.addOption(fileName);
    parser.addOption(pathName);

    parser.parse(QApplication::arguments());


    {
        QString file = parser.value(__slAppOptionsName.xmlMainFileName);
        QString path = parser.value(__slAppOptionsName.xmlMainFilePath);

        QDir dir(path);

        fi.setFile(path + "/" + file);
        if(dir.exists() && fi.exists())
        {
            __slXmlMainFileName    = fi.fileName();
            __slXmlMainFilePath    = fi.absolutePath() + "/";
            __slXmlMainFileDir     = fi.absoluteDir();
        }
        else
        {
            qDebug() << "Main XML file not found";
            qDebug() << "File location setting: " << path + "/" + file;
            parser.showHelp(-1);
        }
    }


    QFile file(__slXmlMainFilePath + __slXmlMainFileName);

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
    addFonts(rootElement);
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
            LCXmlApplication::mBaseTags.plaginPaths.tag);

    if(!nodes.isEmpty())
    {
        nodes = nodes.at(0).toElement().elementsByTagName(
                LCXmlApplication::mBaseTags.plaginPaths.tags.item.tag);
        if(!nodes.isEmpty())
        {
            for(int i = 0; i < nodes.size(); i++)
            {

                QString path = nodes.at(i).toElement().attribute(
                        LCXmlApplication::mBaseTags.plaginPaths.tags.item.attrs.path);
                if(path.isNull()) continue;
                QDir dir(path);
                if(dir.isAbsolute())
                {
                    __slPlaginLibPaths << dir.path();
                    continue;
                }

                path = QDir::currentPath();
                QDir::setCurrent(__slXmlMainFilePath);
                __slPlaginLibPaths << dir.absolutePath();
                QDir::setCurrent(path);
            }
        }
    }

    __slPlaginLibPaths << 
        QCoreApplication::applicationDirPath() + 
        QStringLiteral("/plaginlibs");

    qDebug() << "PlaginLib paths:";
    for(int i = 0; i < __slPlaginLibPaths.size(); i++)
    {
        qDebug() << "\t\t" << __slPlaginLibPaths.at(i);
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
                nodes.at(0).toElement(), __slXmlMainFilePath, __slPlaginLibPaths);
    }
}

//==============================================================================
static void addSources(const QDomElement& _rootElement)
{
    if(LCXmlRemoteDataSourceBuilders::instance().noItems()) return;

    QDomElement element = _rootElement.elementsByTagName(
            LCXmlApplication::mBaseTags.sources.tag).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(
            LCXmlApplication::mBaseTags.sources.attrs.file);

    if(!attrFile.isNull())
    {
        element = staticLoadDomElement(attrFile).documentElement();

        if(element.tagName() != LCXmlApplication::mBaseTags.sources.tag)
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
            auto sources = builder->build(el, __slAppInterface);
            auto it = sources.begin();
            while(it != sources.end())
            {
                if(__slRemoteDataSourceMap.find(it.key()) != sources.end())
                {
                    __slRemoteDataSourceMap.insert(it.key(), it.value());
                }
                it++;
            }
        }
        node = node.nextSibling();
    }
}

//==============================================================================
static void addFonts(const QDomElement& _rootElement)
{
  QDomElement el = _rootElement.firstChildElement(
      LCXmlApplication::mBaseTags.fonts);
  LCXmlFonts::instance().create(el, __slAppInterface);
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
                __slXmlMainFilePath, __slPlaginLibPaths);
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
                __slXmlMainFilePath, __slPlaginLibPaths);
    }
}

//==============================================================================
static void addWindows(const QDomElement& _rootElement)
{
    for(auto node = 
            _rootElement.firstChildElement(
                LCXmlApplication::mBaseTags.window.tag); 

            !node.isNull(); 

            node = node.nextSiblingElement(
                LCXmlApplication::mBaseTags.window.tag))
    {
        QDomElement el = node.toElement();

        if(el.isNull()) continue;
        LCXmlWindows::instance().create(el, __slAppInterface);
    }
}

//==============================================================================
static QDomDocument staticLoadDomElement(const QString& _fileName)
{
    QFile file(__slXmlMainFilePath + _fileName);

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
