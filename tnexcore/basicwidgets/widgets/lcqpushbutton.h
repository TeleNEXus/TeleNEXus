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
#ifndef LCQPUSHBUTTON_H_
#define LCQPUSHBUTTON_H_
#include <QPushButton>

class LCQPushButton : public QPushButton
{
private:
  void* mpLocal;
public:
  explicit LCQPushButton(QWidget* _parent = nullptr);
  explicit LCQPushButton(const QString& _text, 
      QWidget* _parent = nullptr);
  virtual ~LCQPushButton();
  virtual bool event(QEvent* _event) override;
  void setPushDelay(int _msec);
};

#endif /* LCQPUSHBUTTON_H_ */
