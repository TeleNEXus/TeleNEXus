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
#ifndef LCKEYBOARD_H_
#define LCKEYBOARD_H_

#include "LIKeyboard.h"


class LIWindow;
class LIRemoteDataSource;
class LIApplication;

class LCKeyboard : public LIKeyboard
{
public:
  using TRemoteSource = QSharedPointer<LIRemoteDataSource>;
private:
  void* mpLocalData;
private:
    LCKeyboard() = delete;
    LCKeyboard( const QString& _windowId, 
        const LIApplication& _app,
        TRemoteSource _streamSource, const QString& _streamName,
        TRemoteSource _dataSource, const QString& _dataName);
public:
    virtual ~LCKeyboard();
    virtual QSharedPointer<LIKeyboardListener> 
      createListener( 
          LTAction _change, 
          LTAction _enter, 
          LTAction _disconnect,
          const QValidator* _validator) override;

    bool init();

    static QSharedPointer<LCKeyboard> create(
        const QString& _windowId, 
        const LIApplication& _app,
        TRemoteSource _streamSource, const QString& _streamName,
        TRemoteSource _dataSource, const QString& _dataName);

    friend class LCKeyboardListener;
};

#endif /* LCKEYBOARD_H_ */
