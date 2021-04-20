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

//==============================================================================
static bool readAllowCheck(const QFile& _file, QJSEngine* _jsengine);
static bool writeAllowCheck(const QFile& _file, QJSEngine* _jsengine);

CQJSBinaryFile::CQJSBinaryFile(int _engineId) :
  CQJSFileBase(LCQJScriptHiden::getJSEngine(_engineId))
{
  qDebug() << "++++++++++++++++++++++++++Binary file constructor 1";
}

CQJSBinaryFile::CQJSBinaryFile(const QString& _fileName, int _engineId) :
  CQJSFileBase(_fileName, LCQJScriptHiden::getJSEngine(_engineId))
{
  qDebug() << "++++++++++++++++++++++++++Binary file constructor 2";
}

CQJSBinaryFile::~CQJSBinaryFile()
{
  qDebug() << "--------------------------Binary file destructor";
}

qint64 CQJSBinaryFile::write(const QVariantList& _data)
{
  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    /* if(!_data.at(i).canConvert(QVariant::Type::ByteArray)) */
    /* { */
    /*   mpEngine->throwError("Wrong write data"); */
    /*   return -1; */
    /* } */
    /* wd += _data.at(i).toByteArray(); */
    bool flag = false;
    int d = _data.at(i).toUInt(&flag);
    if(!flag) 
    {
      mpEngine->throwError("Wrong write data");
      return -1;
    }
    wd[i] =  (unsigned char)d;
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
  if(!readAllowCheck(mFile, mpEngine)) return QVariantList();

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
  qDebug() << "LCQJSBinaryFile read all data size = " << rd.size();

  return ret;
}

//==============================================================================
static bool readAllowCheck(const QFile& _file, QJSEngine* _jsengine)
{

  if(!_file.isOpen())
  {
    _jsengine->throwError(QStringLiteral("File is not open"));
    return false;
  }

  if(_file.openMode() == QIODevice::OpenModeFlag::WriteOnly)
  { 
    _jsengine->throwError(QStringLiteral("File is open for write only"));
    return false;
  }

  if (_file.openMode() == QIODevice::OpenModeFlag::Append)
  {
    _jsengine->throwError(QStringLiteral("File is open for append"));
    return false;
  }

  return true;
}

//==============================================================================
static bool writeAllowCheck(const QFile& _file, QJSEngine* _jsengine)
{

  if(!_file.isOpen())
  {
    _jsengine->throwError(QStringLiteral("File is not open"));
    return false;
  }

  if(_file.openMode() == QIODevice::OpenModeFlag::ReadOnly)
  {
    _jsengine->throwError(QStringLiteral("File is open for read only"));
    return false;
  }

  return true;
}
