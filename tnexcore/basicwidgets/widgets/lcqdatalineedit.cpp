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
#include "lcqdatalineedit.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIDataFormatter.h"
#include "LIKeyboardListener.h"
#include "LIKeyboard.h"

#include <QDebug>
#include <QMap>
#include <QEvent>
#include <functional>
#include <QKeyEvent>


//==============================================================================
struct SLocalData
{
  QSharedPointer<LIRemoteDataReader>  dataReader;
  QSharedPointer<LIDataFormatter>     readFormatter;
  QSharedPointer<LIRemoteDataWriter>  dataWriter;
  QSharedPointer<LIDataFormatter>     writeFormatter;
  bool flagUpdateOn = false;
  QMap<QEvent::Type, std::function<bool(QEvent*)>> eventMap;
  QSharedPointer<LIKeyboardListener>  keyboardListener;
  SLocalData(
    QSharedPointer<LIRemoteDataReader>  _dataReader,
    QSharedPointer<LIDataFormatter>     _readFormatter,
    QSharedPointer<LIRemoteDataWriter>  _dataWriter,
    QSharedPointer<LIDataFormatter>     _writeFormatter) :
    dataReader(_dataReader),
    readFormatter(_readFormatter),
    dataWriter(_dataWriter),
    writeFormatter(_writeFormatter){}
};


#define toLocalData(p) (static_cast<SLocalData*>(p))
#define ld (*toLocalData(mpLocal))

//==============================================================================
LCQDataLineEdit::LCQDataLineEdit(
    QSharedPointer<LIRemoteDataReader>  _dataReader,
    QSharedPointer<LIDataFormatter>     _readFormatter,
    QSharedPointer<LIRemoteDataWriter>  _dataWriter,
    QSharedPointer<LIDataFormatter>     _writeFormatter,
    QWidget* _parent) :
  QLineEdit(_parent)
{
  mpLocal = new SLocalData(
      _dataReader, 
      _readFormatter, 
      _dataWriter, 
      _writeFormatter);
  
  QString str = "Undef";
  setText(str);
  setValidator(ld.writeFormatter->validator());
  setEnabled(false);

  ld.dataReader->setHandler(
      [this, str](QSharedPointer<QByteArray> _data, 
        LIRemoteDataReader::EReadStatus _status)
      {
        if(ld.flagUpdateOn)
        {
          if(_status != LIRemoteDataReader::EReadStatus::Valid)
          {
            setText(str);
            setEnabled(false);
            return;
          }
          setEnabled(true);
          setText(ld.readFormatter.data()->toString(*_data));
        }
      });


  //init events map
  ld.eventMap.insert(QEvent::Type::Show,
      [this](QEvent* _event)
      {
        setActive(true);
        return QLineEdit::event(_event);
      });

  ld.eventMap.insert(QEvent::Type::Hide,
      [this](QEvent* _event)
      {
        setActive(false);
        return QLineEdit::event(_event);
      });

  ld.eventMap.insert(QEvent::Type::KeyPress,
      [this](QEvent* _event)
      {
        QKeyEvent* ke = dynamic_cast<QKeyEvent*>(_event);

        if(ke == nullptr) return QLineEdit::event(_event);


        switch(ke->key())
        {
        case Qt::Key::Key_Enter:
        case Qt::Key::Key_Return:
          ld.dataWriter->writeRequest(
              ld.writeFormatter->toBytes(text()));
          ld.dataReader->readRequest();
          ld.flagUpdateOn = true;
          break;

        case Qt::Key::Key_Escape:
          this->clearFocus();
          ld.flagUpdateOn = false;
          break;

        default:
          ld.flagUpdateOn = false;
          break;
        }
        return QLineEdit::event(_event);
      });

  ld.eventMap.insert(QEvent::Type::FocusIn,
      [this](QEvent* _event)
      {
        ld.flagUpdateOn = false;
        return QLineEdit::event(_event);
      });

  ld.eventMap.insert(QEvent::Type::FocusOut,
      [this](QEvent* _event)
      {
        ld.flagUpdateOn = true;
        ld.dataReader->readRequest();
        return QLineEdit::event(_event);
      });
}

//------------------------------------------------------------------------------
LCQDataLineEdit::~LCQDataLineEdit()
{
  delete toLocalData(mpLocal);
}

//------------------------------------------------------------------------------
void LCQDataLineEdit::setActive(bool _flag)
{
  if(_flag)
  {
    ld.dataReader->connectToSource();
    ld.flagUpdateOn = true;
  }else
  {
    ld.flagUpdateOn = false;
    setEnabled(false);
    ld.dataReader->disconnectFromSource();
  }
}


//------------------------------------------------------------------------------
void LCQDataLineEdit::installKeyboard(QSharedPointer<LIKeyboard> _keyboard)
{
  auto action_change =
    [this](const QString& _str)
    {
      setText(_str);
    };

  auto action_enter =
    [this](const QString& _str)
    {
      ld.dataWriter->writeRequest(
          ld.writeFormatter->toBytes(_str));
    };

  auto action_disconnect =
    [this](const QString&)
    {
      setActive(true);
    };

  QValidator* validator = nullptr; 
  if(!ld.writeFormatter.isNull())
  {
    validator = ld.writeFormatter->validator();
  }

  ld.keyboardListener = _keyboard->createListener(
      action_change, action_enter, action_disconnect, validator);


  ld.eventMap.insert(QEvent::Type::FocusIn,
      [this](QEvent*)
      {
        clearFocus();
        return true;
      });

  ld.eventMap.insert(QEvent::Type::MouseButtonPress,
      [this](QEvent*)
      {
        if(ld.keyboardListener->connect(text())) setActive(false);
        return true;
      });
}

//------------------------------------------------------------------------------
bool LCQDataLineEdit::event(QEvent* _event)
{
  auto it = ld.eventMap.find(_event->type());
  if(it == ld.eventMap.end()) return QLineEdit::event(_event);
  return it.value()(_event);
}

