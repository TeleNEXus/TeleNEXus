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

#include <limits>
#include <QTimer>
#include <QDomElement>
#include <QCryptographicHash>

static const struct
{
  QString access = "access";
}__slTags;

static const struct
{
  QString id = "id";
  QString level = "level";
  QString password = "password";
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
  mpTimer->setInterval(5 * 60 * 1000); //5min
  QObject::connect(mpTimer, &QTimer::timeout,
      [this]()
      {
        mCurrentLevel = std::numeric_limits<int>::min();
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
  if(mFlagInit) return;

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
      mAccess.insert(id, QPair<int, QByteArray>(level, hash));
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
bool LCSecurity::checkAccessLevel(const QString& _accessId)
{
  auto access_it = mAccess.find(_accessId);
  if(access_it == mAccess.end()) return false;
  if(access_it.value().first == std::numeric_limits<int>::min()) return false;
  if(access_it.value().first > mCurrentLevel) return false;
  return true;
}

//------------------------------------------------------------------------------
void LCSecurity::autorize(const QString& _accessId, const QString& _password)
{
  auto access_it = mAccess.find(_accessId);
  if(access_it == mAccess.end()) return;
  if(access_it.value().first <= mCurrentLevel) return;
  if(__GetHash(_password.toUtf8()) != access_it.value().second) return;
}

