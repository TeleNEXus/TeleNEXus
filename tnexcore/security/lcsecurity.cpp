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

#include "lcsecurity.h"
#include "LIApplication.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"

#include <limits>
#include <QTimer>
#include <QDomElement>
#include <QCryptographicHash>
#include <QDebug>
#include <iostream>
#include <ostream>

static const struct
{
  QString access = "access";
}__slTags;

static const struct
{
  QString access          = "access";
  QString source          = "source";
  QString passwordStream  = "password";
  QString command         = "command";
  QString message         = "message";
  QString password        = "password";
  QString id              = "id";
  QString level           = "level";
}__slAttributes;

//==============================================================================
static QByteArray __GetHash(const QByteArray& _data)
{
  return QCryptographicHash::hash(_data, QCryptographicHash::Algorithm::Md5);
}

//==============================================================================
LCSecurity::LCSecurity() : 
  mFlagInit(false)
  ,mCurrentLevel(std::numeric_limits<int>::min())
  ,mpTimer(new QTimer)
{
  mpTimer->setSingleShot(true);
  /* mpTimer->setInterval(5 * 60 * 1000); //5min */

  mpTimer->setInterval(10 * 1000); //5min

  QObject::connect(mpTimer, &QTimer::timeout,
      [this]()
      {
        mCurrentLevel = std::numeric_limits<int>::min();
        writeMessage("No autorized access");
        qDebug() << "Clear access autorized.";
      });
}

//------------------------------------------------------------------------------
LCSecurity::~LCSecurity()
{
  mpTimer->stop();
  mpTimer->deleteLater();
}

//------------------------------------------------------------------------------

LCSecurity& LCSecurity::instance()
{
  static LCSecurity instance;
  return instance;
}

//------------------------------------------------------------------------------
void LCSecurity::init(const QDomElement& _element, const LIApplication& _app)
{

  using EReadStatus = LIRemoteDataSource::EReadStatus;

  if(mFlagInit) return;

  //get source
  QString attr = _element.attribute(__slAttributes.source);
  if(attr.isNull()) return;
  auto source = _app.getDataSource(attr);
  if(source.isNull()) return;

  //get password stream
  attr = _element.attribute(__slAttributes.passwordStream);
  if(attr.isNull()) return;
  mspPasswordReader = source->createReader(attr);
  if(this->mspPasswordReader.isNull()) return;

  //get access data
  attr = _element.attribute(__slAttributes.access);
  if(attr.isNull()) return;

  mspAccessIdReader = source->createReader(attr);
  if(mspAccessIdReader.isNull()) return;

  //get command
  attr = _element.attribute(__slAttributes.command);
  if(attr.isNull()) return;

  mspCommandReader = source->createReader(attr);
  if(mspAccessIdReader.isNull()) return;

  //get message 
  attr = _element.attribute(__slAttributes.message);
  if(!attr.isNull()) 
  {
    mspMessageWriter = source->createWriter(attr);
    writeMessage("No autorized access");
  }

  mspAccessIdReader->setHandler(
      [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
      {
        if(_status != EReadStatus::Valid) return;
        mAccessId = QString::fromUtf8(*_data);
        qDebug() << "Set new access id : " << mAccessId;
      });

  mspPasswordReader->setHandler(
      [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
      {
        if(_status != EReadStatus::Valid) return;
        QString password = QString::fromUtf8(*_data);
        qDebug() << "Set password : " << password;
        if(checkAccess(mAccessId) == true) return;
        autorize(mAccessId, password);
      });

  mspCommandReader->setHandler(
      [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
      {
        if(_status != EReadStatus::Valid) return;
        if(QString::fromUtf8(*_data) == QStringLiteral("reset"))
        {
          mCurrentLevel = std::numeric_limits<int>::min();
          writeMessage("No autorized access");
          mpTimer->stop();
          qDebug() << "Reset access livel to min.";
        }
      });

  mspAccessIdReader->connectToSource();
  mspPasswordReader->connectToSource();
  mspCommandReader->connectToSource();

  auto add_access = 
    [this](QDomElement& _accessElement)
    {
      QString id = _accessElement.attribute(__slAttributes.id);
      if(id.isNull()) return;
      QString attr = _accessElement.attribute(__slAttributes.level);
      if(attr.isNull()) return;
      bool flag = false;
      int level = attr.toInt(&flag);
      if(!flag) return;
      attr = _accessElement.attribute(__slAttributes.password);
      if(attr.isNull()) return;
      QByteArray hash = __GetHash(attr.toUtf8()); 

      qDebug() << "++Security add access : \n\tid = " << id << 
        ",\n\tlevel = " << level << 
        ",\n\tpassword = " << attr << 
        ",\n\thash = " << QString::fromLatin1(hash.toHex()).toUpper();

      mAccesses.insert(id, QPair<int, QByteArray>(level, hash));
    };

  for(auto element = _element.firstChildElement(__slTags.access); 
      !element.isNull(); 
      element = element.nextSiblingElement(__slTags.access))
  {
    add_access(element);
  }

  mFlagInit = true;
}

//------------------------------------------------------------------------------
bool LCSecurity::checkAccess(const QString& _accessId)
{
  auto access_it = mAccesses.find(_accessId);
  if(access_it == mAccesses.end()) return false;
  if(access_it.value().first == std::numeric_limits<int>::min()) return false;
  if(access_it.value().first > mCurrentLevel) return false;
  qDebug() << "Security: current level '" << mCurrentLevel << 
    "' is mor or equal than desired level '" << access_it.value().first << "'";
  return true;
}

//------------------------------------------------------------------------------
void LCSecurity::autorize(const QString& _accessId, const QString& _password)
{
  qDebug() << "Start autorize.";
  auto access_it = mAccesses.find(_accessId);
  if(access_it == mAccesses.end()) 
  {
    qDebug() << "Security: can't find access id '" << _accessId << "'";
    return;
  }
  if(access_it.value().first <= mCurrentLevel) 
  {
    qDebug() << "Security: current level '" << mCurrentLevel << 
      "' is mor or equal than desired level '" << access_it.value().first << "'";
    return;
  }

  if(__GetHash(_password.toUtf8()) != access_it.value().second) 
  {
    qDebug() << "Wrong password.";
    writeMessage("Wrong password");
    QTimer::singleShot(2000, 
        [this]()
        {
          if(mCurrentLevel == std::numeric_limits<int>::min()) 
          {
            writeMessage("No autorized access");
          }
        });
    return;
  }

  mCurrentLevel = access_it.value().first;

  writeMessage(QString("Autorized access in \"%1\"").arg(_accessId));

  mpTimer->start();
  qDebug() << "End autorize.";
}

//------------------------------------------------------------------------------
void LCSecurity::writeMessage(const QString& _message)
{
  if(mspMessageWriter.isNull()) return;
  mspMessageWriter->writeRequest(_message.toUtf8());
}

