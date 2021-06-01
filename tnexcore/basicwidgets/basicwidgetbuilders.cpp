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

#include "basicwidgetbuilders.h"

#include "lcxmlwidgetbuilder.h"




        /* widget                */ 
        /* label                 */ 
        /* table                 */ 
        /* dataLabel             */ 
        /* dataLineEdit          */ 
        /* dataComboBox          */ 
        /* dataComboLabel        */ 
        /* tabs                  */ 
        /* scrollArea            */ 
        /* button                */ 
        /* listWidget            */ 
        /* stackedWidget         */ 
        /* splitterVertical      */ 
        /* splitterHorizontal    */ 


namespace basicwidgetbuilders
{
QMap<QString, QSharedPointer<LIXmlWidgetBuilder>> getBuilders()
{
  QMap<QString, QSharedPointer<LIXmlWidgetBuilder>> builders;

  builders.insert(QStringLiteral("widget"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("label"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("table"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("dataLabel"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("dataLineEdit"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("dataComboBox"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("dataComboLabel"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("tabs"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("scrollArea"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("button"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("listWidget"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));
  builders.insert(QStringLiteral("stackedWidget"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));

  builders.insert(QStringLiteral("splitterVertical"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));

  builders.insert(QStringLiteral("splitterHorizontal"),
      QSharedPointer<LIXmlWidgetBuilder>(new LCXmlWidgetBuilder()));









  return builders;
}
}
