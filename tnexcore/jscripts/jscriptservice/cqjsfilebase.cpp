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
#include <QDebug>

//==============================================================================
const CQJSFileBase::SOpenModes CQJSFileBase::mOpenModes;

CQJSFileBase::CQJSFileBase(QJSEngine* _jsengine) : 
  QObject(nullptr),
  mpEngine(_jsengine)
{
  qDebug() << "+++++++++++++++++++++++++++++++++++++++CQJSFileBase Constructor";
}

CQJSFileBase::CQJSFileBase(const QString& _fileName, QJSEngine* _jsengine) : 
  QObject(nullptr), 
  mFile(_fileName),
  mpEngine(_jsengine)
{
  qDebug() << "+++++++++++++++++++++++++++++++++++++++CQJSFileBase Constructor";
}

CQJSFileBase::~CQJSFileBase() 
{
  qDebug() << "---------------------------------------CQJSFileBase Destructor";
}

//------------------------------------------------------------------------------
bool CQJSFileBase::open(const QString& _openMode)
{
  if(mFile.isOpen()) return true;
  bool flag = false;
  if(_openMode == mOpenModes.ReadOnly){
     flag = mFile.open(QFile::OpenModeFlag::ReadOnly);
  }
  else if(_openMode == mOpenModes.WriteOnly){
    flag = mFile.open(QFile::OpenModeFlag::WriteOnly);
  }
  else if(_openMode == mOpenModes.ReadWrite){
    flag = mFile.open(QFile::OpenModeFlag::ReadWrite);
  }
  else if(_openMode == mOpenModes.Append){
    flag = mFile.open(QFile::OpenModeFlag::Append);
  }

  if(!flag) { mpEngine->throwError(mFile.errorString()); }

  return flag;
}

//------------------------------------------------------------------------------
QString CQJSFileBase::openMode() const
{
  switch(mFile.openMode())
  {
  case QFile::OpenModeFlag::ReadOnly:
    return mOpenModes.ReadOnly;

  case QFile::OpenModeFlag::WriteOnly:
    return mOpenModes.WriteOnly;

  case QFile::OpenModeFlag::ReadWrite:
    return mOpenModes.ReadWrite;

  case QFile::OpenModeFlag::Append:
    return mOpenModes.Append;

  default:
    return mOpenModes.NotOpen;
  }
}

//------------------------------------------------------------------------------
QString CQJSFileBase::fileName() const
{
  return mFile.fileName();
}

//------------------------------------------------------------------------------
void CQJSFileBase::setFileName(const QString& _fileName) 
{
  if(mFile.isOpen()) 
  {
    mpEngine->throwError("File is opened");
  }

  mFile.setFileName(_fileName);

  if(mFile.error() != QFile::NoError)
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
}

//------------------------------------------------------------------------------
void CQJSFileBase::close()
{
  mFile.close();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::isOpen() const
{
  return mFile.isOpen();
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
  bool flag = mFile.remove();
  if((!flag) && (mFile.error() != QFile::NoError))
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return flag;
}

//------------------------------------------------------------------------------
bool CQJSFileBase::rename(const QString& _fileName)
{
  bool flag = mFile.rename(_fileName);
  if((!flag) && (mFile.error() != QFile::NoError))
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return flag;
}

//------------------------------------------------------------------------------
quint64 CQJSFileBase::pos() const
{
  return mFile.pos();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::seek(quint64 _pos)
{
  bool flag = mFile.seek(_pos);
  if((!flag) && (mFile.error() != QFile::NoError))
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return flag;
}

//------------------------------------------------------------------------------
quint64 CQJSFileBase::size() const
{
  return mFile.size();
}

//------------------------------------------------------------------------------
bool CQJSFileBase::flush()
{
  bool flag = mFile.flush();
  if((!flag) && (mFile.error() != QFile::NoError))
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return flag;
}

//------------------------------------------------------------------------------
bool CQJSFileBase::copy(const QString& _fileName)
{
  bool flag = mFile.copy(_fileName);
  if((!flag) && (mFile.error() != QFile::NoError))
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return flag;
}

//==============================================================================
bool CQJSFileBase::readAllowCheck()
{

  if(!mFile.isOpen())
  {
    mpEngine->throwError(QStringLiteral("File is not open"));
    return false;
  }

  if(mFile.openMode() == QIODevice::OpenModeFlag::WriteOnly)
  { 
    mpEngine->throwError(QStringLiteral("File is open for write only"));
    return false;
  }

  if (mFile.openMode() == QIODevice::OpenModeFlag::Append)
  {
    mpEngine->throwError(QStringLiteral("File is open for append"));
    return false;
  }

  return true;
}

//==============================================================================
bool CQJSFileBase::writeAllowCheck()
{

  if(!mFile.isOpen())
  {
    mpEngine->throwError(QStringLiteral("File is not open"));
    return false;
  }

  if(mFile.openMode() == QIODevice::OpenModeFlag::ReadOnly)
  {
    mpEngine->throwError(QStringLiteral("File is open for read only"));
    return false;
  }

  return true;
}
