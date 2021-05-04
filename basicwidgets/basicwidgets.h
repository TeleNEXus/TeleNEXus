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
#ifndef BASICWIDGETS_H
#define BASICWIDGETS_H

#include "basicwidgets_global.h"

extern "C" void* getWidgetBuilder();
extern "C" void* getTextLabelBuilder();

extern "C" void* getRemLabelBuilder();
extern "C" void* getRemLineEditBuilder();
extern "C" void* getRemComboBoxBuilder();
extern "C" void* getRemComboLabelBuilder();
extern "C" void* getTableWidgetBuilder();
extern "C" void* getTabWidgetBuilder();
extern "C" void* getRemWriteButtonBuilder();
extern "C" void* getSwitchWidgetsListBuilder();
extern "C" void* getControlWindowButtonBuilder();
extern "C" void* getScrollAreaBuilder();
extern "C" void* getListWidgetBuilder();
extern "C" void* getStackedWidgetBuilder();
extern "C" void* getHorizontalSplitterBuilder();
extern "C" void* getVerticalSplitterBuilder();

#endif // BASICWIDGETS_H
