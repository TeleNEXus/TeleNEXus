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
#include "xmlsecurity.h"
#include "projectsource.h"

#include <QLoggingCategory>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDomElement>
#include <QDir>
#include <QCommandLineParser>


#define __smQuitMessage QStringLiteral("Quit from TeleNEXus")

#define __lmVersionString (QString("TeleNEXus version: %1").arg(APPLICATION_VERSION))
#define __lmApplicationDescription "TeleNEXus is a simple SCADA"

static const QString __slRootTag = "APPLICATION";

//==============================================================================
static const struct
{
  QString file = "file";
  QString path = "path";
  QString version = "version";
  QString compil = "compil";
  QString target = "target";
} __slAppOptionsName;

//==============================================================================
struct SParameters
{
  QString defaultPluginsPath;
  QString mainFileName;
  QString projectPath;
  QDir    projectDir;

  bool setParameters(const QCommandLineParser& _parser, char** _argv)
  {

    defaultPluginsPath = QString("%1/%2")
      .arg(QApplication::applicationDirPath())
      .arg("plugins");

    QFileInfo fi(_argv[0]);

    /* QCommandLineParser parser; */

    /* QCommandLineOption fileName(__slAppOptionsName.xmlMainFileName, */
    /*     QStringLiteral("Main XML filename."), */
    /*     __slAppOptionsName.xmlMainFileName, */
    /*     QStringLiteral("main.xml")); */

    /* QCommandLineOption pathName(__slAppOptionsName.xmlMainFilePath, */
    /*     QStringLiteral("Root path XML files"), */
    /*     __slAppOptionsName.xmlMainFilePath, */
    /*     fi.absolutePath()); */

    /* QCommandLineOption version(__slAppOptionsName.version, */
    /*     QStringLiteral("Version"), */
    /*     __slAppOptionsName.version */
    /*     ); */

    /* parser.addOption(fileName); */
    /* parser.addOption(pathName); */
    /* parser.addOption(version); */

    /* parser.parse(QApplication::arguments()); */

    /* qDebug("%s", qPrintable(__lmVersionString)); */
    /* if(_parser.isSet(__slAppOptionsName.version)) */
    /* { */
    /*   return false; */
    /* } */

    /* { */

    /*   QString file = _parser.value(__slAppOptionsName.xmlMainFileName); */
    /*   QString path = _parser.value(__slAppOptionsName.xmlMainFilePath); */

    /*   QDir dir(path); */


    /*   fi.setFile(path + "/" + file); */

    /*   if(dir.exists() && fi.exists()) */
    /*   { */
    /*     mainFileName = fi.fileName(); */
    /*     projectPath  = fi.absolutePath() + "/"; */
    /*     projectDir   = fi.absoluteDir(); */
    /*     QDir::setCurrent(path); */
    /*   } */
    /*   else */
    /*   { */
    /*     _parser.showHelp(-1); */
    /*   } */
    /* } */
    return true;
  }
}__slParameters;




#include <csignal>


enum class EParseCommandLineResult
{
  CommandLineError,

  CommandLineCompileRequest,
  CommandLineLaunch,
  CommandLineHelpRequest,
  CommandLineVersionRequest,
  CommandLineIsEmpty
};
//------------------------------------------------------------------------------
static EParseCommandLineResult __s_parseCommandLine(
    QCommandLineParser& _parser, QStringList& _params, QString& _msg);

//------------------------------------------------------------------------------
static int __s_projectDeploy(QApplication& _app);
//------------------------------------------------------------------------------
static bool __s_compilProject(char** _argv);
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

  app.setApplicationName("TeleNEXus");
  app.setApplicationVersion(
      QString("%1").arg(APPLICATION_VERSION));

  QCommandLineParser parser;
  QString msg;
  QStringList params_list;

  QSharedPointer<LIProjectSource> prjsource;


  switch(__s_parseCommandLine(parser, params_list, msg))
  {
  case EParseCommandLineResult::CommandLineError:
    qDebug().noquote() << msg;
    return 0;
  case EParseCommandLineResult::CommandLineIsEmpty:
  case EParseCommandLineResult::CommandLineHelpRequest:
    parser.showHelp();
  case EParseCommandLineResult::CommandLineVersionRequest:
    parser.showVersion();

  /* case EParseCommandLineResult::CommandLineCompileRequest: */
  /* case EParseCommandLineResult::CommandLineLaunchFromProjectRequest: */
  case EParseCommandLineResult::CommandLineLaunch:
    {
      qDebug() << "Launch from path = " << params_list;
      QString launch_param = params_list.at(0);
      prjsource = LCDirProjectSource::create(launch_param, msg);
      qDebug() << msg;
      if(prjsource.isNull()) 
      {
        return -1;
      }

      QFileInfo fi(launch_param);

      if(fi.dir().isAbsolute())
      {
        qDebug() << "Set current dir = " << QDir::currentPath();
        QDir::setCurrent(launch_param);
      }

      qDebug() << 
        QString("Project source from path '%1' is created").arg(launch_param);

      CApplicationInterface::getInstance().setProjectSource(prjsource);

    }
    break;

  default:
    break;
  }

  /* return 0; */

  /* QString default_plugin_path = QString("%1/%2") */
  /*     .arg(QApplication::applicationDirPath()) */
  /*     .arg("plugins"); */

  if(__s_projectDeploy(app) < 0) return -1;


  QObject::connect(&app, &QApplication::aboutToQuit,
      [&](){
        appinterface.message(__smQuitMessage);
      });

  return app.exec();
}

