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
  qDebug() << "------------------CQJSTextFile destructor.";
}
//------------------------------------------------------------------------------
QString CQJSTextFile::read(quint64 _maxlen)
{
  return QString("QJSTextFile read string data");
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
  return QString("CQJSTextFile return read line data");
}

//------------------------------------------------------------------------------
bool CQJSTextFile::write(const QString& _str)
{
  qDebug() << "CQJSTextFile::write string " << _str;
  qDebug() << "CQJSTextFile::write TextStream::Status " << mStream.status();
  qDebug() << "CQJSTextFile::write TextStream::File::name" << static_cast<QFile*>(mStream.device())->fileName();

  QTextStream stream(&mFile);

  /* stream << _str; */
  if(!mFile.isOpen()) return false;

  stream << _str;
  stream.resetStatus();

  qDebug() << "CQJSTextFile::write TextStream::Status " << mStream.status();
  return true;
}
