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
#include "xmlcommon.h"

/* #include "LIApplication.h" */

#include "applicationinterface.h"

#include "LIXmlRemoteDataSourceBuilder.h"
#include "LIXmlWidgetBuilder.h"
#include "LIXmlLayoutBuilder.h"
#include "xmlfonts.h"

#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"
#include "xmlbuilders.h"
#include "lcxmlformatterfactory.h"
#include "xmldatasources.h"
#include "xmlwindows.h"
#include "xmldataformatters.h"
#include "xmljscripts.h"
#include "xmlkeyboards.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QWidget>
#include <QDomElement>

#include <QFileInfo>
#include <QDir>
#include <QCommandLineParser>

#include <QStringList>
#include <QThread>
#include <QTimer>

static const QString __slRootTag = "APPLICATION";


static QStringList  __slPlaginLibPaths;

static const struct
{
  QString xmlMainFileName = "xmlfile";
  QString xmlMainFilePath = "xmlpath";
} __slAppOptionsName;

struct SParameters
{
  QString mainFileName;
  QString projectPath;
  QDir    projectDir;
  void parseCommandLine(char *argv[])
  {
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
        mainFileName = fi.fileName();
        projectPath  = fi.absolutePath() + "/";
        projectDir   = fi.absoluteDir();
        QDir::setCurrent(path);
        qDebug() << "Project current path = " << QDir::currentPath();
      }
      else
      {
        qDebug() << "Main XML file not found";
        qDebug() << "File location setting: " << path + "/" + file;
        parser.showHelp(-1);
      }
    }
  }
}__slParameters;

//------------------------------------------------------------------------------
static void addPlaginLibPathes(const QDomElement& _rootElement);
static void addSourceBuilders(const QDomElement& _rootElement);
static void addSources(const QDomElement& _element);
static void addFonts(const QDomElement& _rootElement);
static void addFormatters(const QDomElement& _rootElement);
static void addLayoutsBuilders(const QDomElement& _rootElement);
static void addWidgetsBuilders(const QDomElement& _rootElement);
static void addWindows(const QDomElement& _rootElement);
static void addKeyboards(const QDomElement& _rootElement);
static void addScripts(const QDomElement& _rootElement);

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

  __slParameters.parseCommandLine(argv);

  CApplicationInterface::getInstance().setParameters(
      __slParameters.mainFileName,
      __slParameters.projectPath,
      __slParameters.projectDir);


  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;



  QFile file(__slParameters.projectPath + __slParameters.mainFileName);

  if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
  {
    qDebug() << "Application: parse error at line:" << errorLine <<
      " column:" << errorColumn << " msg: " << errorStr;
    qDebug() << "Exit programm";
    return -1;
  }

  QDomElement rootElement = domDoc.documentElement();

  if(rootElement.tagName() != __slRootTag)
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
  addKeyboards(rootElement);
  //----------------------------------------------------
  addWindows(rootElement);
  //----------------------------------------------------
  addScripts(rootElement);

  //----------------------------------------------------
  xmlkeyboards::init();

  xmlwindows::show();


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
      xmlcommon::mBaseTags.plaginpath);

  if(!nodes.isEmpty())
  {
    nodes = nodes.at(0).toElement().elementsByTagName(
        xmlcommon::mCommonTags.item);
    if(!nodes.isEmpty())
    {
      for(int i = 0; i < nodes.size(); i++)
      {

        QString path = nodes.at(i).toElement().attribute(
            xmlcommon::mCommonAttributes.path);
        if(path.isNull()) continue;
        QDir dir(path);
        if(dir.isAbsolute())
        {
          __slPlaginLibPaths << dir.path();
          continue;
        }

        path = QDir::currentPath();
        QDir::setCurrent(__slParameters.projectPath);
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
  builders::sources::upload( 
      _rootElement, __slParameters.projectPath, __slPlaginLibPaths);
}

//==============================================================================
static void addSources(const QDomElement& _rootElement)
{
  xmldatasources::upload(_rootElement);
}

//==============================================================================
static void addFonts(const QDomElement& _rootElement)
{
  xmlfonts::upload(_rootElement);
}

//==============================================================================
static void addFormatters(const QDomElement& _rootElement)
{
  xmldataformatters::upload(_rootElement);
}

//==============================================================================
static void addLayoutsBuilders(const QDomElement& _rootElement)
{
  builders::layouts::upload( _rootElement, __slParameters.projectPath, __slPlaginLibPaths);
}

//==============================================================================
static void addWidgetsBuilders(const QDomElement& _rootElement)
{
  builders::widgets::upload( _rootElement, __slParameters.projectPath, __slPlaginLibPaths);
}

//==============================================================================
static void addWindows(const QDomElement& _rootElement)
{
  xmlwindows::upload(_rootElement);
}

//==============================================================================
static void addKeyboards(const QDomElement& _rootElement)
{
  xmlkeyboards::upload(_rootElement);
}

//==============================================================================
static void addScripts(const QDomElement& _rootElement)
{
  xmljscripts::upload(_rootElement);
}

