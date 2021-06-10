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

#include "xmlwidgetstyles.h"
#include "applicationinterface.h"
#include <QFile>
#include <QDomElement>
#include <QMap>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
//==============================================================================__slAttributes
static const struct
{
  QString style = "style";
  QString file  = "file";
}__slAttributes;

//==============================================================================__slTags
static const struct
{
  QString rootTag = "WIDGETSTYLES";
}__slTags;

//==============================================================================__slStylesMap
static QMap<QString, QString> __slStylesMap;

//==============================================================================declarations
static void uploadStyles(const QDomElement& _element);
static void uploadElement(const QDomElement& _element);

//==============================================================================namespace
namespace xmlwidgetstyles 
{

void upload( const QDomElement &_rootElement)
{
  if(_rootElement.isNull()) return;
  auto element = _rootElement.firstChildElement(__slTags.rootTag);
  if(element.isNull()) return;
  uploadElement(element);
}

QString getWidgetStyle(const QString& _styleId)
{
  auto it = __slStylesMap.find(_styleId);
  if(it == __slStylesMap.end()) return QString();
  return it.value();
}

}

//==============================================================================uploadElement
static void uploadElement(const QDomElement& _element)
{
  QString attr_file = _element.attribute(__slAttributes.file);

  if(attr_file.isNull())
  {
    uploadStyles(_element);
  }
  else
  {
    uploadElement(
        CApplicationInterface::getInstance()
        .getDomDocument(attr_file)
        .documentElement());
  }
}

//==============================================================================uploadStyle
static void uploadStyles(const QDomElement& _element)
{

  //-----------------------------------------------------------load_from_file[]
  auto load_from_file = 
    [](const QString& _fileName)
    {
      QFile style_file(
          CApplicationInterface::getInstance().getProjectPath() + 
          _fileName);
      if(style_file.open(QFile::OpenModeFlag::ReadOnly))
      {
        return QTextStream(&style_file).readAll();
      }
      return QString();
    };

  //-----------------------------------------------------------add_style[]
  auto add_style = 
    [](const QString& _id, const QString& _style)
    {
      QTextStream so(stdout);
      const QRegularExpression rexpr(
          QStringLiteral("(\\(\\.\\.\\.\\))([^\\{\\}\\(\\)]*)(\\{[^\\{\\}]*\\})"));
      auto matches = rexpr.globalMatch(_style);
      QString style;
      while(matches.hasNext())
      {
        auto match = matches.next();
        style += match.captured() + QChar(' ');
      }
      if(!style.isNull())
      {
        __slStylesMap.insert(_id, style);
      }
    };

  for(auto node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    if(!node.isElement()) continue;

    auto el = node.toElement();

    QString attr = el.attribute(__slAttributes.file);

    if(!attr.isNull())
    {
      add_style(el.tagName(), load_from_file(attr));
      continue;
    }

    attr = el.attribute(__slAttributes.style);

    if(!attr.isNull())
    {
      add_style(el.tagName(), attr);
    }
  }
}
