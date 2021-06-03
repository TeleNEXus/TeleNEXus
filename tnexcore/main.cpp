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

#include "applicationinterface.h"
#include "xmlfonts.h"
#include "xmlbuilders.h"
#include "xmldatasources.h"
#include "xmlwindows.h"
#include "xmldataformatters.h"
#include "xmljscripts.h"
#include "xmlkeyboards.h"
#include "xmlpluginpathes.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDomElement>
#include <QDir>
#include <QCommandLineParser>

static const QString __slRootTag = "APPLICATION";

//==============================================================================
static const struct
{
  QString xmlMainFileName = "xmlfile";
  QString xmlMainFilePath = "xmlpath";
} __slAppOptionsName;

//==============================================================================
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

//==============================================================================main
int main(int argc, char *argv[])
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


  //----------------------------------------------------
  xmlpluginpathes::upload(rootElement);
  //----------------------------------------------------
  builders::sources::upload( 
      rootElement, 
      __slParameters.projectPath, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  xmldatasources::upload(rootElement);
  //----------------------------------------------------
  xmlfonts::upload(rootElement);
  //----------------------------------------------------
  xmldataformatters::upload(rootElement);
  //----------------------------------------------------
  builders::layouts::upload( 
      rootElement, 
      __slParameters.projectPath, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  builders::widgets::upload( 
      rootElement, 
      __slParameters.projectPath, 
      xmlpluginpathes::getPlaginPathes());
  //----------------------------------------------------
  xmlkeyboards::upload(rootElement);
  //----------------------------------------------------
  xmlwindows::upload(rootElement);
  //----------------------------------------------------
  xmljscripts::upload(rootElement);
  

  xmlkeyboards::init();
  xmlwindows::show();


  QObject::connect(&app, &QApplication::aboutToQuit,
      [&](){
        qDebug() << "QApplication::aboutToQuit";
      });

  return app.exec();
}


