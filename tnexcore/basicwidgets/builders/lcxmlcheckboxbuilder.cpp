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
#include "lcxmlcheckboxbuilder.h"
#include "widgets/lcqcheckbox.h"
#include "xmlcommon.h"
#include "LIApplication.h"
#include "LIDataFormatter.h"

#include <QDomElement>
#include <QDebug>

//==============================================================================
static const struct
{
  QString text = "text";
  QString data = "data";
  QString checkValue = "checkValue";
  QString uncheckValue = "uncheckValue";
}__slAttributes;
//==============================================================================
LCXmlCheckBoxBuilder::LCXmlCheckBoxBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlCheckBoxBuilder::~LCXmlCheckBoxBuilder()
{
}

//------------------------------------------------------------------------------
QWidget* LCXmlCheckBoxBuilder::buildLocal(
      const QDomElement& _element, const LIApplication& _app)
{
  auto ret = 
    [&_element](QWidget* _widget)
    {
      setWidgetName(      _element, _widget);
      setWidgetStyle(     _element, _widget);
      setWidgetSize(      _element, _widget);
      setWidgetPosition(  _element, _widget);
      setWidgetFixedSize( _element, _widget);
      return _widget;
    };

  auto ret_wrong =
    []()
    {
      return new QCheckBox(QStringLiteral("CheckBox"));
    };
  QString attr_text = _element.attribute(__slAttributes.text);

  bool err_flag = false;

  auto data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data),
        [&err_flag](const QString&)
        {
          err_flag = true;
        });
  if(err_flag) return ret_wrong();

  auto source = _app.getDataSource(data_spec.sourceId);
  if(source.isNull()) return ret_wrong();

  auto format = _app.getDataFormatter(data_spec.formatterId);
  if(format.isNull()) return ret_wrong();

  auto reader = source->createReader(data_spec.dataId);
  if(reader.isNull()) return ret_wrong();

  auto writer = source->createWriter(data_spec.dataId);
  if(writer.isNull()) return ret_wrong();

  QByteArray check_value = 
    format->toBytes(_element.attribute(__slAttributes.checkValue));
  if(check_value.isNull()) ret_wrong();

  QByteArray uncheck_value = 
    format->toBytes(_element.attribute(__slAttributes.uncheckValue));
  /* if(uncheck_value.isNull()) ret_wrong(); */

  return ret(
      new LCQCheckBox(
        attr_text, 
        reader, 
        writer, 
        check_value, 
        uncheck_value));
}
