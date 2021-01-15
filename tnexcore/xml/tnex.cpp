/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "tnex.h"
#include "lcxmlcommon.h"

#include "LIApplication.h"
#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmllayoutbuilders.h"
#include "lcxmlwidgetbuilders.h"
#include "lcxmlwindows.h"
#include "lcxmlfonts.h"

#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"
#include "lcxmlformatterfactory.h"
#include "uploaddataformatters.h"
#include "uploadjscripts.h"

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
static void addFormatters(const QDomElement& _rootElement);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWindows(const QDomElement& _rootElement);
static void addScripts(const QDomElement& _rootElement);

//==============================================================================CApplicationInterface
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

  virtual QSharedPointer<LIDataFormatter> 
    getStdDataFormatter(const QString& _name) const override
    {
      return stddataformatterfactory::getFormatter(_name);
    }

  virtual QSharedPointer<LIDataFormatter> 
    getDataFormatter(const QString& _formatterId) const override
    {
      return uploaddataformatters::getDataFormatter(_formatterId);
    }
};

static CApplicationInterface __slAppInterface;


//------------------------------------------------------------------------------
const LIApplication& tnex::getApplicationInterface()
{
  return __slAppInterface;
}

//------------------------------------------------------------------------------
int tnex::exec(int argc, char *argv[])
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
    addFormatters(rootElement);
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

  if(el.isNull()) return;

  LCXmlFonts::instance().create(el, __slAppInterface);
}

//==============================================================================
static void addFormatters(const QDomElement& _rootElement)
{
  QDomElement el = 
    _rootElement.firstChildElement(LCXmlCommon::mBaseTags.formatters);

  if(el.isNull()) return;

  uploaddataformatters::upload(el, __slAppInterface);
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
  uploadjscripts::upload(
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
