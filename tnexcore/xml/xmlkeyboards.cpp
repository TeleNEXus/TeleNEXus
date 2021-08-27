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

static const struct
{
  QString keyboard = "KEYBOARD";
}__slTags;

static const struct
{
  QString id = "id";
  QString streamSource = "streamSource"; 
  QString streamName = "streamName";
  QString dataSource = "dataSource";
  QString dataName = "dataName";
  QString window = "window";
}__slAttribubes;

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
    QString attr_id = _element.attribute(__slAttribubes.id);
    if(attr_id.isNull()) return;

    QString attr_stream_source = _element.attribute(__slAttribubes.streamSource);
    if(attr_stream_source.isNull()) return;
    auto stream_source = app.getDataSource(attr_stream_source);
    if(stream_source.isNull()) return;

    QString attr_stream_name = _element.attribute(__slAttribubes.streamName);
    if(attr_stream_name.isNull()) return;

    QString attr_data_source = _element.attribute(__slAttribubes.dataSource);
    if(attr_data_source.isNull()) return;
    auto data_source = app.getDataSource(attr_data_source);
    if(data_source.isNull()) return;

    QString attr_data_name = _element.attribute(__slAttribubes.dataName);
    if(attr_data_name.isNull()) return;

    QString attr_window = _element.attribute(__slAttribubes.window);
    if(attr_window.isNull()) return;

    auto keyboard = LCKeyboard::create(
        attr_window, 
        app,
        stream_source, attr_stream_name, 
        data_source, attr_data_name);

    if(keyboard.isNull()) return;

    __slKeyboards.insert(attr_id, keyboard);
  };

  for(auto node = 
      _rootElement.firstChildElement(__slTags.keyboard); 
      !node.isNull(); 
      node = node.nextSiblingElement(__slTags.keyboard))
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    upload_local(el);
  }
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
    __smMessage(QString("%1 can't find keyboard with id '%2'")
        .arg(__smMessageHeader).arg(_keyboardId));
    return QSharedPointer<LIKeyboard>();
  }
  return it.value();
}

}
