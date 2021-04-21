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

#include "cqjsprocess.h"
#include "lcqjscriptservicehiden.h"
#include <QJSValue>
#include <QProcess>

CQJSProcess::CQJSProcess(int _engineId) : 
  QObject(nullptr),
  mpEngine(LCQJScriptHiden::getJSEngine(_engineId))
{
}

//------------------------------------------------------------------------------
void CQJSProcess::addQMetaObject(
    QJSEngine& _jsengine, 
    QJSValue& _jsvalue, 
    const QString& _objectName)
{
  QJSValue jsobject = _jsengine.newQMetaObject(&CQJSProcess::staticMetaObject);

  QJSValue jsaddvalue = _jsengine.newObject();

  jsaddvalue.setProperty(
      QStringLiteral("FailedToStart"),   QJSValue((int)(QProcess::ProcessError::FailedToStart)));
  jsaddvalue.setProperty(
    QStringLiteral("Crashed"),         QJSValue((int)(QProcess::ProcessError::Crashed)));
  jsaddvalue.setProperty(
    QStringLiteral("Timedout"),        QJSValue((int)(QProcess::ProcessError::Timedout)));
  jsaddvalue.setProperty(
    QStringLiteral("WriteError"),      QJSValue((int)(QProcess::ProcessError::WriteError)));
  jsaddvalue.setProperty(
    QStringLiteral("ReadError"),       QJSValue((int)(QProcess::ProcessError::ReadError)));
  jsaddvalue.setProperty(
    QStringLiteral("UnknownError"),    QJSValue((int)(QProcess::ProcessError::UnknownError)));
  jsobject.setProperty(QStringLiteral("Error"), jsaddvalue);


  jsaddvalue = _jsengine.newObject();
  jsaddvalue.setProperty(
      QStringLiteral("NotRunning"),   QJSValue((int)(QProcess::ProcessState::NotRunning)));
  jsaddvalue.setProperty(
      QStringLiteral("Starting"),   QJSValue((int)(QProcess::ProcessState::Starting)));
  jsaddvalue.setProperty(
      QStringLiteral("Running"),   QJSValue((int)(QProcess::ProcessState::Running)));
  jsobject.setProperty(QStringLiteral("State"), jsaddvalue);


  _jsvalue.setProperty(_objectName, jsobject);
}

//------------------------------------------------------------------------------
void CQJSProcess::start(const QString& _command)
{
  mProcess.start(_command);
}

//------------------------------------------------------------------------------
void CQJSProcess::kill()
{
  mProcess.kill();
}

//------------------------------------------------------------------------------
void CQJSProcess::terminate()
{
  mProcess.terminate();
}

//------------------------------------------------------------------------------
bool CQJSProcess::waitForStarted(int msec)
{
  return mProcess.waitForStarted(msec);
}

//------------------------------------------------------------------------------
bool CQJSProcess::waitForFinished(int msec)
{
  return mProcess.waitForFinished(msec);
}

//------------------------------------------------------------------------------
int CQJSProcess::exitCode()
{
  return mProcess.exitCode();
}

//------------------------------------------------------------------------------
int CQJSProcess::stateCode()
{
  return mProcess.state();
}

//------------------------------------------------------------------------------
int CQJSProcess::errorCode()
{
  return mProcess.error();
}

//------------------------------------------------------------------------------
QString CQJSProcess::errorString()
{
  return mProcess.errorString();
}
        
         
