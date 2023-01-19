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

#include "lcsounds.h"
#include "applicationinterface.h"
#include "LIRemoteDataReader.h"
#include "lqplaysound.h"

#include <QDomElement>
#include <QIODevice>
#include <QList> 
#include <QSharedPointer>
#include <QBuffer>
#include <QDebug>

static const struct
{
  QString sound = "sound";
}__slTags;

static const struct
{
  QString file          = "file";
  QString controlSource = "controlSource";
  QString controlData   = "controlData";
  QString repeat        = "repeat";
}__slAttribute;

static const struct
{
  QString commandSoundOn        = "on";
  QString commandSoundOff       = "off";
  QString reapeatCountsInfinite = "inf";
}__slStringValues;

//==============================================================================
static struct 
{
  QList<QSharedPointer<LIRemoteDataReader>> mSoundControllers;
}__slLocalData;

//==============================================================================
LCSounds::LCSounds()
{
}
//------------------------------------------------------------------------------
LCSounds& LCSounds::instance()
{
  static LCSounds instance;
  return instance;
}

//------------------------------------------------------------------------------
void LCSounds::addSounds(const QDomElement& _element)
{
  auto add_sound = 
    [](const QDomElement& _element)
    {
      QString attr = _element.attribute(__slAttribute.file);
      if(attr.isNull()) return;
      auto fd = CApplicationInterface::getInstance().getFileDevice(attr);
      if(fd.isNull()) return;
      if(!fd->open(QIODevice::OpenModeFlag::ReadOnly)) return;
      auto spplay = QSharedPointer<LQPlaySound>(new LQPlaySound());
      QByteArray* data = new QByteArray;
      QBuffer* buffer = new QBuffer(data);
      QObject::connect(buffer, &QBuffer::destroyed,
          [data](QObject*)
          {
            delete data;
          });

      buffer->setParent(spplay.data());
      *data = fd->readAll();
      if(data->isNull())
      {
        //error
        return;
      }
      buffer->open(QIODevice::ReadOnly);
      if(spplay->setDevice(buffer) == false)
      {
        //error
        return;
      }
      attr = _element.attribute(__slAttribute.controlSource);
      if(attr.isNull())
      {
        //error
        return;
      }

      auto source = CApplicationInterface::getInstance().getDataSource(attr);
      if(source.isNull())
      {
        //error
        return;
      }

      attr = _element.attribute(__slAttribute.controlData);
      if(attr.isNull())
      {
        //error
        return;
      }

      auto reader = source->createReader(attr);
      if(reader.isNull())
      {
        //error
        return;
      }

      attr = _element.attribute(__slAttribute.repeat);
      if(!attr.isNull())
      {
        if(attr.toLower() == __slStringValues.reapeatCountsInfinite) 
        {
          spplay->setLoops(LQPlaySound::Infinite);
        }
        else
        {
          bool flag = false;
          int repeat = attr.toInt(&flag);
          if(flag)
          {
            spplay->setLoops(repeat);
          }
        }
      }

      auto handler = 
        [spplay](QSharedPointer<QByteArray> _data, LIRemoteDataSource::EReadStatus _status)
        {
          if(_status != LIRemoteDataSource::EReadStatus::Valid) return;
          QString command = QString::fromUtf8(*(_data.data())).toLower();
          if(command == __slStringValues.commandSoundOn)
          {
            spplay->play();
          }
          else if(command == __slStringValues.commandSoundOff)
          {
            spplay->stop();
          }
        };
      reader->setHandler(handler);
      reader->connectToSource();
      __slLocalData.mSoundControllers << reader;

    };

  for(auto el = _element.firstChildElement(__slTags.sound); 
      !(el.isNull());
      el = el.nextSiblingElement(__slTags.sound))
  {
    add_sound(el);
  }
}

//------------------------------------------------------------------------------


