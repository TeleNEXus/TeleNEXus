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
#include "lcjscript.h"
#include "lcqjscripthiden.h"
#include "lcqjsappinterface.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>

//==============================================================================

struct SPrivateData
{
  int mInterval;
  LCQJScriptHiden* mpScriptHiden;

  SPrivateData(const QString& _script, int _interval):
    mInterval(_interval),
    mpScriptHiden(new LCQJScriptHiden(_script)){}
  ~SPrivateData()
  {
    mpScriptHiden->deleteLater();
  }
};

//==============================================================================
#define mpPrivateData (static_cast<SPrivateData*>(mpData))

//==============================================================================
LCJScript::LCJScript(const QString& _script, int _interval)  :
  mpData( new SPrivateData(_script, _interval) )
{
}

//------------------------------------------------------------------------------
LCJScript::~LCJScript()
{
  delete mpPrivateData;
}

//------------------------------------------------------------------------------
void LCJScript::start() 
{
  mpPrivateData->mpScriptHiden->start(mpPrivateData->mInterval);
}

//------------------------------------------------------------------------------
void LCJScript::stop() 
{
  mpPrivateData->mpScriptHiden->stop();
}


