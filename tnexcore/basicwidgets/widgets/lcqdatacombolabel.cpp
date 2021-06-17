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
#include "lcqdatacombolabel.h"
#include "LIMovieAccess.h"

#include <QMap>
#include <QMovie>
#include <qnamespace.h>
#include <QLabel>

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

//==============================================================================SOwnData
//Собственные данные класса.
struct SOwnData
{
  QWidget* undefItem; //Сущность для неопределенного состояния данных.
  QWidget* wrongItem; //Сущность для данных для которых нет соответствия.
  //Карта соответствий прочитанных данных и выводимых строк.
  QMap<QString, QWidget*> normalItemMap;
  SOwnData(): undefItem(nullptr), wrongItem(nullptr){}
};


//------------------------------------------------------------------------------
#define L_OWNDATA (static_cast<SOwnData*>(mpOwnData))

//==============================================================================LCQDataComboLabel
LCQDataComboLabel::LCQDataComboLabel(const QString& _dataName,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QWidget* _parent) :    QStackedWidget(_parent),
  mspFormatter(_formatter)
{
  using EReadStatus = LIRemoteDataReader::EReadStatus;

  mpOwnData = new SOwnData();

  addItemUndef(new QLabel("Undef"));
  addItemWrong(new QLabel("Wrong"));

  auto read_handler = 
    [this](QSharedPointer<QByteArray> _data, EReadStatus _status)
    {
      if(mFlagActive)
      {
        if(_status != EReadStatus::Valid)
        {
          if(L_OWNDATA->undefItem != nullptr)
          {
            if(currentWidget() != L_OWNDATA->undefItem) 
              setCurrentWidget(L_OWNDATA->undefItem);
          }
          setEnabled(false);
          return;
        }
        setEnabled(true);
        auto it = L_OWNDATA->normalItemMap.find( mspFormatter->toString(*_data));
        if(it == L_OWNDATA->normalItemMap.end())
        {
          if(L_OWNDATA->wrongItem != nullptr)
          {
            if(currentWidget() != L_OWNDATA->wrongItem) 
              setCurrentWidget(L_OWNDATA->wrongItem);
          }
          return;
        }
        if(it.value() != currentWidget())
        {
          setCurrentWidget(it.value());
        }
      }
    };

  mDataReader = _dataSource->createReader(_dataName);

  if(!mDataReader.isNull())
  {
    mDataReader->setHandler(read_handler);
  }
  else
  {
    mDataReader = QSharedPointer<LIRemoteDataReader>(new CReaderStub);
  }

  setEnabled(false);
}


//------------------------------------------------------------------------------~LCQDataComboLabel
LCQDataComboLabel::~LCQDataComboLabel()
{
  delete L_OWNDATA;
}

//------------------------------------------------------------------------------setActive
void LCQDataComboLabel::setActive(bool _flag)
{
  if(_flag)
  {
    mDataReader->connectToSource();
    mFlagActive = true;
  }
  else
  {
    mDataReader->disconnectFromSource();
    setEnabled(false);
    mFlagActive = false;
  }
}

//------------------------------------------------------------------------------addItem
void LCQDataComboLabel::addItem(QWidget* _widget, const QString&  _val)
{
  auto it = L_OWNDATA->normalItemMap.find(_val);

  if(it != L_OWNDATA->normalItemMap.end())
  {
    if(it.value() == _widget) return;
    removeWidget(it.value());
    L_OWNDATA->normalItemMap.erase(it);
    it.value()->deleteLater();
  }
  L_OWNDATA->normalItemMap.insert(_val, _widget);
  addWidget(_widget);
}

//------------------------------------------------------------------------------addItemUndef
void LCQDataComboLabel::addItemUndef(QWidget* _widget)
{
  if(L_OWNDATA->undefItem != nullptr)
  {
    removeWidget(L_OWNDATA->undefItem);
    L_OWNDATA->undefItem->deleteLater();
  }
  L_OWNDATA->undefItem = _widget;
  addWidget(_widget);
  setCurrentWidget(_widget);
}

//------------------------------------------------------------------------------addItemWrong
void LCQDataComboLabel::addItemWrong(QWidget* _widget)
{
  if(L_OWNDATA->wrongItem != nullptr)
  {
    removeWidget(L_OWNDATA->wrongItem);
    L_OWNDATA->wrongItem->deleteLater();
  }
  L_OWNDATA->wrongItem = _widget;
  addWidget(_widget);
}

//------------------------------------------------------------------------------event
bool LCQDataComboLabel::event(QEvent *_event)
{
  bool ret = false;
  switch(_event->type())
  {
  case QEvent::Type::Show:
    setActive(true);
    ret = true;
    break;
  case QEvent::Type::Hide:
    setActive(false);
    ret = true;
    break;
  default:
    break;
  }
  QStackedWidget::event(_event);
  return ret;
}

