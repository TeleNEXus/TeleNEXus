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
#include "basicwidgets.h"
#include "lcqremwritebutton.h"
#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremlabelbuilder.h"
#include "lcxmlremlineeditbuilder.h"
#include "lcxmlremcomboboxbuilder.h"
#include "lcxmlremcombolabelbuilder.h"
#include "lcxmltablewidgetbuilder.h"
#include "lcxmltabwidgetbuilder.h"
#include "lcxmlbuttonbuilder.h"
#include "lcxmlswitchwidgetslistbuilder.h"
#include "lcxmlscrollareabuilder.h"

//------------------------------------------------------------------------------
void* getWidgetBuilder()
{
  return new LCXmlWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getTextLabelBuilder()
{
  return new LCXmlTextLabelBuilder;
}

//------------------------------------------------------------------------------
void* getRemLabelBuilder()
{
  return new LCXmlRemLabelBuilder;
}

//------------------------------------------------------------------------------
void*  getRemLineEditBuilder()
{
  return new LCXmlRemLineEditBuilder;
}

//------------------------------------------------------------------------------
void*  getRemComboBoxBuilder()
{
  return new LCXmlRemComboBoxBuilder;
}

//------------------------------------------------------------------------------
void*  getRemComboLabelBuilder()
{
  return new LCXmlRemComboLabelBuilder;
}

//------------------------------------------------------------------------------
void* getTableWidgetBuilder()
{
  return new LCXmlTableWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getTabWidgetBuilder()
{
  return new LCXmlTabWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getRemWriteButtonBuilder()
{
  return new LCXmlButtonBuilder(LCXmlButtonBuilder::EType::writeData);
}

//------------------------------------------------------------------------------
void* getControlWindowButtonBuilder()
{
  return new LCXmlButtonBuilder(LCXmlButtonBuilder::EType::controlWindows);
}

//------------------------------------------------------------------------------
void* getSwitchWidgetsListBuilder()
{
  return new LCXmlSwitchWidgetsListBuilder;
}

//------------------------------------------------------------------------------
void* getScrollAreaBuilder()
{
  return new LCXmlScrollAreaBuilder;
}
