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
#include "lcxmlremcomboboxbuilder.h"
#include "widgets/lcqremcombobox.h"
#include "LIApplication.h"
#include "LIWindow.h"

#include <QDomElement>
#include <qcombobox.h>
#include <qdom.h>
#include <qwidget.h>

//==============================================================================
LCXmlRemComboBoxBuilder::LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemComboBoxBuilder::~LCXmlRemComboBoxBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
  QString read        = "read";
  QString write       = "write";
  QString source      = "source";
  QString format      = "format";
  QString text        = "text";
  QString value       = "value";
} __slAttributes;

//------------------------------------------------------------------------------
const struct
{
  QString item    = "item";
} __slTags;

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQRemComboBox* _box, 
    QSharedPointer<LIDataFormatter> __format);

//------------------------------------------------------------------------------build
QWidget* LCXmlRemComboBoxBuilder::buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{
  QWidget *ret = nullptr;
  QString dataread;
  QString datawrite;
  QString attr = _element.attribute(__slAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  if(attr.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  source = _app.getDataSource(attr);

  if(source.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  dataread = _element.attribute(__slAttributes.read);

  if(dataread.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  datawrite = _element.attribute(__slAttributes.write);

  if(datawrite.isNull())
  {
    datawrite = dataread;
  }

  format = _app.getDataFormatter(
      _element.attribute(__slAttributes.format));
  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret = new LCQRemComboBox(dataread, datawrite, source, format);
  buildCombobox(_element, static_cast<LCQRemComboBox*>(ret), format);

LABEL_WRONG_EXIT:
  if(ret == nullptr) 
  {
    ret = new QComboBox(); 
    static_cast<QComboBox*>(ret)->setEnabled(false);
    static_cast<QComboBox*>(ret)->addItem(_element.tagName());
  }

  return ret;
}

//------------------------------------------------------------------------------buildCombobox
static void buildCombobox(   const QDomElement& _element, 
    LCQRemComboBox* _box,
    QSharedPointer<LIDataFormatter> _format)
{
  QDomNodeList nodes = _element.childNodes();

  for(auto item_element = _element.firstChildElement(__slTags.item);
      !item_element.isNull();
      item_element = item_element.nextSiblingElement(__slTags.item))
  {
    QString attr_name = item_element.attribute(__slAttributes.text);
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

