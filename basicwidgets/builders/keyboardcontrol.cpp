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
#include "lcqkeyboardcontrol.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"
#include "LIApplication.h"
#include "LIDataFormatter.h"
#include "LIKeyboard.h"
#include <QDomElement>
#include <QWidget>
#include <QByteArray>
#include <QDebug>
#include <QEvent>

static const struct
{
  QString keyboard = "keyboard";
}__slAttributes;

//==============================================================================
class CLocalData
{
public:
  QWidget* mpWidget = nullptr;
  QSharedPointer<LIKeyboard> mspKeyboard;
  QSharedPointer<LIRemoteDataReader> mspStreamReader;
  QSharedPointer<LIDataFormatter> mspDataFormatter;
  
public:
  CLocalData() = delete;
  CLocalData(QWidget* _widget, QSharedPointer<LIKeyboard> _keyboard):
    mpWidget(_widget),
    mspKeyboard(_keyboard),
    mspDataFormatter(_keyboard->getDataFormatter())
  {
  }
};

#define toLocalData(p) (reinterpret_cast<CLocalData*>(p))
#define ld (*toLocalData(mpLocal))

//==============================================================================

LCQKeyboardControl::LCQKeyboardControl(void* _localData) : 
  QObject(toLocalData(_localData)->mpWidget),
  mpLocal(_localData)
{

  auto source = ld.mspKeyboard->getStreamSource();

  ld.mspStreamReader = source->createReader(ld.mspKeyboard->getStreamName(),
      [this](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
      {
      });

  ld.mpWidget->installEventFilter(this);
}

LCQKeyboardControl::~LCQKeyboardControl()
{
  delete reinterpret_cast<CLocalData*>(mpLocal);
}

//------------------------------------------------------------------------------
bool LCQKeyboardControl::build(const QDomElement& _element, 
    QWidget* _widget,
    const LIApplication& _app)
{
  QString attr_keyboard = _element.attribute(__slAttributes.keyboard);
  if(attr_keyboard.isNull()) return false;
  auto keyboard = _app.getKeyboard(attr_keyboard);
  if(keyboard.isNull()) return false;

  CLocalData* local = new CLocalData(_widget, keyboard);

  new LCQKeyboardControl(local);

  return true;
}

//------------------------------------------------------------------------------
bool LCQKeyboardControl::eventFilter(QObject* _obj, QEvent* _event)
{
  return false;
}




