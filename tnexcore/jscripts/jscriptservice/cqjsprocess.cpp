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
  qDebug() << "++++++++++++++++++++CQJSProcess Constructor";
}

CQJSProcess::~CQJSProcess()
{
  qDebug() << "--------------------CQJSProcess Destructor";
}

//------------------------------------------------------------------------------
/* void CQJSProcess::addQMetaObject( */
/*     QJSEngine& _jsengine, */ 
/*     QJSValue& _jsvalue, */ 
/*     const QString& _objectName) */
/* { */
  /* QJSValue jsobject = _jsengine.newQMetaObject(&CQJSProcess::staticMetaObject); */

  /* QJSValue jsaddvalue = _jsengine.newObject(); */

  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("FailedToStart"),   QJSValue((int)(QProcess::ProcessError::FailedToStart))); */
  /* jsaddvalue.setProperty( */
  /*   QStringLiteral("Crashed"),         QJSValue((int)(QProcess::ProcessError::Crashed))); */
  /* jsaddvalue.setProperty( */
  /*   QStringLiteral("Timedout"),        QJSValue((int)(QProcess::ProcessError::Timedout))); */
  /* jsaddvalue.setProperty( */
  /*   QStringLiteral("WriteError"),      QJSValue((int)(QProcess::ProcessError::WriteError))); */
  /* jsaddvalue.setProperty( */
  /*   QStringLiteral("ReadError"),       QJSValue((int)(QProcess::ProcessError::ReadError))); */
  /* jsaddvalue.setProperty( */
  /*   QStringLiteral("UnknownError"),    QJSValue((int)(QProcess::ProcessError::UnknownError))); */
  /* jsobject.setProperty(QStringLiteral("EnumError"), jsaddvalue); */


  /* jsaddvalue = _jsengine.newObject(); */
  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("NotRunning"),   QJSValue((int)(QProcess::ProcessState::NotRunning))); */
  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("Starting"),   QJSValue((int)(QProcess::ProcessState::Starting))); */
  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("Running"),   QJSValue((int)(QProcess::ProcessState::Running))); */
  /* jsobject.setProperty(QStringLiteral("EnumState"), jsaddvalue); */


  /* jsaddvalue = _jsengine.newObject(); */
  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("StandartOutput"),   QJSValue((int)(QProcess::ProcessChannel::StandardOutput))); */
  /* jsaddvalue.setProperty( */
  /*     QStringLiteral("StandartError"),   QJSValue((int)(QProcess::ProcessChannel::StandardError))); */
  /* jsobject.setProperty(QStringLiteral("EnumChannel"), jsaddvalue); */

  /* _jsvalue.setProperty(_objectName, jsobject); */
/* } */

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
  qDebug() << "mProcess.readAll = " << rd;

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
  /* QByteArray wd; */

  /* for(int i = 0; i < _data.size(); i++) */
  /* { */
  /*   QByteArray d = _data.at(i).toByteArray(); */
  /*   qDebug() << "CQJSProcess::write d = " << d; */
  /*   wd.append(d); */
  /* } */
  /* qint64 ret = mProcess.write(wd); */
  /* if(ret < 0) */
  /* { */
  /*   mpEngine->throwError(mProcess.errorString()); */
  /* } */

  /* return ret; */

  QByteArray wd;

  for(int i = 0; i < _data.size(); i++)
  {
    bool flag = false;
    /* int d = _data.at(i).toUInt(&flag); */
    QChar d = _data.at(i).toChar();
    /* if(!flag) { continue; } */
    /* wd[i] =  (unsigned char)d; */
    wd[i] =  d.toC;
  }
  qint64 ret = mProcess.write(wd);
  if(ret < 0)
  {
    mpEngine->throwError(mProcess.errorString());
  }
  return ret;
}

