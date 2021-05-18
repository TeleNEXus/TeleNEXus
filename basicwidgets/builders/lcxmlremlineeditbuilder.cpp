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
#include "lcxmlremlineeditbuilder.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include "LIKeyboard.h"
#include "LIKeyboardListener.h"
#include "LIDataFormatter.h"
#include "LIRemoteDataWriter.h"
#include "LIRemoteDataReader.h"

#include <QLineEdit>
#include <QDomElement>
#include <QDebug>
#include <QKeyEvent>
#include <QMap>
#include <functional>
#include <qnamespace.h>

//==============================================================================
class CQControlBase : public QObject
{
protected:
  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIRemoteDataWriter>  mDataWriter;
  QSharedPointer<LIDataFormatter>     mFormatter;
  QLineEdit* mpLineEdit;
  bool mFlagUpdateOn = false;

  QMap<QEvent::Type, std::function<bool(QObject*, QEvent*)>> mEventsMap;


  CQControlBase() = delete;

  CQControlBase(
    const QString& _dataNameRead,
    const QString& _dataNameWrite,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QLineEdit* _lineEdit) :  
    QObject(_lineEdit), mFormatter(_formatter), mpLineEdit(_lineEdit)
  {
    QString str = "Undef";
    mpLineEdit->setText(str);
    mpLineEdit->setValidator(_formatter->validator());
    mpLineEdit->setEnabled(false);

    mDataReader = _dataSource->createReader(_dataNameRead,
        [this, str](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
        {
          if(mFlagUpdateOn)
          {
            if(_status != LERemoteDataStatus::DS_OK)
            {
              mpLineEdit->setText(str);
              mpLineEdit->setEnabled(false);
              return;
            }
            mpLineEdit->setEnabled(true);
            mpLineEdit->setText(mFormatter.data()->toString(*_data));
          }
        });
    mDataWriter = _dataSource->createWriter(_dataNameWrite);


    //init events map
    mEventsMap.insert(QEvent::Type::Show,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          setActive(true);
          return false;
        });

    mEventsMap.insert(QEvent::Type::Hide,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          setActive(false);
          return false;
        });

    mEventsMap.insert(QEvent::Type::KeyPress,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          QKeyEvent* ke = dynamic_cast<QKeyEvent*>(_event);

          if(ke == nullptr) return false;

          switch(ke->key())
          {
          case Qt::Key::Key_Enter:
          case Qt::Key::Key_Return:
            mDataWriter->writeRequest(mFormatter->toBytes(mpLineEdit->text()));
            mDataReader->readRequest();
            mFlagUpdateOn = true;
          break;

          case Qt::Key::Key_Escape:
            mpLineEdit->clearFocus();
            mFlagUpdateOn = false;
            break;

          default:
            mFlagUpdateOn = false;
            break;
          }
          return false;
        });

    mEventsMap.insert(QEvent::Type::FocusIn,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          mFlagUpdateOn = false;
          return false;
        });

    mEventsMap.insert(QEvent::Type::FocusOut,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          mFlagUpdateOn = true;
          mDataReader->readRequest();
          return false;
        });
  }

  void  setActive(bool _flag)
  {
    if(_flag)
    {
      mDataReader->connectToSource();
      mFlagUpdateOn = true;
    }else
    {
      mFlagUpdateOn = false;
      mpLineEdit->setEnabled(false);
      mDataReader->disconnectFromSource();
    }
  }

public:

  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {
    Q_UNUSED(_obj);
    auto it = mEventsMap.find(_event->type());
    if(it == mEventsMap.end()) return false;
    return it.value()(_obj, _event);
  }

  static void install(
    const QString& _dataNameRead,
    const QString& _dataNameWrite,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QLineEdit* _lineEdit)
  {
    auto ctrl = new CQControlBase( 
        _dataNameRead, _dataNameWrite, _dataSource, _formatter, _lineEdit);
    _lineEdit->installEventFilter(ctrl);
  }

};

//==============================================================================
class CQControlKeyboard : public CQControlBase
{
private:
  QSharedPointer<LIKeyboardListener> mKeyboardListener;
  CQControlKeyboard() = delete;
  CQControlKeyboard(
    const QString& _dataNameRead,
    const QString& _dataNameWrite,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QLineEdit* _lineEdit,
    QSharedPointer<LIKeyboard> _keyboard) :  
    CQControlBase(
        _dataNameRead, _dataNameWrite, _dataSource, _formatter, _lineEdit)
  {

    //-----------------------------keyboard

    auto action_change =
      [_lineEdit](const QString& _str)
      {
        _lineEdit->setText(_str);
      };

    auto action_enter =
      [this](const QString& _str)
      {
        mDataWriter->writeRequest(mFormatter->toBytes(_str));
      };

    auto action_disconnect =
      [this](const QString& _str)
      {
        Q_UNUSED(_str);
        setActive(true);
      };

    QValidator* validator = nullptr; 
    if(!mFormatter.isNull())
    {
      validator = mFormatter->validator();
    }

    mKeyboardListener = _keyboard->createListener(
        action_change, action_enter, action_disconnect, validator);


    mEventsMap.insert(QEvent::Type::FocusIn,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          mpLineEdit->clearFocus();
          return true;
        });

