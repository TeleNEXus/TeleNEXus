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

CQJSBinaryFile::CQJSBinaryFile(QJSEngine* _jsengine) :
  CQJSFileBase(_jsengine)
{
}

CQJSBinaryFile::CQJSBinaryFile(const QString& _fileName, QJSEngine* _jsengine) :
  CQJSFileBase(_fileName, _jsengine)
{
}

qint64 CQJSBinaryFile::write(const QVariantList& _data)
{
  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    if(!_data.at(i).canConvert(QVariant::Type::ByteArray))
    {
      mpEngine->throwError("Wrong write data");
      return -1;
    }
    /* bool flag = false; */
    /* int d = _data.at(i).toUInt(&flag); */
    /* if(!flag) */ 
    /* { */
    /*   return 0; */
    /* } */
    /* wd[i] =  (unsigned char)d; */
    wd += _data.at(i).toByteArray();
  }
  qint64 ret = mFile.write(wd);
  if(ret < 0)
  {
    mpEngine->throwError(mFile.errorString());
  }
  return ret;
}

QVariantList CQJSBinaryFile::read(qint64 _maxSize)
{
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
    ret << QVariant(rd[i]);
  }

  return ret;
}

QVariantList CQJSBinaryFile::readAll()
{
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
    ret << QVariant(rd[i]);
  }

  return ret;
}
