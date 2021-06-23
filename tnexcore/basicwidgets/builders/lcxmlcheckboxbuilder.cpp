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
#include "widgets/lcqdatacheckbox.h"
#include "xmlcommon.h"
#include "LIApplication.h"
#include "LIDataFormatter.h"

#include <QDomElement>
#include <QDebug>

//==============================================================================
static const struct
{
  QString text          = "text";
  QString data          = "data";
  QString dataRead      = "dataRead";
  QString dataWrite     = "dataWrite";
  QString checkValue    = "checkValue";
  QString uncheckValue  = "uncheckValue";
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

  QSharedPointer<LIRemoteDataReader> data_reader;
  QSharedPointer<LIRemoteDataWriter> data_writer;
  QByteArray check_value;
  QByteArray uncheck_value;
  QString attr_text;

  auto ret_ok = 
    [&_element,
    &attr_text,
    &data_reader,
    &data_writer,
    &check_value,
    &uncheck_value]()
    {
      auto check_box = new LCQDataCheckBox(
          attr_text, 
          data_reader, 
          data_writer, 
          check_value, 
          uncheck_value);

      setWidgetName(      _element, check_box);
      setWidgetStyle(     _element, check_box);
      setWidgetSize(      _element, check_box);
      setWidgetPosition(  _element, check_box);
      setWidgetFixedSize( _element, check_box);
      return check_box;
    };

  auto ret_wrong =
    []()
    {
      return new QCheckBox(QStringLiteral("CheckBox"));
    };

  attr_text = _element.attribute(__slAttributes.text);

  bool err_flag = false;

  auto err_handler =
    [&err_flag](const QString&)
    {
      err_flag = true;
    };

  auto data_spec_read = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data), err_handler);


  if(!err_flag)
  {
    auto source = _app.getDataSource(data_spec_read.sourceId);
    if(source.isNull()) return ret_wrong();

    auto format = _app.getDataFormatter(data_spec_read.formatterId);
    if(format.isNull()) return ret_wrong();

    data_reader = source->createReader(data_spec_read.dataId);
    if(data_reader.isNull()) return ret_wrong();

    data_writer = source->createWriter(data_spec_read.dataId);
    if(data_writer.isNull()) return ret_wrong();

    check_value = 
      format->toBytes(_element.attribute(__slAttributes.checkValue));
    if(check_value.isNull()) ret_wrong();

    uncheck_value = 
      format->toBytes(_element.attribute(__slAttributes.uncheckValue));
    return ret_ok();
  }

  err_flag = false;

  data_spec_read = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataRead), err_handler);
  if(err_flag) return ret_wrong();

  auto data_spec_write = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataWrite), err_handler);
  if(err_flag) return ret_wrong();
  
  {
    auto source = _app.getDataSource(data_spec_read.sourceId);
    if(source.isNull()) return ret_wrong();

    auto format = _app.getDataFormatter(data_spec_read.formatterId);
    if(format.isNull()) return ret_wrong();

    data_reader = source->createReader(data_spec_read.dataId);
    if(data_reader.isNull()) return ret_wrong();

    check_value = 
      format->toBytes(_element.attribute(__slAttributes.checkValue));
    if(check_value.isNull()) ret_wrong();
  }

  {
    auto source = _app.getDataSource(data_spec_write.sourceId);
    if(source.isNull()) return ret_wrong();

    auto format = _app.getDataFormatter(data_spec_write.formatterId);
    if(format.isNull()) return ret_wrong();

    data_writer = source->createWriter(data_spec_write.dataId);
    if(data_writer.isNull()) return ret_wrong();

    uncheck_value = 
      format->toBytes(_element.attribute(__slAttributes.uncheckValue));
  }

  return ret_ok();
}



