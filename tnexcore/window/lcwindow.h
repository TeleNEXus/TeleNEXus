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
#ifndef LCWINDOW_H_
#define LCWINDOW_H_

#include "LIWindow.h"
#include <qnamespace.h>

class QWidget;
class LCWindow : public LIWindow
{
private:
  void* mpLocal;
public:
  LCWindow() = delete;
  explicit LCWindow(QWidget* _widget);
  virtual ~LCWindow();
  virtual void show(EShowMode _mode) override;
  virtual void hide() override;
  virtual void action(const QString& _action) override;
  virtual bool validateAction(
      const QString& _action, QString* msg = nullptr) override;
  virtual void addActionShow(TAction _action) override;
  virtual void addActionHide(TAction _action) override;

  void setTitle(const QString& _title);
  void setShowMode(EShowMode _showMode);
  void setModality(bool _flag);
  void setFlags(Qt::WindowFlags _flags);
  void setSize(const QSize& _size);
  void setPosition(const QPoint& _pos);
  QSize getSize();

  static EShowMode stringToShowMode(const QString& _modeString, bool* _flag = nullptr);

};


#endif /* LCWINDOW_H_ */
