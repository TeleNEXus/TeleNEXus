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

#include "xmlkeyboards.h"
#include "xmlcommon.h"
#include "lckeyboard.h"
#include "applicationinterface.h"

#include <QDomElement>
#include <QDebug>

#define __smMessageHeader "Keyboards:"
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg)

static const struct
{
  QString keyboard = "KEYBOARD";
}__slTags;

static const struct
{
  QString id = "id";
  QString source = "source"; 
  QString stream = "stream";
  QString data = "data";
  QString window = "window";
}__slAttributes;

QMap<QString, QSharedPointer<LIKeyboard>> __slKeyboards;

//==============================================================================
namespace xmlkeyboards 
{

//------------------------------------------------------------------------------
void upload( const QDomElement &_rootElement)
{

  const auto upload_local = [](const QDomElement _element)
  {
    const LIApplication& app = CApplicationInterface::getInstance();
    QString attr_id = _element.attribute(__slAttributes.id);
    if(attr_id.isNull()) 
    {
      __smWarning(
          QString("Keyboard described in line %1 has no attribute %2")
          .arg(_element.lineNumber())
          .arg(__slAttributes.id));
      return;
    }

    QString attr_source = _element.attribute(__slAttributes.source);
    if(attr_source.isNull())
    {
      __smWarning(
          QString("Keyboard described in line %1 has no attribute %2")
          .arg(_element.lineNumber())
          .arg(__slAttributes.source));
      return;
    }

    auto source = app.getDataSource(attr_source);
    if(source.isNull())
    {
      __smWarning(
          QString("Can't find stream source '%1' for keyboard described in line %2")
          .arg(attr_source)
          .arg(_element.lineNumber()));
      return;
    }

    QString attr_stream_name = _element.attribute(__slAttributes.stream);
    if(attr_stream_name.isNull())
    {
      __smWarning(
          QString("Keyboard described in line %1 has no attribute %2")
          .arg(_element.lineNumber())
          .arg(__slAttributes.stream));
      return;
    }

    QString attr_data_name = _element.attribute(__slAttributes.data);
    if(attr_data_name.isNull())
    {
      __smWarning(
          QString("Keyboard described in line %1 has no attribute %2")
          .arg(_element.lineNumber())
          .arg(__slAttributes.data));
      return;
    }

    QString attr_window = _element.attribute(__slAttributes.window);
    if(attr_window.isNull())
    {
      __smWarning(
          QString("Keyboard described in line %1 has no attribute %2")
          .arg(_element.lineNumber())
          .arg(__slAttributes.window));
      return;
    }

    auto keyboard = LCKeyboard::create(
        attr_window, 
        app,
        source, attr_stream_name, attr_data_name);


    if(keyboard.isNull())
    {
      __smWarning(
          QString("Can't create keyboard described in line %1")
          .arg(_element.lineNumber()));
      return;
    }

    __smMessage(
      QString("Add keyboard with parameters:" 
        "\n\tKeyboard id:\t%1"
        "\n\tsource:\t%2"
        "\n\tStream :\t%3"
        "\n\tData name:\t%4"
        "\n\tWindow:\t\t%5\n"
        )
      .arg(attr_id)
      .arg(attr_source)
      .arg(attr_stream_name)
      .arg(attr_data_name)
      .arg(attr_window));
    __slKeyboards.insert(attr_id, keyboard);
  };

  __smMessage(QString("\n\tBegining deploy of keyboards\n"));
  for(auto element = 
      _rootElement.firstChildElement(__slTags.keyboard); 
      !element.isNull(); 
      element = element.nextSiblingElement(__slTags.keyboard))
  {
    if(element.isNull()) continue;
    upload_local(element);
  }
  __smMessage(QString("\n\tEnd deploy of keyboards\n"));
}

//------------------------------------------------------------------------------
void init()
{
  for(auto it = __slKeyboards.begin(); it != __slKeyboards.end(); it++)
  {
    auto keyboard = dynamic_cast<LCKeyboard*>(it.value().data());
    if(!keyboard) continue;
    keyboard->init();
  }
}

//------------------------------------------------------------------------------
QSharedPointer<LIKeyboard> getKeyboard(const QString& _keyboardId)
{
  auto it = __slKeyboards.find(_keyboardId);
  if(it == __slKeyboards.end()) 
  {
    return QSharedPointer<LIKeyboard>();
  }
  return it.value();
}

}
