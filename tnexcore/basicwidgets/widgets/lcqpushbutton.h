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
#include <QList>
#include <functional>

class LCQPushButton : public QPushButton
{
public:
  using TActionsList = QList<std::function<void(void)>>;
private:
  void* mpLocal;
public:
  LCQPushButton() = delete;

  explicit LCQPushButton(
      const QString& _text,
      const TActionsList& _pushActions,
      const TActionsList& _releaseActions,
      int _pushDelay);

  virtual ~LCQPushButton();
  virtual bool event(QEvent* _event) override;
};

#endif /* LCQPUSHBUTTON_H_ */
