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

#include "lckeyboard.h"
#include "LIKeyboardListener.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataWriter.h"
#include "LIWindow.h"
#include "LIApplication.h"
#include <QList>
#include <functional>
#include <QDebug>
#include <QValidator>

using TWindow = QWeakPointer<LIWindow>; 
using TRemoteSource = LCKeyboard::TRemoteSource;

using LTAction = LCKeyboard::LTAction;


using TRemoteReader = QSharedPointer<LIRemoteDataReader>;
using TRemoteWriter = QSharedPointer<LIRemoteDataWriter>;

using EReadStatus = LIRemoteDataReader::EReadStatus;

//==============================================================================CReaderStub
class CReaderStub : public LIRemoteDataReader
{
public:
  CReaderStub(){}
  virtual void readRequest()override {}
  virtual void connectToSource()override {}
  virtual void disconnectFromSource()override {}
  virtual void setHandler(THandler)override {}
};

class LCKeyboardListener;
//==============================================================================SLocalData
struct SLocalData
{
  const LIApplication& application;
  QString windowId;
  TWindow wpWindow;
  TRemoteReader streamReader; 
  TRemoteWriter dataWriter;
  QString currentData;
  QWeakPointer<LCKeyboardListener> listener;
  QWeakPointer<LCKeyboard> ownPointer;
  QMap<QString,std::function<void(void)>> controlActions;
  SLocalData(const LIApplication& _app) : application(_app){}
};

#define toLocalData(p) (reinterpret_cast<SLocalData*>(p))
#define ld (*(toLocalData(mpLocalData)))

//==============================================================================LCKeyboardListener
class LCKeyboardListener : public LIKeyboardListener
{
public:
  QWeakPointer<LCKeyboardListener> mwpOwn;
  QWeakPointer<LCKeyboard> mwpKeyboard;
  LTAction mfActionChange;
  LTAction mfActionEnter;
  LTAction mfActionDisconnect;
  const QValidator* mpValidator;

public: 

  LCKeyboardListener(){} 

  virtual bool connect(const QString& _currData) override
  {
    auto keyboard = mwpKeyboard.lock();
    if(keyboard.isNull()) return false;
    auto kd = toLocalData(keyboard->mpLocalData);
    if(!kd->listener.isNull()) return false;
    auto window = kd->wpWindow.lock();
    if(window.isNull()) return false;
    window->show();
    kd->listener = mwpOwn;
    kd->dataWriter->writeRequest(_currData.toUtf8());
    kd->currentData = _currData;
    return true;
  }
};

