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

#include "projectsource.h"
#include "applicationinterface.h"
#include "lqrobuffer.h"

#include <QDebug>
#include <QMap>
#include <QBuffer>
//==============================================================================
LCDirProjectSource::LCDirProjectSource(const QDir& _projectDir) :
  mProjectDir(_projectDir)
{
}

//------------------------------------------------------------------------------
LCDirProjectSource::~LCDirProjectSource()
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCDirProjectSource> LCDirProjectSource::create(
    const QString& _prjPath, QString& _msg)
{
  QString path = _prjPath;
  if(!_prjPath.contains(QRegExp("\\/$")))
  {
    path += QString("/");
  }
  QFileInfo dir_info(path);

  if(!dir_info.exists()) 
  {
    _msg = QString("Directory with path '%1' is not exists").arg(path);
    return nullptr;
  }

  if(!dir_info.isDir()) 
  {
    _msg = QString("There is no directory on the path '%1'").arg(path);
    return nullptr;
  }

  QDir pd(path);

  _msg = QString("Create project source with path '%1'").arg(pd.absolutePath());

  return 
    QSharedPointer<LCDirProjectSource>(new LCDirProjectSource(pd));
}

//------------------------------------------------------------------------------
QSharedPointer<QIODevice> LCDirProjectSource::getFileDevice(
    const QString& _fileName) const
{
  const LIApplication& app = CApplicationInterface::getInstance();

  QString file_name = app.toProjectRelativeFilePath(_fileName);

  if(file_name.length() == 0)
  {
    app.warning(QString("Project source: wrong file name."));
  }

  QFileInfo fi(file_name);

  if(!fi.exists())
  {
    app.warning(
        QString("Project source: Can't find file '%1'")
      .arg(file_name));
    return nullptr;
  }

  if(fi.isDir())
  {
    app.warning(
        QString("Project source: '%1' is a directory")
        .arg(file_name));
    return nullptr;
  }
  return QSharedPointer<QIODevice>(new QFile(file_name));
}

//==============================================================================
LCPacketProjectSource::LCPacketProjectSource(
    QSharedPointer<QMap<QString,QByteArray>> _filesMap):
  mFilesMap(_filesMap)
{
}

//------------------------------------------------------------------------------
QSharedPointer<LCPacketProjectSource> LCPacketProjectSource::create(
    QSharedPointer<QMap<QString, QByteArray>> _filesMap)
{
  return QSharedPointer<LCPacketProjectSource>(
      new LCPacketProjectSource(_filesMap));
}

//------------------------------------------------------------------------------
QSharedPointer<QIODevice> 
LCPacketProjectSource::getFileDevice(const QString& _fileName) const
{
  const LIApplication& app = CApplicationInterface::getInstance();

  QString file_name = app.toProjectRelativeFilePath(_fileName);

  if(file_name.length() == 0)
  {
    app.warning(QString("Project source: wrong file name."));
  }

  auto it = mFilesMap->find(file_name);

  if(it != mFilesMap->end())
  {
    return QSharedPointer<QIODevice>(new LQROBuffer(&it.value()));
  }

  QFileInfo fi(file_name);

  if(!fi.exists()) 
  {
    app.warning(
        QString("Project source: Can't find file '%1'")
        .arg(_fileName));
    return nullptr;
  }

  if(fi.isDir())
  {
    app.warning(
        QString("Project source: There is a directory at the path '%1'")
        .arg(_fileName));
    return nullptr;
  }

  return QSharedPointer<QIODevice>(new QFile(file_name));
}