//------------------------------------------------------------------------------
static EParseCommandLineResult 
__s_parseCommandLine(
    QCommandLineParser& _parser, QStringList& _params, QString& _msg)
{

  _parser.setApplicationDescription(__lmApplicationDescription);

  QCommandLineOption file("file", QStringLiteral("Project file"), "file");

  QCommandLineOption launch("launch",
      QStringLiteral("Launch TeleNEXus project"),
      "launch");


  QCommandLineOption compil("compil",
      QStringLiteral("Compil directory path"),
      "compil");

  QCommandLineOption target("target",
      QStringLiteral("Compil target file name"),
      "target");

  QCommandLineOption version = _parser.addVersionOption();

  QCommandLineOption help = _parser.addHelpOption();

  _parser.addOption(file);
  _parser.addOption(launch);
  _parser.addOption(compil);
  _parser.addOption(target);

  if(!_parser.parse(QApplication::arguments()))
  {
    _msg = _parser.errorText();
    return EParseCommandLineResult::CommandLineError;
  }

  if(_parser.isSet(version)) 
  {
    return EParseCommandLineResult::CommandLineVersionRequest;
  }

  if(_parser.isSet(help)) 
  {
    return EParseCommandLineResult::CommandLineHelpRequest;
  }

  if(_parser.optionNames().size() == 0) 
  {
    return EParseCommandLineResult::CommandLineIsEmpty;
  }

  if(_parser.isSet(launch))
  {
    _params << _parser.value(launch);
    return EParseCommandLineResult::CommandLineLaunch;
  }

  return EParseCommandLineResult::CommandLineIsEmpty;
}

//------------------------------------------------------------------------------
static int __s_projectDeploy(QApplication& _app)
{
  const CApplicationInterface& appinterface = 
    CApplicationInterface::getInstance();

  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;

  appinterface.message(QStringLiteral("\n\tBegining deploy of project\n"));


  /* QFile file(__slParameters.projectPath + __slParameters.mainFileName); */

  auto main_file = appinterface.getFileDevice("main.xml");

  if(main_file.isNull()) 
  {
    appinterface.message(__smQuitMessage);
    return -1;
  }

  if(!domDoc.setContent(main_file.data(), true, &errorStr, &errorLine, &errorColumn))
  {
    appinterface.error(
        QString("Application: parse error at line: %1 column: %2 msg: #3")
       .arg(errorLine)
       .arg(errorColumn)
       .arg(errorStr));
    appinterface.message(__smQuitMessage);
    return -1;
  }

  QDomElement rootElement = domDoc.documentElement();


  if(rootElement.tagName() != __slRootTag)
  {
    appinterface.error("Application: wrong root element");
    appinterface.message(__smQuitMessage);
    return -1;
  }

  QDomNode childNode = rootElement.firstChild();

  if(childNode.isNull())
  {
    appinterface.error("Application: no child elements");
    appinterface.message(__smQuitMessage);
    return -1;
  }

  //----------------------------------------------------
  xmlpluginpathes::upload(rootElement);
  //----------------------------------------------------
  xmlglobalstyle::upload(rootElement, _app);
  //----------------------------------------------------
  builders::sources::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  builders::layouts::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  builders::widgets::upload( 
      rootElement, 
      xmlpluginpathes::getPlaginPathes());

  //----------------------------------------------------
  xmldatasources::upload(rootElement);

  //----------------------------------------------------
  xmldataformatters::upload(rootElement);

  //----------------------------------------------------
  xmlwidgetstyles::upload(rootElement);
  
  //----------------------------------------------------
  xmlkeyboards::upload(rootElement);
  //----------------------------------------------------
  xmlsecurity::upload(rootElement);
  //----------------------------------------------------
  xmlwindows::upload(rootElement);
  //----------------------------------------------------
  xmljscripts::upload(rootElement);

  appinterface.message(QStringLiteral("\n\tEnd deploy of project\n"));

  xmlkeyboards::init();
  xmlwindows::show();
  return 0;
}

//------------------------------------------------------------------------------
static bool __s_compilProject(char** _argv)
{
  return false;
}