//==============================================================================LCKeyboard
LCKeyboard::LCKeyboard( const QString& _windowId, 
    const LIApplication& _app,
    TRemoteSource _streamSource, const QString& _streamName,
    TRemoteSource _dataSource, const QString& _dataName)
{
  mpLocalData = new SLocalData(_app);

  ld.windowId = _windowId;

  //--------------------------------------------------add control actions
  ld.controlActions.insert(QStringLiteral("[Enter]"),
        [this]()
        {
          auto listener = ld.listener.lock();
          if(listener.isNull()) return;
          listener->mfActionEnter(ld.currentData);
        });

  ld.controlActions.insert(QStringLiteral("[Clear]"),
        [this]()
        {
          auto listener = ld.listener.lock();
          if(listener.isNull()) return;
          ld.currentData.clear();
          listener->mfActionChange(ld.currentData);
          ld.dataWriter->writeRequest(QStringLiteral("\0").toUtf8());
        });

  ld.controlActions.insert(QStringLiteral("[Backspace]"),
        [this]()
        {
          auto listener = ld.listener.lock();
          if(listener.isNull()) return;

          int size = ld.currentData.size();

          if(size > 1)
          {
            ld.currentData.resize(size - 1);
            ld.dataWriter->writeRequest(ld.currentData.toUtf8());
            listener->mfActionChange(ld.currentData);
          }
          else
          {
            ld.currentData.clear();
            listener->mfActionChange(ld.currentData);
            ld.dataWriter->writeRequest(QStringLiteral("\0").toUtf8());
          }
        });
  //--------------------------------------------------


  auto stream_read_handler = 
      [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
      {
        if(_status != EReadStatus::Valid) return;
        QString instr = QString::fromUtf8(*_data);
        auto control_action_iterator = ld.controlActions.find(instr);
        if(control_action_iterator != ld.controlActions.end())
        {
          control_action_iterator.value()();
          return;
        }

        auto listener = ld.listener.lock();
        if(listener.isNull()) return;

        auto validator = listener->mpValidator;

        if(validator)
        {
          instr = ld.currentData + instr;
          int pos;
          switch(validator->validate(instr, pos))
          {
          case QValidator::State::Acceptable:
          case QValidator::State::Intermediate:
            ld.currentData = instr;
            break;

          case QValidator::State::Invalid:
            break;
          }
        }
        else
        {
          ld.currentData.append(instr.at(0));
        }

        ld.dataWriter->writeRequest(ld.currentData.toUtf8());
        listener->mfActionChange(ld.currentData);
      };

  ld.streamReader = _streamSource->createReader(_streamName);
  if(!ld.streamReader.isNull())
  {
    ld.streamReader->setHandler(stream_read_handler);
    ld.dataWriter = _dataSource->createWriter(_dataName);
  }
  else
  {
    ld.streamReader = QSharedPointer<LIRemoteDataReader>(new CReaderStub);
  }
}

LCKeyboard::~LCKeyboard()
{
  delete &ld;
}

//------------------------------------------------------------------------------
QSharedPointer<LIKeyboardListener> LCKeyboard::createListener( 
    LTAction _change, 
    LTAction _enter, 
    LTAction _disconnect,
    const QValidator* _validator)
{
  auto listener = QSharedPointer<LCKeyboardListener>(new LCKeyboardListener);
  listener->mwpKeyboard = ld.ownPointer;
  listener->mwpOwn = listener;
  listener->mfActionChange = _change;
  listener->mfActionEnter = _enter;
  listener->mfActionDisconnect = _disconnect;
  listener->mpValidator = _validator;
  return listener;
}

//------------------------------------------------------------------------------
bool LCKeyboard::init()
{

  ld.wpWindow = ld.application.getWindow(ld.windowId);
  auto window = ld.wpWindow.lock();
  if(window.isNull())
  {
    return false;
  }

  auto ownPointer = ld.ownPointer;

  window->addActionShow(
      [ownPointer]()
      {
        auto sp_own = ownPointer.lock();
        if(sp_own.isNull()) return;
        toLocalData(sp_own->mpLocalData)->streamReader->connectToSource();
      });

  window->addActionHide(
      [ownPointer]()
      {
        auto sp_own = ownPointer.lock();
        if(sp_own.isNull()) return;
        SLocalData* kld = toLocalData(sp_own->mpLocalData);
        kld->streamReader->disconnectFromSource();
        if(kld->listener.isNull()) return;
        auto listener = kld->listener.lock();
        if(!listener.isNull()) 
        {
          listener->mfActionDisconnect(kld->currentData);
        }
        kld->listener.clear();
      });
  return true;
}

//------------------------------------------------------------------------------
QSharedPointer<LCKeyboard> LCKeyboard::create(
    const QString& _windowId, 
    const LIApplication& _app,
    TRemoteSource _streamSource, const QString& _streamName,
    TRemoteSource _dataSource, const QString& _dataName)
{
  auto keyboard = QSharedPointer<LCKeyboard>(
      new LCKeyboard(_windowId, 
        _app,
        _streamSource, _streamName, 
        _dataSource, _dataName));
  toLocalData(keyboard->mpLocalData)->ownPointer = keyboard;
  return keyboard;
}

