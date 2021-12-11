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
  QString file    = "file";
  QString controlSource = "controlSource";
  QString controlData = "controlData";
  QString repeat  = "repeat";
}__slAttribute;

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

      qDebug() <<"++++add sound 0";
      QString attr = _element.attribute(__slAttribute.file);
      if(attr.isNull()) return;
      qDebug() <<"++++add sound 1";
      auto fd = CApplicationInterface::getInstance().getFileDevice(attr);
      if(fd.isNull()) return;
      qDebug() <<"++++add sound 2";
      if(!fd->open(QIODevice::OpenModeFlag::ReadOnly)) return;

      qDebug() <<"++++add sound 3";
      auto spplay = QSharedPointer<LQPlaySound>(new LQPlaySound());

      qDebug() <<"++++add sound 4";
      QByteArray* data = new QByteArray;

      qDebug() <<"++++add sound 5";
      QBuffer* buffer = new QBuffer(data);
      QObject::connect(buffer, &QBuffer::destroyed,
          [data](QObject*)
          {
            delete data;
          });

      buffer->setParent(spplay.data());

      qDebug() <<"++++add sound 5";
      *data = fd->readAll();
      if(data->isNull())
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 6";
      buffer->open(QIODevice::ReadOnly);
      if(spplay->setDevice(buffer) == false)
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 7";
      attr = _element.attribute(__slAttribute.controlSource);
      if(attr.isNull())
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 8";

      auto source = CApplicationInterface::getInstance().getDataSource(attr);
      if(source.isNull())
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 9";
      attr = _element.attribute(__slAttribute.controlData);
      if(attr.isNull())
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 10";
      auto reader = source->createReader(attr);
      if(reader.isNull())
      {
        //error
        return;
      }

      qDebug() <<"++++add sound 11";
      attr = _element.attribute(__slAttribute.repeat);
      if(!attr.isNull())
      {
        if(attr.toLower() == "inf") 
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

      qDebug() <<"++++add sound 12";

      auto handler = 
        [spplay](QSharedPointer<QByteArray> _data, LIRemoteDataSource::EReadStatus _status)
        {
          if(_status != LIRemoteDataSource::EReadStatus::Valid) return;
          QString command = QString::fromUtf8(*(_data.data())).toLower();
          if(command == "on")
          {
            spplay->play();
          }
          else if(command == "off")
          {
            spplay->stop();
          }
        };

      qDebug() <<"++++add sound 5";
      reader->setHandler(handler);
      reader->connectToSource();
      __slLocalData.mSoundControllers << reader;

    };

  for(auto el = _element.firstChildElement(__slTags.sound); 
      !(el.isNull());
      el = el.nextSiblingElement(__slTags.sound))
  {
    add_sound(el);
    qDebug() << "Add sound";
  }
}

//------------------------------------------------------------------------------


