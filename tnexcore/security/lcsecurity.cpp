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
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "lqsecurityfilter.h"
#include "applicationinterface.h"
#include "LIWindow.h"

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
  QString window          = "window";
  QString resetTime       = "resetTime";

  QString access          = "access";
  QString noAccess        = "noAccess";

  QString requiredAccess  = "requiredAccess";
  QString currentAccess   = "currentAccess";

  QString source          = "source";
  QString passwordStream  = "password";
  QString command         = "command";
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
  ,mResetTime(-1)
{
  mpTimer->setSingleShot(true);

  QObject::connect(mpTimer, &QTimer::timeout,
      [this]()
      {
        resetAccess();
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
void LCSecurity::init(const QDomElement& _element)
{

  using EReadStatus = LIRemoteDataSource::EReadStatus;

  if(mFlagInit) return;

  QString attr;

  mWindowId = _element.attribute(__slAttributes.window);
  if(mWindowId.isNull()) return;

  //get reset time
  attr = _element.attribute(__slAttributes.resetTime);

  if(!attr.isNull())
  {
    bool flag = false;
    int time = attr.toInt(&flag);
    if(flag)
    {
      if(time >= 0)
      {
        mResetTime = time * 1000;
      }
    }
  }

  mNoAccess = _element.attribute(__slAttributes.noAccess);
  if(mNoAccess.isNull())
  {
    mNoAccess = QStringLiteral("defaul");
  }

  //get source
  attr = _element.attribute(__slAttributes.source);
  if(attr.isNull()) return;

  auto source = CApplicationInterface::getInstance().getDataSource(attr);

  if(source.isNull()) return;

  //get password stream
  attr = _element.attribute(__slAttributes.passwordStream);
  if(attr.isNull()) return;
  mspPasswordReader = source->createReader(attr);
  if(this->mspPasswordReader.isNull()) return;

  //get required access data reader
  attr = _element.attribute(__slAttributes.requiredAccess);
  if(attr.isNull()) return;

  mspRequiredAccessIdReader = source->createReader(attr);
  if(mspRequiredAccessIdReader.isNull()) return;

  //get required access data writer 
  mspRequiredAccessIdWriter = source->createWriter(attr);
  if(mspRequiredAccessIdWriter.isNull()) return;

  //get current access data writer 
  attr = _element.attribute(__slAttributes.currentAccess);
  if(attr.isNull()) return;

  mspCurrentAccessIdWriter = source->createWriter(attr);
  if(mspCurrentAccessIdWriter.isNull()) return;
  mspCurrentAccessIdWriter->writeRequest(mNoAccess.toUtf8());

  //get command
  attr = _element.attribute(__slAttributes.command);
  if(attr.isNull()) return;

  mspCommandReader = source->createReader(attr);
  if(mspRequiredAccessIdReader.isNull()) return;


  mspRequiredAccessIdReader->setHandler(
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
          resetAccess();
          mpTimer->stop();
          qDebug() << "Reset access level to min.";
        }
      });

  mspRequiredAccessIdReader->connectToSource();
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
QObject* LCSecurity::createEventFilter(
    const QDomElement& _element, const QSet<QEvent::Type>& _events) const
{
  QString access_id = _element.attribute(__slAttributes.access);
  if(access_id.isNull()) return nullptr;
  return createEventFilter(access_id, _events);
}

//------------------------------------------------------------------------------
QObject* LCSecurity::createEventFilter(
    const QString& _accessId, 
    const QSet<QEvent::Type>& _events) const
{
  if(mFlagInit == false) return nullptr;

  auto handler = 
    [this, _events, _accessId](QObject*, QEvent* _event)
    {

      if(!_events.contains(_event->type())) 
      {
        return false;
      }

      qDebug() << "Security filter : event = " << _event->type();

      if(checkAccess(_accessId)) 
      {
        qDebug() << "Security filter : access is already avaliable.";
        return false;
      }
      qDebug() << "Security filter : access required.";
      auto window = CApplicationInterface::getInstance().getWindow(mWindowId);
      if(window.isNull())
      {
        qDebug() << 
          QString("Security filter : Can't find autorize window '%1'").arg(mWindowId);
        return false;
      }
      mspRequiredAccessIdWriter->writeRequest(_accessId.toUtf8());
      window->show();
      return true;
    };

  return new LQSecurityFilter(handler);
}

//------------------------------------------------------------------------------
bool LCSecurity::checkAccess(const QString& _accessId) const
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
    return;
  }

  mCurrentLevel = access_it.value().first;
  mspCurrentAccessIdWriter->writeRequest(_accessId.toUtf8());
  auto window = CApplicationInterface::getInstance().getWindow(mWindowId);

  if(!window.isNull())
  {
    window->hide();
  }

  if(mResetTime < 0) return;
  mpTimer->start(mResetTime);
  qDebug() << "End autorize.";
}

//------------------------------------------------------------------------------
void LCSecurity::resetAccess()
{
  mCurrentLevel = std::numeric_limits<int>::min();
  mspCurrentAccessIdWriter->writeRequest(mNoAccess.toUtf8());
}
