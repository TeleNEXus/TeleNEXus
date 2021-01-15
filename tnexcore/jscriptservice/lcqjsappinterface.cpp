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
#include "lcqjsappinterface.h"
#include "lcqjsappservice.h"
#include "LIApplication.h"
#include <QDebug>
#include <QSharedPointer>
#include <QVariantList>


//==============================================================================
struct SPrivateData
{
  QSharedPointer<LCQJSAppService> psAppService;
  SPrivateData() :
    psAppService(LCQJSAppService::getService()){}
};

//------------------------------------------------------------------------------
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCQJSAppInterface::LCQJSAppInterface() :
  mpData(new SPrivateData)
{
}

//------------------------------------------------------------------------------
LCQJSAppInterface::~LCQJSAppInterface()
{
  delete mpPrivateData;
}

//------------------------------------------------------------------------------
void LCQJSAppInterface::debugOut(const QString& _str)
{
  qDebug("%s", qPrintable(_str));
}

//------------------------------------------------------------------------------
QString LCQJSAppInterface::getProjectPath()
{
  return mpPrivateData->psAppService->getProjectPath();
}

//------------------------------------------------------------------------------
QVariantList LCQJSAppInterface::readData(
    const QString& _sourceId, const QString& _dataId)
{
  QByteArray data_array = 
    mpPrivateData->psAppService->readData(_sourceId, _dataId); 
  QVariantList ret;
  for(int i = 0; i < data_array.size(); i++)
  {
    ret << QVariant(static_cast<unsigned char>(data_array.at(i)));
  }
  return ret; 
}

//------------------------------------------------------------------------------
int LCQJSAppInterface::writeData(
    const QString& _sourceId, 
    const QString& _dataId, 
    const QVariantList& _data)
{

  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    bool flag = false;
    int d = _data.at(i).toUInt(&flag);
    if(!flag) 
    {
      return 0;
    }
    wd[i] =  (unsigned char)d;
  }

  return mpPrivateData->psAppService->writeData(_sourceId, _dataId, wd);
}

//------------------------------------------------------------------------------
/* QVariantList LCQJSAppInterface::testGetByteArray() */
/* { */
/*   /1* int data = 0x01020304; *1/ */
/*   int data = 0x01020304; */
/*   QVariantList vl; */
/*   /1* QByteArray arr(reinterpret_cast<const char*>(&i), sizeof(int)); *1/ */
/*   for(int i = 0; i < (int)sizeof(int); i++) */
/*   { */
/*     vl << QVariant((reinterpret_cast<char*>(&data))[i]); */
/*     /1* vl << QVariant(i); *1/ */
/*   } */
/*   return vl; */

/*   /1* _retString = "SByteArray LCQJSAppInterface::testGetByteArray(QString& _retString)"; *1/ */
/*   /1* return (reinterpret_cast<const char*>(&i), sizeof(int)); *1/ */
/* } */
