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

#ifndef LCQSTACKEDWIDGET_H_
#define LCQSTACKEDWIDGET_H_

#include <QStackedWidget>

#include <QSharedPointer>

class LIRemoteDataSource;
class LIRemoteDataReader;
class LIRemoteDataWriter;
class LIDataFormatter;


class LCQStackWidget: public QStackedWidget 
{
  Q_OBJECT;

private:
  void* mpLocal = nullptr;

public:

  explicit LCQStackWidget(QWidget* _parent = nullptr);

  explicit LCQStackWidget(
      QSharedPointer<LIRemoteDataSource> _source,
      QString _data,
      QSharedPointer<LIDataFormatter> _formatter,
      QWidget* _parent = nullptr);

  virtual ~LCQStackWidget();

  void addWidget(QWidget* _widget, const QString& _matching);
  void addWidgetUndef(QWidget* _widget);
  void addWidgetWrong(QWidget* _widget);
  virtual bool event(QEvent *e) override;
private:
  void setActive(bool _flag);
};
#endif /*LCQSTACKEDWIDGET_H_ */
