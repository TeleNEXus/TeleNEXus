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
#include "cqjstextfile.h"
#include <QTextStream>
#include <QDebug>

//==============================================================================
CQJSTextFile::CQJSTextFile(CQJSFileBase* _parent) : 
  CQJSFileBase(_parent),
  mStream(&mFile)
{
}

CQJSTextFile::CQJSTextFile(const QString& _fileName, CQJSFileBase* _parent ):
  CQJSFileBase(_fileName, _parent),
  mStream(&mFile)
{
}

CQJSTextFile::~CQJSTextFile()
{
}

//------------------------------------------------------------------------------
QString CQJSTextFile::read(quint64 _maxlen)
{
  if(!mFile.isOpen()) return QString();
  return mStream.read(_maxlen);
}

//------------------------------------------------------------------------------
QString CQJSTextFile::readAll()
{
  if(!mFile.isOpen()) return QString();
  return mStream.readAll();
}

//------------------------------------------------------------------------------
QString CQJSTextFile::readLine(quint64 _maxlen)
{
  if(!mFile.isOpen()) return QString();
  return mStream.readLine(_maxlen);
}

//------------------------------------------------------------------------------
bool CQJSTextFile::write(const QString& _str)
{
  if(!mFile.isOpen()) return false;
  mStream << _str;
  return true;
}

//------------------------------------------------------------------------------
bool CQJSTextFile::seek(quint64 _pos)
{
  return mStream.seek(_pos);
}
