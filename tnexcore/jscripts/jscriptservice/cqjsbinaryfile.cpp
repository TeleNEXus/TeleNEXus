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
#include "cqjsbinaryfile.h"
#include "lcqjscriptservicehiden.h"
#include <QDebug>


CQJSBinaryFile::CQJSBinaryFile(int _engineId) :
  CQJSFileBase(LCQJScriptHiden::getJSEngine(_engineId))
{
}

CQJSBinaryFile::CQJSBinaryFile(const QString& _fileName, int _engineId) :
  CQJSFileBase(_fileName, LCQJScriptHiden::getJSEngine(_engineId))
{
}

CQJSBinaryFile::~CQJSBinaryFile()
{
}

qint64 CQJSBinaryFile::write(const QVariantList& _data)
{
  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    bool flag = false;
    unsigned char d = _data.at(i).toUInt(&flag);
    if(!flag) 
    { 
      mpEngine->throwError("Wrong data to write"); 
      return -1;
    }
    wd[i] = static_cast<quint8>(d);
  }
  qint64 ret = mFile.write(wd);
  if(ret < 0)
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
  }
  return ret;
}

QVariantList CQJSBinaryFile::read(qint64 _maxSize)
{
  if(!readAllowCheck()) return QVariantList();

  QByteArray rd = mFile.read(_maxSize);

  if(mFile.error() != QFile::NoError)
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
    return QVariantList();
  }

  QVariantList ret;
  for (qint32 i = 0; i < rd.size(); i++)
  {
    ret << QVariant(static_cast<quint8>(rd[i]));
  }

  return ret;
}

QVariantList CQJSBinaryFile::readAll()
{
  if(!readAllowCheck()) return QVariantList();

  QByteArray rd = mFile.readAll();

  if(mFile.error() != QFile::NoError)
  {
    mpEngine->throwError(mFile.errorString());
    mFile.unsetError();
    return QVariantList();
  }

  QVariantList ret;
  for (qint32 i = 0; i < rd.size(); i++)
  {
    ret << QVariant(static_cast<quint8>(rd[i]));
  }
  return ret;
}

