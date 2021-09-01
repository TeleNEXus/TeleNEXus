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
#include "xmlcommon.h"
#include "applicationinterface.h"
#include <QFileInfo>
#include <QFile>
#include <QDomElement>
#include <QMap>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

#define __smMessageHeader "Widgets styles:"
#define __smMessage(msg) CApplicationInterface::getInstance().message(msg)
#define __smWarning(msg) CApplicationInterface::getInstance().warning(msg)

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
  if(element.isNull()) 
  {
    return;
  }
  __smMessage("\n\tBegining deploy of widgets styles.\n");
  uploadElement(element);
  __smMessage("\n\tEnd deploy of widgets styles.\n");
}

QString getWidgetStyle(const QString& _styleId)
{
  auto it = __slStylesMap.find(_styleId);
  if(it == __slStylesMap.end()) 
  {
    __smMessage(
          QString("Widget style '%1' not exists")
          .arg(_styleId));
    return QString();
  }
  return it.value();
}

}

//==============================================================================uploadElement
static void uploadElement(const QDomElement& _element)
{
  QString attr_file = _element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    if(!QFileInfo::exists(attr_file))
    {
      __smWarning(QString("File '%1' is not exists").arg(attr_file));
      return;
    }
    auto ddoc = xmlcommon::loadDomDocument(attr_file);
    if(!ddoc.isNull()) uploadStyles(ddoc.documentElement());
  }
  else
  {
    uploadStyles(_element);
  }
}

//==============================================================================uploadStyle
static void uploadStyles(const QDomElement& _element)
{

  //-----------------------------------------------------------add_style[]
  auto add_style = 
    [](const QString& _id, const QString& _style)
    {
      QTextStream so(stdout);
      const QRegularExpression rexpr(
          QStringLiteral(
            "(\\(\\.\\.\\.\\))([^\\{\\}\\(\\)]*)(\\{[^\\{\\}]*\\})"));
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
        __smMessage(QString("Add widget style '%1'").arg(_id));
      }
    };

  //-----------------------------------------------------------load_from_file[]
  auto load_from_file = 
    [&add_style](const QString& _tagName, const QString& _fileName)
    {
      QFile style_file( _fileName);
      if(style_file.open(QFile::OpenModeFlag::ReadOnly))
      {
         add_style(_tagName, QTextStream(&style_file).readAll());
      }
      else
      {
        __smWarning(
            QString("Can't open widget style file '%1'").arg(_fileName));
      }
    };


  for(auto el = _element.firstChildElement();
      !el.isNull();
      el = el.nextSiblingElement())
  {
    /* __smMessage(QString("Begining load widget style '%1'").arg(el.tagName())); */

    QString attr = el.attribute(__slAttributes.file);

    if(!attr.isNull())
    {
      load_from_file(el.tagName(), attr);
    }
    else
    {
      attr = el.attribute(__slAttributes.style);
      if(!attr.isNull())
      {
        add_style(el.tagName(), attr);
      }
    }

    /* __smMessage(QString("End load widget style '%1'").arg(el.tagName())); */
  }
}
