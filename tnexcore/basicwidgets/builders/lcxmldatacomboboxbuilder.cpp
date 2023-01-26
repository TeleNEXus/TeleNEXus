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
#include "lcxmldatacomboboxbuilder.h"
#include "xmlcommon.h"
#include "widgets/lcqdatacombobox.h"
#include "LIApplication.h"
#include "LIWindow.h"

#include <QDomElement>
#include <QComboBox>

//==============================================================================
LCXmlDataComboBoxBuilder::LCXmlDataComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlDataComboBoxBuilder::~LCXmlDataComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
  QString data        = "data";
  QString dataRead    = "dataRead";
  QString dataWrite   = "dataWrite";
  QString name        = "name";
  QString value       = "value";
} __slAttributes;

//------------------------------------------------------------------------------
const struct
{
  QString item    = "item";
} __slTags;

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQDataComboBox* _box, 
    QSharedPointer<LIDataFormatter> __format);

//------------------------------------------------------------------------------buildLocal
QWidget* LCXmlDataComboBoxBuilder::buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{

  QSharedPointer<LIRemoteDataReader>  data_reader;
  QSharedPointer<LIDataFormatter>     formatter_read;
  QSharedPointer<LIRemoteDataWriter>  data_writer;
  QSharedPointer<LIDataFormatter>     formatter_write;

  auto ret_wrong = 
    []()
    {
      return new QComboBox();
    };

  auto ret_ok = 
    [ &_element, &data_reader, &formatter_read, &data_writer, &formatter_write]()
    {
      auto combo_box = new LCQDataComboBox( 
          data_reader, formatter_read, data_writer, formatter_write);

      buildCombobox(_element, combo_box, formatter_read);

      setWidgetName(combo_box);
      setWidgetStyle(     _element, combo_box);
      setWidgetSize(      _element, combo_box);
      setWidgetPosition(  _element, combo_box);
      setWidgetFixedSize( _element, combo_box);

      return combo_box;
    };
  
  bool err_flag = false;

  auto parse_ctrl = 
    [&err_flag](const QString&)
    {
      err_flag = true;
    };

  auto dataSpec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data), parse_ctrl);

  if(!err_flag)
  {
    auto source = _app.getDataSource(dataSpec.sourceId);
    if(source.isNull()) return ret_wrong();

    data_reader = source->createReader(dataSpec.dataId);
    if(data_reader.isNull()) return ret_wrong();

    data_writer = source->createWriter(dataSpec.dataId);
    if(data_writer.isNull()) return ret_wrong();

    formatter_read = 
      formatter_write = _app.getDataFormatter(dataSpec.formatterId);
    if(formatter_read.isNull()) return ret_wrong();
    return ret_ok();
  }

  err_flag = false;
  dataSpec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataRead), parse_ctrl);

  if(err_flag) return ret_wrong();

  auto source = _app.getDataSource(dataSpec.sourceId);
  if(source.isNull()) return ret_wrong();

  data_reader = source->createReader(dataSpec.dataId);
  if(data_reader.isNull()) return ret_wrong();

  formatter_read = _app.getDataFormatter(dataSpec.formatterId);
  if(formatter_read.isNull()) return ret_wrong();


  err_flag = false;
  dataSpec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataWrite), parse_ctrl);

  if(err_flag) return ret_wrong();

  source = _app.getDataSource(dataSpec.sourceId);
  if(source.isNull()) return ret_wrong();

  data_writer = source->createWriter(dataSpec.dataId);
  if(data_writer.isNull()) return ret_wrong();

  formatter_write = _app.getDataFormatter(dataSpec.formatterId);
  if(formatter_read.isNull()) return ret_wrong();

  return ret_ok();


  /* QWidget *ret = nullptr; */
  /* QString dataread; */
  /* QString datawrite; */
  /* QString attr = _element.attribute(__slAttributes.source); */
  /* QSharedPointer<LIRemoteDataSource> source; */
  /* QSharedPointer<LIDataFormatter> format; */

  /* if(attr.isNull()) */
  /* { */
  /*   goto LABEL_WRONG_EXIT; */
  /* } */

  /* source = _app.getDataSource(attr); */

  /* if(source.isNull()) */
  /* { */
  /*   goto LABEL_WRONG_EXIT; */
  /* } */

  /* dataread = _element.attribute(__slAttributes.read); */

  /* if(dataread.isNull()) */
  /* { */
  /*   goto LABEL_WRONG_EXIT; */
  /* } */

  /* datawrite = _element.attribute(__slAttributes.write); */

  /* if(datawrite.isNull()) */
  /* { */
  /*   datawrite = dataread; */
  /* } */

  /* format = _app.getDataFormatter( */
  /*     _element.attribute(__slAttributes.format)); */
  /* if(format.isNull()) */
  /* { */
  /*   goto LABEL_WRONG_EXIT; */
  /* } */

  /* if(format.isNull()) */
  /* { */
  /*   goto LABEL_WRONG_EXIT; */
  /* } */

  /* ret = new LCQDataComboBox(dataread, datawrite, source, format); */
  /* buildCombobox(_element, static_cast<LCQDataComboBox*>(ret), format); */

/* LABEL_WRONG_EXIT: */
  /* if(ret == nullptr) */ 
  /* { */
  /*   ret = new QComboBox(); */ 
  /*   static_cast<QComboBox*>(ret)->setEnabled(false); */
  /*   static_cast<QComboBox*>(ret)->addItem(_element.tagName()); */
  /* } */

  /* return ret; */
}

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQDataComboBox* _box,
    QSharedPointer<LIDataFormatter> _format)
{
  QDomNodeList nodes = _element.childNodes();

  for(auto item_element = _element.firstChildElement(__slTags.item);
      !item_element.isNull();
      item_element = item_element.nextSiblingElement(__slTags.item))
  {
    QString attr_name = item_element.attribute(__slAttributes.name);
    QString attr_value = item_element.attribute(__slAttributes.value);

    if (attr_value.isNull()) 
    {
      continue;
    }

    if (attr_name == "")
    {
      attr_name = attr_value;
    }

    QByteArray value = _format->toBytes(attr_value);

    if( (value.isNull()) || (value.size() == 0))
    {
      continue;
    }
    _box->addItem(attr_name, _format->toString(value));
  }
}

