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

static const struct
{
  QString file      = "file";
  QString id        = "id";
  QString stdformat = "stdformat";
  QString script    = "script";
}__slAttributes;

static const struct
{
  QString add = "add";
}__slTags;

static class CCreator
{
private:
  QMap<QString, std::function<QSharedPointer<LIDataFormatter>(const QDomElement&)>> mCreatorsMap;

public:
  //----------------------------------------------------------------------------
  CCreator()
  {
    mCreatorsMap.insert(__slAttributes.stdformat,
        [](const QDomElement& _element)
        {
          return stddataformatterfactory::createFormatter(
              __slAttributes.stdformat,
              _element);
        });

    mCreatorsMap.insert(__slAttributes.script,
        [](const QDomElement& _element)
        {
          Q_UNUSED(_element);
          return nullptr;
        });
  }

  //----------------------------------------------------------------------------
  void create(const QDomElement& _element)
  {
    QString attr = _element.attribute(__slAttributes.id);
    if(attr.isNull()) return;
    auto it = mCreatorsMap.find(attr);
    if(it == mCreatorsMap.end()) return;
    it.value()(_element);
  }

}__slCreator;

//==============================================================================xmlformatters
namespace xmldataformatters
{

static QMap<QString, QSharedPointer<LIDataFormatter>> __slFormattersMap;

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


