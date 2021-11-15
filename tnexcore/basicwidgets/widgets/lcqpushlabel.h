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

#ifndef LCQPUSLABEL_H_
#define LCQPUSLABEL_H_

#include <QLabel>
#include <QList>
#include <functional>

class QTimer;

class LCQPushLabel : public QLabel
{
  Q_OBJECT;
public:
  using TChangeView = std::function<void(QLabel* _label)>;
  using TActionsList = QList<std::function<void(void)>>;

private:

  enum class EState
  {
    released,
    pressed
  };

  EState mState;
  QTimer* mpTimer;

  TChangeView  mSetViewPressed;
  TChangeView  mSetViewReleased;
  TActionsList mPressActions;
  TActionsList mReleaseActions;

public:

  LCQPushLabel() = delete;

  LCQPushLabel(
      TChangeView _statePush, 
      TChangeView _releaseState,
      const TActionsList& _actionsPush, 
      const TActionsList& _actionsRelease,
      int _pushDelay);

  virtual ~LCQPushLabel();
  virtual bool eventFilter(QObject* _obj, QEvent* _event);
};

#endif /* LCQPUSLABEL_H_ */
