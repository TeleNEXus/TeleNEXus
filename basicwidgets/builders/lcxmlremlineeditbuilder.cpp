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
#include "lcxmlremlineeditbuilder.h"
#include "lcqremlineedit.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QDebug>

//==============================================================================
LCXmlRemLineEditBuilder::LCXmlRemLineEditBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemLineEditBuilder::~LCXmlRemLineEditBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
  QString data    = "data";
  QString source  = "source";
  /* QString format  = "format"; */
} __attrNames;

//------------------------------------------------------------------------------
QWidget* LCXmlRemLineEditBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QString data;
  QString attr = element.attribute(__attrNames.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  auto ret_widget = 
    [_buildData](QLineEdit* _le = nullptr)
    {
      if(_le == nullptr) _le = new QLineEdit(_buildData->element.tagName());
      QString style = LCBuildersCommon::getBaseStyleSheet(_buildData->element, _buildData->application);
      _le->setStyleSheet(style);
      LCBuildersCommon::initPosition(_buildData->element, *_le);
      return _le;
    };



  if(attr.isNull())
  {
    return ret_widget();
  }

  source = app.getDataSource(attr);

  if(source.isNull())
  {
    return ret_widget();
  }

  data = element.attribute(__attrNames.data);

  if(data.isNull())
  {
    return ret_widget();
  }

  attr = element.attribute(LCBuildersCommon::mAttributes.dataformatter);
  format = _buildData->application.getStdDataFormatter(attr);


  if(format.isNull())
  {
    attr = element.attribute(LCBuildersCommon::mAttributes.dataformatterid);
    format = _buildData->application.getDataFormatter(attr);
    if(format.isNull()) return ret_widget();
  }

  return ret_widget(new LCQRemLineEdit(data, data, source, format));
}



