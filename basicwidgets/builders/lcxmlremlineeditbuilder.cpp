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
#include "lcxmlformatterfactory.h"
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
  QString format  = "format";
} __attrNames;

//------------------------------------------------------------------------------
QWidget* LCXmlRemLineEditBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;
  QLineEdit* ret = nullptr;

  QString data;
  QString attr = element.attribute(__attrNames.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  if(attr.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  source = app.getDataSource(attr);

  if(source.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  data = element.attribute(__attrNames.data);

  if(data.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  format = LCXmlStdDataFormatterFactory::
    instance().createStringFormatter(element.attributes());

  if(format.isNull())
  {
    goto LABEL_WRONG_EXIT;
  }

  ret= new LCQRemLineEdit(data, data, source, format);

LABEL_WRONG_EXIT:

  if(ret == nullptr) ret = new QLineEdit(element.tagName());

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);
  
  ret->setStyleSheet(style);
  
  LCBuildersCommon::initPosition(element, *ret);
  return ret;
}



