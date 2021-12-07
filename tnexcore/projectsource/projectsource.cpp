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
  QFileInfo dir_info(_prjPath);

  if(!dir_info.exists()) 
  {
    _msg = QString("Directory with path '%1' is not exists").arg(_prjPath);
    return nullptr;
  }

  if(!dir_info.isDir()) 
  {
    _msg = QString("There is no directory on the path '%1'").arg(_prjPath);
    return nullptr;
  }

  QDir pd(_prjPath);

  _msg = QString("Create project source with path '%1'").arg(pd.absolutePath());

  return 
    QSharedPointer<LCDirProjectSource>(new LCDirProjectSource(pd));
}

//------------------------------------------------------------------------------
QSharedPointer<QIODevice> LCDirProjectSource::getFileDevice(
    const QString& _fileName) const
{

  QFileInfo fi(_fileName);

  if(!fi.exists())
  {
    CApplicationInterface::getInstance().warning(
        QString("Project source: Can't find file '%1'")
      .arg(_fileName));
    return nullptr;
  }

  if(fi.isDir())
  {
    CApplicationInterface::getInstance().warning(
        QString("Project source: There is directory at the path '%1'")
        .arg(_fileName));
    return nullptr;
  }
  return QSharedPointer<QIODevice>(new QFile(_fileName));
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
  QString file_name;


  if(_fileName.contains(QRegExp("^\\.\\/")))
  {
    file_name = _fileName.mid(2);
  }

  auto it = mFilesMap->find(_fileName);

  if(it != mFilesMap->end())
  {
    return QSharedPointer<QIODevice>(new LQROBuffer(&it.value()));
  }

  QFileInfo fi(_fileName);

  if(!fi.exists()) 
  {
    CApplicationInterface::getInstance().warning(
        QString("Project source: Can't find file '%1'")
        .arg(fi.fileName()));
    return nullptr;
  }

  if(fi.isDir())
  {
    CApplicationInterface::getInstance().warning(
        QString("Project source: There is directory at the path '%1'")
        .arg(_fileName));
    return nullptr;
  }

  return QSharedPointer<QIODevice>(new QFile(_fileName));
}

