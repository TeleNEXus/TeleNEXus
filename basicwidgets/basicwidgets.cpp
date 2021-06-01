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
#include "lcxmlwidgetbuilder.h"
#include "lcxmltextlabelbuilder.h"
#include "lcxmlremlabelbuilder.h"
#include "lcxmlremlineeditbuilder.h"
#include "lcxmlremcomboboxbuilder.h"
#include "lcxmlremcombolabelbuilder.h"
#include "lcxmltablewidgetbuilder.h"
#include "lcxmltabwidgetbuilder.h"
#include "lcxmlbuttonbuilder.h"
#include "lcxmlscrollareabuilder.h"
#include "lcxmllistwidgetbuilder.h"
#include "lcxmlstackedwidgetbuilder.h"
#include "lcxmlsplitterbuilder.h"
#include <qnamespace.h>

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
void* getButtonBuilder()
{
  return new LCXmlButtonBuilder();
}

//------------------------------------------------------------------------------
void* getScrollAreaBuilder()
{
  return new LCXmlScrollAreaBuilder;
}

//------------------------------------------------------------------------------
void* getListWidgetBuilder()
{
  return new LCXmlListWidgetBuilder;
}

//------------------------------------------------------------------------------
void* getStackedWidgetBuilder()
{
  return new LCXmlStackedWidgetBuilder();
}

//------------------------------------------------------------------------------
void* getHorizontalSplitterBuilder()
{
  return new LCXmlSplitterBuilder(Qt::Horizontal);
}

//------------------------------------------------------------------------------
void* getVerticalSplitterBuilder()
{
  return new LCXmlSplitterBuilder(Qt::Vertical);
}

