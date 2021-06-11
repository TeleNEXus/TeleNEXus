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

/* #include "basicwidgetbuilders.h" */

#include "basicwidgetbuilders.h"
#include "LIXmlWidgetBuilder.h"

#include "lcxmlframebuilder.h"
#include "lcxmllabelbuilder.h"
#include "lcxmldatalabelbuilder.h"
#include "lcxmlbuttonbuilder.h"
#include "lcxmltablewidgetbuilder.h"
#include "lcxmldatalineeditbuilder.h"

/* #include "lcxmlremcomboboxbuilder.h" */
/* #include "lcxmlremcombolabelbuilder.h" */
/* #include "lcxmltabwidgetbuilder.h" */
/* #include "lcxmlscrollareabuilder.h" */
/* #include "lcxmllistwidgetbuilder.h" */
/* #include "lcxmlstackedwidgetbuilder.h" */
/* #include "lcxmlsplitterbuilder.h" */
#include <qnamespace.h>


namespace basicwidgetbuilders
{
QMap<QString, QSharedPointer<LIXmlWidgetBuilder>> getBuilders()
{
  QMap<QString, QSharedPointer<LIXmlWidgetBuilder>> builders_map;
  auto add_builder = 
    [&builders_map](const QString& _name, LIXmlWidgetBuilder* _builder)
    {
      builders_map.insert(_name, QSharedPointer<LIXmlWidgetBuilder>(_builder));
    };

  add_builder(QStringLiteral("Frame"              ),  new LCXmlFrameBuilder());
  add_builder(QStringLiteral("Button"             ),  new LCXmlButtonBuilder());
  add_builder(QStringLiteral("Label"              ),  new LCXmlLabelBuilder());
  add_builder(QStringLiteral("DataLabel"          ),  new LCXmlDataLabelBuilder());
  add_builder(QStringLiteral("Table"              ),  new LCXmlTableWidgetBuilder());
  add_builder(QStringLiteral("DataLineEdit"       ),  new LCXmlDataLineEditBuilder());

  /* add_builder(QStringLiteral("dataLineEdit"       ),  new LCXmlRemLineEditBuilder()); */
  /* add_builder(QStringLiteral("dataComboBox"       ),  new LCXmlRemComboBoxBuilder()); */
  /* add_builder(QStringLiteral("dataComboLabel"     ),  new LCXmlRemComboLabelBuilder()); */
  /* add_builder(QStringLiteral("tabs"               ),  new LCXmlTabWidgetBuilder()); */
  /* add_builder(QStringLiteral("scrollArea"         ),  new LCXmlScrollAreaBuilder()); */
  /* add_builder(QStringLiteral("listWidget"         ),  new LCXmlListWidgetBuilder()); */
  /* add_builder(QStringLiteral("stackedWidget"      ),  new LCXmlStackedWidgetBuilder()); */
  /* add_builder(QStringLiteral("splitterVertical"   ),  new LCXmlSplitterBuilder(Qt::Vertical)); */
  /* add_builder(QStringLiteral("splitterHorizontal" ),  new LCXmlSplitterBuilder(Qt::Horizontal)); */

  return builders_map;
}
}
