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
#include <xmldataformatters.h>
#include "LIApplication.h"
#include "LIDataFormatter.h"
#include "lcxmlcommon.h"
#include "lcxmlformatterfactory.h"

#include <functional>
#include <QDomElement>
#include <QMap>
#include <QDebug>

static const struct
{

  QString file      = "file";
  QString id        = "id";
  QString stdformat = "stdformat";
  QString script    = "script";
  QString builder   = "builder";

}__slAttributes;

static const struct
{
  QString add = "add";
}__slTags;

static QMap<QString, QSharedPointer<LIDataFormatter>> __slFormattersMap;

static class CCreator
{

public:
  //----------------------------------------------------------------------------
  CCreator()
  {
  }

  //----------------------------------------------------------------------------
  QSharedPointer<LIDataFormatter> createFromStd(const QDomElement& _element)
  {
    QString format = _element.attribute(__slAttributes.stdformat);
    return stddataformatterfactory::createFormatter(format, _element);
  }

  //----------------------------------------------------------------------------
  QSharedPointer<LIDataFormatter>  createFromScript(const QDomElement& _element)
  {
    Q_UNUSED(_element);
    return nullptr;
  }

  //----------------------------------------------------------------------------
  QSharedPointer<LIDataFormatter>  createFromBuilder(const QDomElement& _element)
  {
    Q_UNUSED(_element);
    return nullptr;
  }

  //----------------------------------------------------------------------------
  void create(const QDomElement& _element)
  {

    QString attr_id = _element.attribute(__slAttributes.id);
    if(attr_id.isNull()) return;

    auto attr_all = _element.attributes();

    QSharedPointer<LIDataFormatter> fsp;

    if(attr_all.contains(__slAttributes.stdformat))
    {
      fsp = createFromStd(_element);
    }
    else if(attr_all.contains(__slAttributes.script))
    {
      fsp = createFromScript(_element);
    }
    else if(attr_all.contains(__slAttributes.builder))
    {
      fsp = createFromBuilder(_element);
    }

    if(!fsp.isNull())
    {
      __slFormattersMap.insert(attr_id, fsp);
    }
  }

}__slCreator;

//==============================================================================xmlformatters
namespace xmldataformatters
{


//------------------------------------------------------------------------------
void create( const QDomElement &_element, const LIApplication& _app)
{
  QString attr_file =  _element.attribute(__slAttributes.file);

  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      if(el.tagName() == LCXmlCommon::mBaseTags.formatters) create(el, _app);
    }
    return;
  }

  for(auto node = _element.firstChildElement(__slTags.add);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.add))
  {
    __slCreator.create(node.toElement());
  }

}

//------------------------------------------------------------------------------
QSharedPointer<LIDataFormatter> getDataFormatter(const QString& _formatterId)
{
  auto it = __slFormattersMap.find(_formatterId);
  if(it == __slFormattersMap.end()) return nullptr;
  return it.value();
}

}


