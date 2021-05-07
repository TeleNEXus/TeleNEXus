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
#include "lclocalsource.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"
#include "lcqlocalsourcehiden.h"

#include <QThread>

//==============================================================================SLocalData
struct SLocalData
{
  QThread* mpThread;
  QSharedPointer<LCQLocalSourceHiden> mspLocalSourceHiden;
  SLocalData() :
    mpThread(new QThread)
  {
    mspLocalSourceHiden = LCQLocalSourceHiden::create();
    mspLocalSourceHiden.data()->moveToThread(mpThread);
    mpThread->start();
  }

  ~SLocalData()
  {
    mpThread->quit();
    mpThread->wait(1000);
    mpThread->deleteLater();
  }
};

//------------------------------------------------------------------------------
#define mpLocalData (static_cast<SLocalData*>(mpData))

//==============================================================================LCLocalDataSource
LCLocalDataSource::LCLocalDataSource()
{
  mpData = new SLocalData();
}

//------------------------------------------------------------------------------
LCLocalDataSource::~LCLocalDataSource()
{
  delete mpLocalData;
}

//------------------------------------------------------------------------------
QSharedPointer<LCLocalDataSource> LCLocalDataSource::create()
{
  return QSharedPointer<LCLocalDataSource>(new LCLocalDataSource());
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addByteItem(
    const QString& _dataName, const QByteArray& _data)
{
  mpLocalData->mspLocalSourceHiden->addDataItem(_dataName, _data);
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addBitItem(
    const QString& _dataName, const QBitArray& _data)
{
  mpLocalData->mspLocalSourceHiden->addDataItem(_dataName, _data);
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addStringItem(
    const QString& _dataName, const QString& _data)
{
  mpLocalData->mspLocalSourceHiden->addDataItem(_dataName, _data);
}

//------------------------------------------------------------------------------
void LCLocalDataSource::addStreamItem(const QString& _dataName)
{
  mpLocalData->mspLocalSourceHiden->addStreamItem(_dataName);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataReader> LCLocalDataSource::createReader(
    const QString& _dataName,
    LTReadAction _readAction)
{
  return mpLocalData->mspLocalSourceHiden->createReader(
      _dataName, 
      _readAction);
}

//------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataWriter> LCLocalDataSource::createWriter(
    const QString& _dataName,
    LTWriteAction _writeAction)
{
  return mpLocalData->mspLocalSourceHiden->createWriter(
      _dataName, 
      _writeAction);
}


