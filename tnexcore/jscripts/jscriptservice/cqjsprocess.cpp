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
#include <QProcess>
#include <QVariant>

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


  jsaddvalue = _jsengine.newObject();
  jsaddvalue.setProperty(
      QStringLiteral("StandartOutput"),   QJSValue((int)(QProcess::ProcessChannel::StandardOutput)));
  jsaddvalue.setProperty(
      QStringLiteral("StandartError"),   QJSValue((int)(QProcess::ProcessChannel::StandardError)));
  jsobject.setProperty(QStringLiteral("Channel"), jsaddvalue);

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
void CQJSProcess::closeReadChannel(int _channel)
{
  if(_channel == 0)
    mProcess.closeReadChannel(QProcess::ProcessChannel::StandardOutput);
  else
    mProcess.closeReadChannel(QProcess::ProcessChannel::StandardError);
}

//------------------------------------------------------------------------------
void CQJSProcess::closeWriteChannel()
{
  mProcess.closeWriteChannel();
}

//------------------------------------------------------------------------------
int CQJSProcess::readChannel()
{
  return mProcess.readChannel();
}

//------------------------------------------------------------------------------
void CQJSProcess::setReadChannel(int _channel)
{
  if(_channel == 0)
    mProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);
  else
    mProcess.setReadChannel(QProcess::ProcessChannel::StandardError);
}

//------------------------------------------------------------------------------
QVariantList CQJSProcess::read(qint64 _maxSize)
{
  QByteArray rd = mProcess.read(_maxSize);
  QVariantList ret;
  for (qint32 i = 0; i < rd.size(); i++)
  {
    ret << QVariant(rd[i]);
  }
  return ret;
}

//------------------------------------------------------------------------------
QVariantList CQJSProcess::readAll()
{
  QByteArray rd = mProcess.readAll();
  QVariantList ret;
  for (qint32 i = 0; i < rd.size(); i++)
  {
    ret << QVariant(rd[i]);
  }
  return ret;
}

//------------------------------------------------------------------------------
qint64 CQJSProcess::write(const QVariantList& _data)
{
  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    bool flag = false;
    int d = _data.at(i).toUInt(&flag);
    if(!flag) 
    {
      mpEngine->throwError("Wrong write data");
      return -1;
    }
    wd[i] =  (unsigned char)d;
  }
  qint64 ret = mProcess.write(wd);
  if(ret < 0)
  {
    mpEngine->throwError(mProcess.errorString());
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CQJSProcess::waitForBytesWritten(int msecs)
{
  return mProcess.waitForBytesWritten(msecs);
}

//------------------------------------------------------------------------------
bool CQJSProcess::waitForReadyRead(int msecs)
{
  return mProcess.waitForReadyRead(msecs);
}



