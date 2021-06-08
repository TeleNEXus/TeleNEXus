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
#ifndef LCXMLBUILDERBASE_H_
#define LCXMLBUILDERBASE_H_

#include "LIXmlWidgetBuilder.h"
#include <QPoint>
#include <QSharedPointer>
#include <qglobal.h>

class LIMovieAccess;

class LCXmlBuilderBase : public LIXmlWidgetBuilder
{
protected:

  LCXmlBuilderBase();
  virtual ~LCXmlBuilderBase();

  virtual QWidget* build( 
      const QDomElement&      _element, 
      const LIApplication&    _app) override final;

protected:
  virtual QWidget* buildLocal(
      const QDomElement& _element, const LIApplication& _app) = 0;
  static bool setWidgetPosition(const QDomElement& _element, QWidget* _widget);
  static bool setWidgetSize(const QDomElement& _element, QWidget* _widget);
  static bool setWidgetFixedSize(const QDomElement& _element, QWidget* _widget);
  static void setWidgetName(const QDomElement& _element, QWidget* _widget);
  static void setWidgetStyle(const QString& _style, QWidget* _widget);
  static void setWidgetStyle(const QDomElement& _element, QWidget* _widget);
  static QSharedPointer<LIMovieAccess> getMovie(const QString& _movie, const LIApplication& _app);
  static QPixmap getPixmap(const QString& _file, const LIApplication& _app);
  static QPixmap parsePixmap(const QString& _expr, const LIApplication& _app);
};

#endif