    mEventsMap.insert(QEvent::Type::MouseButtonPress,
        [this](QObject* _obj, QEvent* _event)
        {
          Q_UNUSED(_obj);
          Q_UNUSED(_event);
          if(mKeyboardListener->connect(mpLineEdit->text())) setActive(false);
          return true;
        });
  }
public:
  static void install(
    const QString& _dataNameRead,
    const QString& _dataNameWrite,
    QSharedPointer<LIRemoteDataSource> _dataSource,
    QSharedPointer<LIDataFormatter> _formatter,
    QLineEdit* _lineEdit,
    QSharedPointer<LIKeyboard> _keyboard)
  {
    auto ctrl = new CQControlKeyboard( 
        _dataNameRead, _dataNameWrite, _dataSource, _formatter, _lineEdit, _keyboard);
    _lineEdit->installEventFilter(ctrl);
  }
};


//==============================================================================
class CQEventFilter : public QObject
{
private:
  QSharedPointer<LIKeyboardListener> mListener;
  CQEventFilter() : QObject(nullptr) {}

public:
  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {
    bool ret = false;
    auto line_edit = dynamic_cast<QLineEdit*>(_obj);
    if(line_edit == nullptr) return ret;

    switch(_event->type())
    {
    case QEvent::Type::MouseButtonPress:
        mListener->connect(line_edit->text());
        ret = true;
      break;

    case QEvent::Type::FocusIn:
      line_edit->clearFocus();
      ret = true;
      break;

    default:
      break;
    }
    return ret;
  }

  static void install(
      QSharedPointer<LIKeyboardListener> _listener, QLineEdit* _lineEdit)
  {
    auto filter = new CQEventFilter();
    filter->mListener = _listener;
    filter->setParent(_lineEdit);
    _lineEdit->installEventFilter(filter);
  }
};

//==============================================================================
const struct
{
  QString data    = "data";
  QString source  = "source";
  QString keyboard = "keyboard";
} __slAttributes;

//==============================================================================
LCXmlRemLineEditBuilder::LCXmlRemLineEditBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlRemLineEditBuilder::~LCXmlRemLineEditBuilder()
{

}


//------------------------------------------------------------------------------
QWidget* LCXmlRemLineEditBuilder::buildLocal(
      QSharedPointer<SBuildData> _buildData)
{

  QLineEdit* line_edit = new QLineEdit(_buildData->element.tagName());

  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QString data;
  QString attr = element.attribute(__slAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  auto ret_widget = 
    [_buildData, line_edit]()
    {
      QString style = LCBuildersCommon::getBaseStyleSheet(_buildData->element, _buildData->application);
      line_edit->setStyleSheet(style);
      LCBuildersCommon::initPosition(_buildData->element, *line_edit);

      /* //-----------------------------keyboard */
      /* QString keyboard_id = _buildData->element.attribute(__slAttributes.keyboard); */
      /* if(keyboard_id.isNull()) return _le; */
      /* auto keyboard = _buildData->application.getKeyboard(keyboard_id); */
      /* if(keyboard.isNull()) return _le; */

      /* auto action_change = */
      /*   [_le](const QString& _str) */
      /*   { */
      /*   }; */

      /* auto action_enter = */
      /*   [_le](const QString& _str) */
      /*   { */
      /*     _le->setText(_str); */
      /*   }; */

      /* auto action_disconnect = */
      /*   [](const QString& _str) */
      /*   { */
      /*   }; */

      /* auto listener = keyboard->createListener(action_change, action_enter, action_disconnect); */

      /* CQEventFilter::install(listener, _le); */

      return line_edit;
    };

  if(attr.isNull())
  {
    return ret_widget();
  }

  source = app.getDataSource(attr);

  if(source.isNull())
  {
    return ret_widget();
  }

  data = element.attribute(__slAttributes.data);

  if(data.isNull())
  {
    return ret_widget();
  }

  attr = element.attribute(LCBuildersCommon::mAttributes.dataformatter);
  format = _buildData->application.getStdDataFormatter(attr);


  if(format.isNull())
  {
    attr = element.attribute(LCBuildersCommon::mAttributes.dataformatterid);
    format = _buildData->application.getDataFormatter(attr);
    if(format.isNull()) return ret_widget();
  }

  auto get_keyboard = 
    [&_buildData]()
  {
    QString keyboard_id = _buildData->element.attribute(__slAttributes.keyboard);
    if(keyboard_id.isNull()) return QSharedPointer<LIKeyboard>();
    return  _buildData->application.getKeyboard(keyboard_id);
  };

  auto keyboard = get_keyboard();

  if(keyboard.isNull())
  {
    CQControlBase::install(data,data,source,format,line_edit);
  }
  else
  {
    CQControlKeyboard::install(data,data,source,format,line_edit,keyboard);
  }

  return ret_widget();
}



