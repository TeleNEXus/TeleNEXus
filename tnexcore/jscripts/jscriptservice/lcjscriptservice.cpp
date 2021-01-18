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
#include "lcjscriptservice.h"
#include "lcqjscriptservicehiden.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>

//==============================================================================
struct SPrivateData
{
  LCQJScriptHiden* mpScriptHiden;

  SPrivateData(
      const QString& _script, 
      const QMap<QString, QString> _attributes,
      const QString& _fileName):
    mpScriptHiden(new LCQJScriptHiden(_script, _attributes, _fileName)){}

  ~SPrivateData()
  {
    mpScriptHiden->deleteLater();
  }
};

//==============================================================================
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCJScriptService::LCJScriptService(
    const QString& _script, 
    const QMap<QString, QString>& _attributes,
    const QString& _fileName)  :
  mpData( new SPrivateData(_script, _attributes, _fileName) )
{
}

//------------------------------------------------------------------------------
LCJScriptService::~LCJScriptService()
{
  delete mpPrivateData;
}

//------------------------------------------------------------------------------
QSharedPointer<LIJScriptService> LCJScriptService::create(
    const QString& _script, 
    const QMap<QString, QString>& _attributes,
    const QString& _fileName)
{
  return QSharedPointer<LCJScriptService>( 
      new LCJScriptService(_script, _attributes, _fileName) );
}

//------------------------------------------------------------------------------
void LCJScriptService::launch(int _interval) 
{
  mpPrivateData->mpScriptHiden->launch(_interval);
}

//------------------------------------------------------------------------------
void LCJScriptService::stop() 
{
  mpPrivateData->mpScriptHiden->stop();
}

//------------------------------------------------------------------------------
void LCJScriptService::execute() 
{
  mpPrivateData->mpScriptHiden->execute();
}
