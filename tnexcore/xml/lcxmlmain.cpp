#include "lcxmlmain.h"
#include "lcxmlcommon.h"

#include "LIApplication.h"
#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmllayoutbuilders.h"
#include "lcxmlwidgetbuilders.h"
#include "lcxmlwindows.h"
#include "lcxmlfonts.h"
#include "lcxmljsscripts.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QWidget>

#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>

#include <QStringList>
#include <QThread>
#include <QTimer>
#include "LIRemoteDataReadListener.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"

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
static QDomDocument loadDomElement(const QString& _fileName);
static void addPlaginLibPathes(const QDomElement& _rootElement);
static void addSourceBuilders(const QDomElement& _rootElement);
static void addSources(const QDomElement& _element);
static void addFonts(const QDomElement& _rootElement);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWindows(const QDomElement& _rootElement);
static void addScripts(const QDomElement& _rootElement);

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
    return loadDomElement( _fileName);
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
LCXmlMain::LCXmlMain()
{

}

//------------------------------------------------------------------------------
LCXmlMain& LCXmlMain::instance()
{
    static LCXmlMain inst;
    return inst;
}

//------------------------------------------------------------------------------
const LIApplication& LCXmlMain::getApplicationInterface()
{
  return __slAppInterface;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class CDataListener: public LIRemoteDataReadListener
{
public:
    CDataListener(){}
    virtual ~CDataListener(){}
    virtual void dataIsRead(
            QSharedPointer<QByteArray> _data, 
            LERemoteDataStatus status) 
    {
      qDebug() << "CDataListener Data is Read : data = " << *_data.data() << " status = " << (quint16)status;
    }
};

static void sharedDeleter(CDataListener* _listener)
{
  qDebug() << "sharedDeleter";
  delete _listener;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int LCXmlMain::exec(int argc, char *argv[])
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

    if(rootElement.tagName() != LCXmlCommon::mBaseTags.rootTag)
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
    //----------------------------------------------------
    addScripts(rootElement);


    QObject::connect(&app, &QApplication::aboutToQuit,
                     [&](){
            qDebug() << "QApplication::aboutToQuit";
    });




    /* QThread* thr = new QThread; */
    /* QTimer* tmr1 = new QTimer; */

    /* tmr1->moveToThread(thr); */

    /* QObject::connect(thr, &QThread::started, */ 
    /*     [tmr1]() */
    /*     { */
    /*       qDebug() << "Start Timer --------"; */
    /*       tmr1->start(500); */
    /*     }); */


    /* auto source = __slAppInterface.getDataSource("localsource"); */
    /* QSharedPointer<CDataListener> listener(new CDataListener, sharedDeleter); */
    /* QSharedPointer<LIRemoteDataReader> reader; */
    /* if(!source.isNull()) */
    /* { */
    /*   qDebug() << "Timer: source is NULL"; */
    /*   reader = source->createReader("bytes_uint16", listener); */
    /* } */

    /* QObject::connect(tmr1, &QTimer::timeout, */ 
    /*     [source, reader]() */
    /*     { */
    /*       qDebug() << "Timeout--------"; */
    /*       if(source.isNull()) */
    /*       { */
    /*         qDebug() << "Timer: source is NULL"; */
    /*         return; */
    /*       } */
    /*       qDebug() << "Timer: get source"; */
    /*       if(!reader.isNull()) reader->readRequest(); */
    /*     }); */

    /* /1* QObject::connect(tmr1, &QTimer::timeout, *1/ */ 
    /* /1*     []() *1/ */
    /* /1*     { *1/ */
    /* /1*       qDebug() << "Timeout--------"; *1/ */
    /* /1*       /2* auto source = __slAppInterface.getDataSource("localsource"); *2/ *1/ */
    /* /1*       auto source = __slAppInterface.getDataSource("qtslavetcp"); *1/ */
    /* /1*       if(source.isNull()) *1/ */
    /* /1*       { *1/ */
    /* /1*         qDebug() << "Timer: source is NULL"; *1/ */
    /* /1*         return; *1/ */
    /* /1*       } *1/ */
    /* /1*       qDebug() << "Timer: get source"; *1/ */

    /* /1*       QSharedPointer<CDataListener> listener(new CDataListener, sharedDeleter); *1/ */
    /* /1*       /2* auto reader = source->createReader("bytes_uint16", listener); *2/ *1/ */
    /* /1*       auto reader = source->createReader("hreg0", listener); *1/ */
    /* /1*       if(!reader.isNull()) *1/ */ 
    /* /1*       { *1/ */
    /* /1*         qDebug() << "Read Request"; *1/ */
    /* /1*         reader->readRequest(); *1/ */
    /* /1*       } *1/ */
    /* /1*     }); *1/ */

/*     thr->start(); */


    return app.exec();
}

//==============================================================================
static void addPlaginLibPathes(const QDomElement& _rootElement)
{
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlCommon::mBaseTags.plaginpath);

    if(!nodes.isEmpty())
    {
        nodes = nodes.at(0).toElement().elementsByTagName(
                LCXmlCommon::mCommonTags.item);
        if(!nodes.isEmpty())
        {
            for(int i = 0; i < nodes.size(); i++)
            {

                QString path = nodes.at(i).toElement().attribute(
                        LCXmlCommon::mCommonAttributes.path);
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
            LCXmlCommon::mBaseTags.sourceBuilders);

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
            LCXmlCommon::mBaseTags.sources).at(0).toElement();

    if(element.isNull()) return;

    QString attrFile = element.attribute(
            LCXmlCommon::mCommonAttributes.file);

    if(!attrFile.isNull())
    {
        element = loadDomElement(attrFile).documentElement();

        if(element.tagName() != LCXmlCommon::mBaseTags.sources)
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
      LCXmlCommon::mBaseTags.fonts);
  LCXmlFonts::instance().create(el, __slAppInterface);
}

//==============================================================================
static void addLayoutsBuilders(const QDomElement& _rootElement)
{
    //Загрузка построителей источников данных.
    QDomNodeList nodes = _rootElement.elementsByTagName(
            LCXmlCommon::mBaseTags.layoutBuilders);

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
            LCXmlCommon::mBaseTags.widgetBuilders);

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
                LCXmlCommon::mBaseTags.window); 

            !node.isNull(); 

            node = node.nextSiblingElement(
                LCXmlCommon::mBaseTags.window))
    {
        QDomElement el = node.toElement();

        if(el.isNull()) continue;
        LCXmlWindows::instance().create(el, __slAppInterface);
    }
}

//==============================================================================
static void addScripts(const QDomElement& _rootElement)
{
  LCXmlJScripts::instance().load(
      _rootElement.firstChildElement(LCXmlCommon::mBaseTags.scripts), 
      __slAppInterface);
}

//==============================================================================
static QDomDocument loadDomElement(const QString& _fileName)
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
