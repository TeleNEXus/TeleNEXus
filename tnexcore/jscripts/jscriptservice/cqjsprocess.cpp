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
//==============================================================================
CQJSProcess::CQJSProcess(int _engineId) : 
  QObject(nullptr),
  mpEngine(LCQJScriptHiden::getJSEngine(_engineId))
{
}

CQJSProcess::~CQJSProcess()
{
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
void CQJSProcess::waitForStarted(int msec)
{
  if(!mProcess.waitForStarted(msec))
    mpEngine->throwError(mProcess.errorString());
}

//------------------------------------------------------------------------------
void CQJSProcess::waitForFinished(int msec)
{
  if(!mProcess.waitForFinished(msec))
    mpEngine->throwError(mProcess.errorString());
}

//------------------------------------------------------------------------------
void CQJSProcess::waitForBytesWritten(int msecs)
{
  if(!mProcess.waitForBytesWritten(msecs))
  {
    mpEngine->throwError(mProcess.errorString());
  }
}

//------------------------------------------------------------------------------
void CQJSProcess::waitForReadyRead(int msecs)
{
  if(!mProcess.waitForReadyRead(msecs))
    mpEngine->throwError(mProcess.errorString());
}

//------------------------------------------------------------------------------
void CQJSProcess::setWorkingDirectory(const QString& _dir)
{
  mProcess.setWorkingDirectory(_dir);
}

//------------------------------------------------------------------------------
QString CQJSProcess::workingDirecory()
{
  return mProcess.workingDirectory();
}

//------------------------------------------------------------------------------
bool CQJSProcess::isRunning()
{
  return (mProcess.state() == QProcess::ProcessState::Running)?(true):(false);
}

//------------------------------------------------------------------------------
bool CQJSProcess::isStarting()
{
  return (mProcess.state() == QProcess::ProcessState::Starting)?(true):(false);
}

//------------------------------------------------------------------------------
bool CQJSProcess::isCrashed()
{
  return 
    (mProcess.exitStatus() == QProcess::ExitStatus::CrashExit)?(true):(false);
}

//------------------------------------------------------------------------------
int CQJSProcess::exitCode()
{
  return mProcess.exitCode();
}
         
//------------------------------------------------------------------------------
void CQJSProcess::setReadChannelStdOut()
{
  mProcess.setReadChannel(QProcess::StandardOutput);
}
//------------------------------------------------------------------------------
void CQJSProcess::setReadChannelStdErr()
{
  mProcess.setReadChannel(QProcess::StandardError);
}
//------------------------------------------------------------------------------
bool CQJSProcess::isReadChannelStdOut()
{
  return (mProcess.readChannel() == QProcess::ProcessChannel::StandardOutput) ? 
    (true) : (false);
}
//------------------------------------------------------------------------------
bool CQJSProcess::isReadCnannelStdErr()
{
  return (mProcess.readChannel() == QProcess::ProcessChannel::StandardError) ?
    (true) : (false);
}
//------------------------------------------------------------------------------
void CQJSProcess::closeReadChannelStdOut()
{
  mProcess.closeReadChannel(QProcess::StandardOutput);
}
//------------------------------------------------------------------------------
void CQJSProcess::closeReadChannelStdErr()
{
  mProcess.closeReadChannel(QProcess::StandardError);
}
//------------------------------------------------------------------------------
void CQJSProcess::closeWriteChannel()
{
  mProcess.closeWriteChannel();
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
    ret << QVariant(static_cast<quint8>(rd[i]));
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
    unsigned char d = _data.at(i).toUInt(&flag);
    if(!flag) 
    { 
      mpEngine->throwError("Wrong data to write"); 
      return -1;
    }
    wd[i] = static_cast<quint8>(d);
  }
  qint64 ret = mProcess.write(wd);
  if(ret < 0)
  {
    mpEngine->throwError(mProcess.errorString());
  }
  return ret;
}

