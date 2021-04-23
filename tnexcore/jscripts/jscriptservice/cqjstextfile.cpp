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
#include "lcqjscriptservicehiden.h"
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

//==============================================================================CQJSTextFile
CQJSTextFile::CQJSTextFile(int _engineId) : 
  CQJSFileBase(LCQJScriptHiden::getJSEngine(_engineId))
{
}

CQJSTextFile::CQJSTextFile(const QString& _fileName, int _engineId):
  CQJSFileBase(_fileName, LCQJScriptHiden::getJSEngine(_engineId))
{
}

CQJSTextFile::~CQJSTextFile()
{
  mStream.flush();
}

//------------------------------------------------------------------------------
QString CQJSTextFile::read(quint64 _maxlen)
{
  if(!readAllowCheck()) return QString();
  return mStream.read(_maxlen);
}

//------------------------------------------------------------------------------
QString CQJSTextFile::readAll()
{
  if(!readAllowCheck()) return QString();
  return mStream.readAll();
}

//------------------------------------------------------------------------------
QString CQJSTextFile::readLine(quint64 _maxlen)
{
  if(!readAllowCheck()) return QString();
  return mStream.readLine(_maxlen);
}

//------------------------------------------------------------------------------
bool CQJSTextFile::write(const QString& _str)
{
  if(!writeAllowCheck()) return false;
  mStream << _str;
  mStream.flush();
  return true;
}

//------------------------------------------------------------------------------
bool CQJSTextFile::open(const QString& _openMode)
{
  if(!CQJSFileBase::open(_openMode)) 
  {
    return false;
  }
  mStream.setDevice(&mFile);
  return true;
}

//------------------------------------------------------------------------------
void CQJSTextFile::close()
{
  mStream.flush();
  CQJSFileBase::close();
}

//------------------------------------------------------------------------------
bool CQJSTextFile::seek(quint64 _pos)
{
  if(!mFile.isOpen()) 
  {
    mpEngine->throwError("File is not open");
    return false;
  }
  return mStream.seek(_pos);
}

//------------------------------------------------------------------------------
bool CQJSTextFile::setEncoding(const QString& _encodingName)
{
  QTextCodec* codec = QTextCodec::codecForName(_encodingName.toLocal8Bit());
  if(!codec) return false;
  mStream.setCodec(codec);
  return true;
}

