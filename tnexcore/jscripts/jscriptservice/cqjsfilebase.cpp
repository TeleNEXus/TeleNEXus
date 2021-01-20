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
#include "cqjsfilebase.h"
#include <QString>
#include <QFileInfo>

//==============================================================================
static const struct 
{
  QString NotOpen = "no";
  QString ReadOnly = "r";
  QString WriteOnly = "w";
  QString ReadWrite = "rw";
  QString Append = "a";
}__slOpenModes;

//==============================================================================
CQJSFileBase::CQJSFileBase(QObject* _parent) : 
  QObject(_parent) 
{
}

CQJSFileBase::CQJSFileBase(const QString& _fileName, QObject* _parent) : 
  QObject(_parent), 
  mFile(_fileName)
{
}

//------------------------------------------------------------------------------
bool CQJSFileBase::open(const QString& _openMode)
{
  if(mFile.isOpen()) return true;
  if(_openMode.isNull()) return false;
  bool ret = false;
  if(_openMode == __slOpenModes.ReadOnly){
    ret = mFile.open(QFile::OpenModeFlag::ReadOnly);
  }
  else if(_openMode == __slOpenModes.WriteOnly){
    ret = mFile.open(QFile::OpenModeFlag::WriteOnly);
  }
  else if(_openMode == __slOpenModes.ReadWrite){
    ret = mFile.open(QFile::OpenModeFlag::ReadWrite);
  }
  else if(_openMode == __slOpenModes.Append){
    ret = mFile.open(QFile::OpenModeFlag::Append);
  }
  return ret;
}

//------------------------------------------------------------------------------
QString CQJSFileBase::openMode() const
{
  switch(mFile.openMode())
  {
  case QFile::OpenModeFlag::ReadOnly:
    return __slOpenModes.ReadOnly;

  case QFile::OpenModeFlag::WriteOnly:
    return __slOpenModes.WriteOnly;

  case QFile::OpenModeFlag::ReadWrite:
    return __slOpenModes.ReadWrite;

  case QFile::OpenModeFlag::Append:
    return __slOpenModes.Append;

  default:
    return __slOpenModes.NotOpen;
  }
}

//------------------------------------------------------------------------------
QString CQJSFileBase::fileName() const
{
  return mFile.fileName();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::setFileName(const QString& _fileName) 
{
  if(mFile.isOpen()) return false;
  mFile.setFileName(_fileName);
  return true;
}

//------------------------------------------------------------------------------
void CQJSFileBase::close()
{
  mFile.close();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::atEnd() const
{
  return mFile.atEnd();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::exists() const
{
  return mFile.exists();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::remove()
{
  return mFile.remove();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::rename(const QString& _newName)
{
  return mFile.rename(_newName);
}

//------------------------------------------------------------------------------
quint64 CQJSFileBase::pos() const
{
  return mFile.pos();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::seek(quint64 _pos)
{
  return mFile.seek(_pos);
}

//------------------------------------------------------------------------------
quint64 CQJSFileBase::size() const
{
  return mFile.size();
}
