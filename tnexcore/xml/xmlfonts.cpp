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
#include "xmlfonts.h"
#include "xmlcommon.h"
#include "applicationinterface.h"

#include <QFont>
#include <QMap>
#include <QDomElement>
#include <QDebug>

//==============================================================================
QMap<QString, QString> __slFontStyleMap;

//==============================================================================
static const struct
{
  QString file    = "file";
  QString fontId  = "id";
  QString style   = "style";
}__slAttributes;

//------------------------------------------------------------------------------
static const struct
{
  QString rootTag = "FONTS";
  QString font = "font";
}__slTags;


//==============================================================================
static void addFonts(const QDomElement &_element)
{
  for(QDomNode node = _element.firstChildElement(__slTags.font);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.font))
  {
    QDomElement el = node.toElement();
    QString attr_id = el.attribute(__slAttributes.fontId);

    if(attr_id.isNull()) continue;

    QString attr_style = el.attribute(__slAttributes.style);
    if(!attr_style.isNull())
    {
      __slFontStyleMap.insert(attr_id, attr_style);
    }
  }
}



static void uploadLocal(const QDomElement& _element)
{

  static const LIApplication& app = CApplicationInterface::getInstance();

  QString attr_file =  _element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    QDomElement el = app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == __slTags.rootTag) uploadLocal(el);
    }
    return;
  }
  addFonts(_element);
}


namespace xmlfonts
{
void upload(const QDomElement& _rootElement)
{
  auto el = _rootElement.firstChildElement(__slTags.rootTag);
  if(el.isNull()) return;
  uploadLocal(el);
}

QString getFontStyle(const QString& _id)
{
  auto it = __slFontStyleMap.find(_id);
  if(it == __slFontStyleMap.end()) return QString();
  return it.value();
}

}
