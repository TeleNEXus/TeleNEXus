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

#include "xmlsounds.h"
#include "xmlcommon.h"
#include "applicationinterface.h"
#include "lqplaysound.h"
#include "LIRemoteDataReader.h"

#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

#define __smMessage(msg) CApplicationInterface::getInstance().message(msg, LIApplication::EMessageType::Deploy)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg, LIApplication::EMessageType::Deploy)

//==============================================================================
static const struct
{
  QString file    = "file";
  QString controlSource = "controlSource";
  QString controlData = "controlData";
  QString repeat  = "repeat";
}__slAttributes;

static const struct
{
  QString sounds = "SOUNDS";
  QString sound = "sound";
}__slTags;


static struct 
{
  QList<QSharedPointer<LIRemoteDataReader>> mSoundControllers;
}__slLocalData;

//------------------------------------------------------------------------------
static void __s_addSounds(const QDomElement& _element);

namespace xmlsounds
{
void upload(const QDomElement& _rootElement)
{
  auto end_upload = 
    []()
    {
      __smMessage("\n\tEnd deploy sounds.\n");
    };

  __smMessage( "\n\tBegin deploy sounds.\n");

  QDomElement element = _rootElement.firstChildElement(__slTags.sounds);
  if(element.isNull()) 
  {
    __smMessage("\n\tProject has no sounds block.\n");
    end_upload();
    return;
  }

  QString attr_file = element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    element = xmlcommon::loadDomDocument(attr_file).documentElement();
    if(element.isNull()) { return end_upload(); }
  }

  __s_addSounds(element);

  end_upload();
}

}

//------------------------------------------------------------------------------
static void __s_addSounds(const QDomElement& _element)
{

  auto add_sound = 
    [](const QDomElement& _element)
    {
      QString attr = _element.attribute(__slAttributes.file);
      if(attr.isNull()) 
      {
        __smWarning(
            QString("Element 'sound' in line(%1) has no attribute '%2'")
            .arg(_element.lineNumber()).arg(__slAttributes.file));
        return;
      }

      auto fd = CApplicationInterface::getInstance().getFileDevice(attr);

      if(fd.isNull()) 
      {
        return;
      }

      if(!fd->open(QIODevice::OpenModeFlag::ReadOnly)) 
      {
        __smWarning(QString("Can't open for read file '%1'").arg(__slAttributes.file));
        return;
      }

      auto spplay = QSharedPointer<LQPlaySound>(new LQPlaySound());

      if(spplay->setDevice(fd.data()) == false)
      {
        __smWarning(QString("Error set audio source: '%1'").arg(spplay->getErrorMessage()));
        return;
      }

      QString source_id = _element.attribute(__slAttributes.controlSource);
      if(source_id.isNull())
      {
        __smWarning(QString("Element 'sound' has no attribute '%1'").arg(__slAttributes.controlSource));
        return;
      }


      auto source = CApplicationInterface::getInstance().getDataSource(source_id);
      if(source.isNull())
      {
        __smWarning(QString("Can't find source '%1'").arg(source_id));
        return;
      }

      attr = _element.attribute(__slAttributes.controlData);
      if(attr.isNull())
      {
        __smWarning(QString("Element 'sound' has no attribute '%1'").arg(__slAttributes.controlData));
        return;
      }

      auto reader = source->createReader(attr);
      if(reader.isNull())
      {
        __smWarning(QString("Can't create reader '%1' for source '%2'").arg(attr).arg(source_id));
        return;
      }

      attr = _element.attribute(__slAttributes.repeat);
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


