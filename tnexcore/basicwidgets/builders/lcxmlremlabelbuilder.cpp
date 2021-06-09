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
#include "lcxmlremlabelbuilder.h"
#include "LIApplication.h"
#include "lcqremlabel.h"
#include "xmlcommon.h"

#include <QLabel>
#include <QDomElement>
#include <QDebug>

static const struct
{
  QString data = "data";
}__slAttributes;



//------------------------------------------------------------------------------
QWidget* LCXmlRemLabelBuilder::buildLocal(
   const QDomElement& _element, const LIApplication& _app) 
{
  auto ret = 
    [&_element](QLabel* _label)
    {
      setWidgetName(_element, _label);
      setWidgetStyle(_element, _label);
      setWidgetSize(_element, _label);
      setWidgetPosition(_element, _label);
      setWidgetFixedSize(_element, _label);
      return _label;
    };

  auto ret_wrong = 
    [&_element, &ret]()
    {
      return ret(new QLabel(_element.tagName()));
    };

  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  bool err_flag = false;

  auto dataSpec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data), 
      [&err_flag](const QString&)
      {
        err_flag = true;
      });

  if(err_flag) 
  {
    return ret_wrong();
  }

  source = _app.getDataSource(dataSpec.sourceId);

  if(source.isNull())
  {
    return ret_wrong();
  }

  format = _app.getDataFormatter(dataSpec.formatterId);

  if(format.isNull())
  {
    return ret_wrong();
  }

  return ret(new LCQRemLabel(dataSpec.dataId, source, format));

}

