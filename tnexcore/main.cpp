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

#include "version.h"
#include "applicationinterface.h"
#include "xmlbuilders.h"
#include "xmldatasources.h"
#include "xmlwindows.h"
#include "xmldataformatters.h"
#include "xmljscripts.h"
#include "xmlkeyboards.h"
#include "xmlpluginpathes.h"
#include "xmlwidgetstyles.h"
#include "xmlglobalstyle.h"

#include <QLoggingCategory>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDomElement>
#include <QDir>
#include <QCommandLineParser>



#define __lmVersionString (QString("TeleNEXus version: %1").arg(APPLICATION_VERSION))

static const QString __slRootTag = "APPLICATION";

//==============================================================================
static const struct
{
  QString xmlMainFileName = "xmlfile";
  QString xmlMainFilePath = "xmlpath";
  QString version         = "version";
} __slAppOptionsName;

//==============================================================================
struct SParameters
{
  QString defaultPluginsPath;
  QString mainFileName;
  QString projectPath;
  QDir    projectDir;
  bool parseCommandLine(char *argv[])
  {
    defaultPluginsPath = QString("%1/%2")
      .arg(QDir::currentPath())
      .arg(QStringLiteral("plugins"));

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

    QCommandLineOption version(__slAppOptionsName.version,
        QStringLiteral("Version"),
        __slAppOptionsName.version
        );

    parser.addOption(fileName);
    parser.addOption(pathName);
    parser.addOption(version);

    parser.parse(QApplication::arguments());

    qDebug("%s", qPrintable(__lmVersionString));
    if(parser.isSet(__slAppOptionsName.version))
    {
      return false;
    }

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
      }
      else
      {
        parser.showHelp(-1);
      }
    }
    return true;
  }
}__slParameters;




#include <csignal>
//------------------------------------------------------------------------------
static void appExit(int _sig)
{
  Q_UNUSED(_sig);
  QCoreApplication::exit(0);
}

//==============================================================================main
int main(int argc, char *argv[])
{

  signal(SIGINT, appExit);
  signal(SIGTERM, appExit);


  QLoggingCategory::setFilterRules("qt5ct.debug=false");

  const LIApplication& appinterface = CApplicationInterface::getInstance();

  QApplication app(argc, argv);

  if(!__slParameters.parseCommandLine(argv)) return 0;

  appinterface.message(QString("Project current path:\n\t%1").arg(QDir::currentPath()));
  appinterface.message(QString("Project main file:\n\t%1").arg(__slParameters.mainFileName));

  {
    QString str = QStringLiteral("Default plugins pathes: \n");
    auto lp = QCoreApplication::libraryPaths();
    auto it = lp.begin();

    while(it != lp.end())
    {
      str += QString("\t%1\n").arg(*it); 
      it++;
    }

    appinterface.message(str);
  }

  CApplicationInterface::getInstance().setParameters(
      __slParameters.mainFileName,
      __slParameters.projectPath,
      __slParameters.projectDir);


  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;



  appinterface.message(QStringLiteral("Project deploy begining..."));



  QFile file(__slParameters.projectPath + __slParameters.mainFileName);

  if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
  {
    appinterface.message(
        QString("Application: parse error at line: %1 column: %2 msg: #3")
       .arg(errorLine)
       .arg(errorColumn)
       .arg(errorStr));
    appinterface.message("Exit programm");
    return -1;
  }

  QDomElement rootElement = domDoc.documentElement();

  if(rootElement.tagName() != __slRootTag)
  {
    appinterface.message("Application: wrong root element");
    appinterface.message("Exit programm");
    return -1;
  }

  QDomNode childNode = rootElement.firstChild();

  if(childNode.isNull())
  {
    appinterface.message("Application: no child elements");
    appinterface.message("Exit programm");
    return -1;
  }

  //----------------------------------------------------
  xmlpluginpathes::upload(rootElement,
      __slParameters.defaultPluginsPath);
  //----------------------------------------------------
  xmlglobalstyle::upload(rootElement, app);
  //----------------------------------------------------
  builders::sources::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  xmldatasources::upload(rootElement);
  //----------------------------------------------------
  xmlwidgetstyles::upload(rootElement);
  
  //----------------------------------------------------
  xmldataformatters::upload(rootElement);
  //----------------------------------------------------
  builders::layouts::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  builders::widgets::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  xmlkeyboards::upload(rootElement);
  //----------------------------------------------------
  xmlwindows::upload(rootElement);
  //----------------------------------------------------
  xmljscripts::upload(rootElement);

  appinterface.message(QStringLiteral("Project deploy end."));

  xmlkeyboards::init();
  xmlwindows::show();

  QObject::connect(&app, &QApplication::aboutToQuit,
      [&](){
        appinterface.message("Quit from TeleNEXus");
      });

  return app.exec();
}


