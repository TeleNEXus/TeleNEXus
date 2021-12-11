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
#include "xmlsounds.h"
#include "projectsource.h"

#include <QLoggingCategory>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QDomElement>
#include <QDir>
#include <QCommandLineParser>
#include <QDirIterator>
#include <QDataStream>
#include <QCryptographicHash>

#define __smCompressLevel 3
#define __smQuitMessage QStringLiteral("Quit from TeleNEXus")
#define __smHashAlgorithm (QCryptographicHash::Sha256) 
#define __smPacketFileVersion (1)
#define __smPacketFileMagicNumber (0xA0B1C2D3)
#define __lmVersionString (QString("TeleNEXus version: %1").arg(APPLICATION_VERSION))
#define __lmApplicationDescription "TeleNEXus is a simple SCADA."

static const QString __slRootTag = "APPLICATION";

#include <csignal>

enum class EParseCommandLineResult
{
  CommandLineError,

  CommandLineCompileRequest,
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
static int __s_compilProject(
    const QString& _sourcePath, const QString& _targetFileName);
//------------------------------------------------------------------------------
static QSharedPointer<QMap<QString, QByteArray>> 
__s_readPacketFile(const QString& _fileName, QString& _msg);
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

  case EParseCommandLineResult::CommandLineHelpRequest:
    parser.showHelp();

  case EParseCommandLineResult::CommandLineVersionRequest:
    parser.showVersion();

  case EParseCommandLineResult::CommandLineCompileRequest:
    return __s_compilProject(params_list.at(0), params_list.at(1));

  case EParseCommandLineResult::CommandLineIsEmpty:
    {
      QString launch_param;
      if(argc < 2) 
      {
        launch_param = QString("./");
      }
      else
      {
        launch_param = QString(argv[1]);
      }

      QFileInfo fi(launch_param);
      qDebug() << "Launch param = " << launch_param;

      if(!fi.exists())
      {
        qDebug().noquote() << 
          QString("Path '%1' is not exists.").arg(fi.absoluteFilePath());
        return -1;
      }

      if(fi.isFile())
      {
        qDebug().noquote() << 
          QString("Launch using a packet file '%1'").arg(fi.absoluteFilePath());
        QDir::setCurrent(fi.absolutePath());
        qDebug().noquote() << "Current path = " << QDir::currentPath();
        QString error_msg;
        auto files_data = __s_readPacketFile(fi.absoluteFilePath(), error_msg);
        if(files_data.isNull())
        {
          qDebug().noquote() << error_msg;
          return -1;
        }

        prjsource = LCPacketProjectSource::create(files_data);
        CApplicationInterface::getInstance().setProjectSource(prjsource);
        CApplicationInterface::getInstance().setMessageOn(false);

        /* return 0; */
      }
      else
      {
        qDebug().noquote() << 
          QString("Launch from dir '%1'").arg(fi.absoluteFilePath());
        QDir::setCurrent(fi.absoluteFilePath());
        prjsource = LCDirProjectSource::create(fi.absoluteFilePath(), msg);
        qDebug().noquote() << msg;
        if(prjsource.isNull()) 
        {
          return -1;
        }
        qDebug().noquote()<< 
          QString("Project source from path '%1' is created")
          .arg(fi.absoluteFilePath());
        CApplicationInterface::getInstance().setProjectSource(prjsource);
      }
    }
    break;

