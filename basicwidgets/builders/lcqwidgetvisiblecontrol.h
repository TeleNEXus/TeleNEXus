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
#ifndef LCQWIDGETVISIBLECONTROL_H_
#define LCQWIDGETVISIBLECONTROL_H_

#include <QObject>

class QWidget;
class QDomElement;
class LIApplication;

class LCQWidgetVisibleControl : public QObject
{
  Q_OBJECT;
private:
  void* mpLocal = nullptr;

public:
  static bool build(const QDomElement& _element, 
      QWidget* _widget, 
      const LIApplication& _app);
  virtual ~LCQWidgetVisibleControl();
private:
  LCQWidgetVisibleControl() = delete;
  LCQWidgetVisibleControl(QWidget* _widget);

protected:
  virtual bool eventFilter(QObject* _opbj, QEvent* _event) override;

};

#endif /* LCQWIDGETVISIBLECONTROL_H_ */