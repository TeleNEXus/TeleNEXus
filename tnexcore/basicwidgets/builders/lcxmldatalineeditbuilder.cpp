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
#include "lcxmldatalineeditbuilder.h"
#include "xmlcommon.h"
#include "LIApplication.h"
#include "LIKeyboard.h"
#include "LIKeyboardListener.h"
#include "LIDataFormatter.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataReader.h"
#include "lcqdatalineedit.h"

#include <QLineEdit>
#include <QDomElement>
#include <QDebug>
#include <QKeyEvent>
#include <QMap>
#include <functional>
#include <qnamespace.h>

//==============================================================================
const struct
{
  QString data = "data";
  QString dataRead = "dataRead";
  QString dataWrite = "dataWrite";
  QString keyboard = "keyboard";
} __slAttributes;

//==============================================================================
LCXmlDataLineEditBuilder::LCXmlDataLineEditBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlDataLineEditBuilder::~LCXmlDataLineEditBuilder()
{

}


//------------------------------------------------------------------------------
QWidget* LCXmlDataLineEditBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{
  QSharedPointer<LIRemoteDataReader> data_reader;
  QSharedPointer<LIRemoteDataWriter> data_writer;
  QSharedPointer<LIDataFormatter> read_formatter;
  QSharedPointer<LIDataFormatter> write_formatter;

  auto ret_wrong = 
    []()
    {
      return new QLineEdit(QStringLiteral("LineEdit"));
    };

  auto ret_ok = 
    [&_element,
    &_app,
    &data_reader,
    &data_writer,
    &read_formatter,
    &write_formatter]()
    {
      auto data_line_edit = new LCQDataLineEdit(
          data_reader, 
          read_formatter, 
          data_writer, 
          write_formatter);

      //install keyboard
      QString attr_keyboard = _element.attribute(__slAttributes.keyboard);

      if(!attr_keyboard.isNull())
      {
        auto keyboard = _app.getKeyboard(attr_keyboard);
        if(!keyboard.isNull())
        {
          data_line_edit->installKeyboard(keyboard);
        }
      }

      setWidgetName(      _element, data_line_edit);
      setWidgetStyle(     _element, data_line_edit);
      setWidgetSize(      _element, data_line_edit);
      setWidgetPosition(  _element, data_line_edit);
      setWidgetFixedSize( _element, data_line_edit);
      return data_line_edit;
    };

  bool error_flag = false;
  auto error_handler =
    [&error_flag](const QString&)
    {
      error_flag = true;
    };

  auto data_spec_read = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data), error_handler);

  if(!error_flag)
  {
    auto source = _app.getDataSource(data_spec_read.sourceId);
    if(source.isNull()) return ret_wrong();
    data_reader = source->createReader(data_spec_read.dataId);
    if(data_reader.isNull()) return ret_wrong();
    data_writer = source->createWriter(data_spec_read.dataId);
    if(data_writer.isNull()) return ret_wrong();
    read_formatter = _app.getDataFormatter(data_spec_read.formatterId);
    if(read_formatter.isNull()) return ret_wrong();
    write_formatter = read_formatter;
    return ret_ok();
  }

  error_flag = false;
  data_spec_read = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataRead), error_handler);
  if(error_flag) return ret_wrong();

  auto data_spec_write = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataWrite), error_handler);
  if(error_flag) return ret_wrong();

  {
    auto source = _app.getDataSource(data_spec_read.sourceId);
    if(source.isNull()) return ret_wrong();
    data_reader = source->createReader(data_spec_read.dataId);
    if(data_reader.isNull()) return ret_wrong();
    read_formatter = _app.getDataFormatter(data_spec_read.formatterId);
    if(read_formatter.isNull()) return ret_wrong();
  }

  {
    auto source = _app.getDataSource(data_spec_write.sourceId);
    if(source.isNull()) return ret_wrong();
    data_writer = source->createWriter(data_spec_write.dataId);
    if(data_writer.isNull()) return ret_wrong();
    write_formatter = _app.getDataFormatter(data_spec_write.formatterId);
    if(write_formatter.isNull()) return ret_wrong();
  }

  return ret_ok();
}