  default:
    break;
  }

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

  QCommandLineOption compil(QStringList() << "c" << "compil",
      QStringLiteral("Compil directory path."),
      "compil");

  QCommandLineOption target(QStringList() << "t" << "target",
      QStringLiteral("Compil target file name."),
      "target");


  _parser.addOption(compil);
  _parser.addOption(target);
  QCommandLineOption version = _parser.addVersionOption();
  QCommandLineOption help = _parser.addHelpOption();

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

  if(_parser.isSet(compil))
  {

    if(_parser.value(target).isNull())
    {
      _msg = "The target file for compilation is not specified.";
      return EParseCommandLineResult::CommandLineError;
    }
    _params << _parser.value(compil) << _parser.value(target);
    return EParseCommandLineResult::CommandLineCompileRequest;
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

  appinterface.message(QStringLiteral("\n\tBegining deploy of project\n"));

  appinterface.message("Opening the main project file...");

  domDoc = CApplicationInterface::getInstance().getDomDocument("main.xml");

  if(domDoc.isNull())
  {
    appinterface.message(__smQuitMessage);
    return -1;
  }

  QDomElement rootElement = domDoc.documentElement();


  if(rootElement.tagName() != __slRootTag)
  {
    appinterface.error("Application: wrong root element");
    appinterface.message(__smQuitMessage, LIApplication::EMessageType::Deploy);
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
  xmlsounds::upload(rootElement);

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

  appinterface.message(QStringLiteral("\n\tEnd deploy of project\n"), 
      LIApplication::EMessageType::Deploy);
  appinterface.getInstance().setMessageOn(true);

  xmlkeyboards::init();
  xmlwindows::show();
  return 0;
}

//------------------------------------------------------------------------------
static int __s_compilProject(
    const QString& _sourcePath, const QString& _targetFileName)
{
  qDebug() << 
    QString("Start compil project dir with path '%1' to target file '%2'")
    .arg(_sourcePath).arg(_targetFileName);

  QFileInfo fi_source(_sourcePath);
  QFileInfo fi_target(_targetFileName);
  QString targer_file_name = fi_target.absoluteFilePath();

  QDir::setCurrent(fi_source.absoluteFilePath());
  QDir curr_dir("./");

  QFile target_file(targer_file_name);

  if(!target_file.open(QFile::OpenModeFlag::WriteOnly))
  {
    qDebug().noquote() << 
      QString("Can't open target file '%1' for write.").arg(targer_file_name);
    return -1;
  }

  QDataStream dstream(&target_file);

  QDir::Filters dir_filter =  QDir::Filter::NoDotAndDotDot | 
    QDir::Filter::Files;
  QDirIterator it("./", dir_filter, QDirIterator::IteratorFlag::Subdirectories);

  //----------------------------------------------------------------
  dstream << (quint32)__smPacketFileMagicNumber;  //magic number
  dstream << (quint32)__smPacketFileVersion;      //packet file version
  //----------------------------------------------------------------

  qDebug().noquote() << "--------------------------------------";
  qDebug().noquote() << "Packing project files...";
  qDebug().noquote() << "--------------------------------------";

  int file_counter = 0;
  while(it.hasNext())
  {
    QString file_name = curr_dir.relativeFilePath(it.next());

    qDebug().noquote() << file_name; 

    QByteArray data = file_name.toUtf8();
    QByteArray hash = 
      QCryptographicHash::hash( data, __smHashAlgorithm);

    dstream << qCompress(data,__smCompressLevel) << hash;

    QFile file(file_name);

    if(!file.open(QFile::OpenModeFlag::ReadOnly))
    {
      qDebug().noquote() << QString("Can't open project file '%1' for read.").arg(file_name);
      target_file.close();
      target_file.remove();
      return -1;
    }

    data = file.readAll();
    hash =
      QCryptographicHash::hash( data, __smHashAlgorithm);
    dstream << qCompress(data, __smCompressLevel) << hash;
    file_counter++;
  }
  qDebug().noquote() << QString("Total packed files count '%1'").arg(file_counter);
  return 0;
}

//------------------------------------------------------------------------------
static QSharedPointer<QMap<QString, QByteArray>> 
__s_readPacketFile(const QString& _fileName, QString& _errorMsg)
{
  QFile file(_fileName);
  if(!file.open(QFile::OpenModeFlag::ReadOnly))
  {
    _errorMsg =  QString("Can't open packet file '%1' for read.").arg(_fileName);
    return nullptr;
  }
  QDataStream dstream(&file);
  //----------------------------------------
  quint32 magic_number = 0;
  quint32 file_version = 0;

  dstream >> magic_number >> file_version;

  if(magic_number != __smPacketFileMagicNumber)
  {
    _errorMsg =  QString("Wrong packet file format.");
    return nullptr;
  }

  if(file_version != __smPacketFileVersion)
  {
    _errorMsg =
      QString("Wrong packet file version '%1', required version '%2'")
      .arg(file_version).arg(__smPacketFileVersion);
    return nullptr;
  }
  //----------------------------------------

  auto check_hash = 
    [&_fileName, &_errorMsg]( const QByteArray& readed_data, 
        const QByteArray& readed_hash)
    {
      if(readed_hash != 
          QCryptographicHash::hash(readed_data, __smHashAlgorithm))
      {
        _errorMsg =  QString("Error read packet file '%1'.").arg(_fileName);
        return false;
      }
      return true;
    };

  auto files_data = 
    QSharedPointer<QMap<QString,QByteArray>>(new QMap<QString,QByteArray>());

  /* qDebug().noquote() << "----------------------------------------"; */
  /* qDebug().noquote() << "Unpack files..."; */
  /* qDebug().noquote() << "----------------------------------------"; */

  while(!dstream.atEnd())
  {
    QByteArray read_data;
  
    QByteArray read_hash;
    QByteArray hash;

    dstream >> read_data;
    dstream >> read_hash;
    read_data = qUncompress(read_data);

    if(!check_hash(read_data, read_hash)) return nullptr;

    QString file_name = QString::fromUtf8(read_data);

    dstream >> read_data;
    dstream >> read_hash;
    read_data = qUncompress(read_data);

    if(!check_hash(read_data, read_hash)) return nullptr;

    files_data->insert(file_name, read_data);

    /* qDebug().noquote() << QString("'%1'").arg(file_name); */
  }

  /* qDebug().noquote() << "----------------------------------------"; */
  /* qDebug().noquote() << QString("Total unpacked files: '%1'").arg(files_data->size()); */

  return files_data;
}
