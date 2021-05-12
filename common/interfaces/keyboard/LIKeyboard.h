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
#ifndef LIKEYBOARD_H_ 
#define LIKEYBOARD_H_

#include <QSharedPointer>
#include <functional>

class LIKeyboardListener;

class LIKeyboard 
{
public:

  using LTChangeAction  = std::function<void(const QString&)>;
  using LTControlAction = std::function<void(void)>;

public:
    LIKeyboard(){}
    virtual ~LIKeyboard(){}
    virtual QSharedPointer<LIKeyboardListener> 
      createListener( 
          LTChangeAction  _change, 
          LTControlAction _enter, 
          LTControlAction _disconnect) = 0;
};

#endif // LIKEYBOARD_H_

