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
#include "lcxmldatalineeditbuilder.h"
#include "xmlcommon.h"
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
  QSharedPointer<LIDataFormatter>     mDataReadFormatter;

  QSharedPointer<LIRemoteDataWriter>  mDataWriter;
  QSharedPointer<LIDataFormatter>     mDataWriteFormatter;

  QLineEdit* mpLineEdit;
  bool mFlagUpdateOn = false;

  QMap<QEvent::Type, std::function<bool(QObject*, QEvent*)>> mEventsMap;


  CQControlBase() = delete;

  CQControlBase(
      const QString& _dataReadName,
      QSharedPointer<LIRemoteDataSource>  _dataReadSource,
      QSharedPointer<LIDataFormatter>     _dataReadFormatter,
      const QString& _dataWriteName,
      QSharedPointer<LIRemoteDataSource>  _dataWriteSource,
      QSharedPointer<LIDataFormatter>     _dataWriteFormatter,
      QLineEdit* _lineEdit) :  
    QObject(_lineEdit), 
    mDataReadFormatter(_dataReadFormatter),
    mDataWriteFormatter(_dataWriteFormatter),
    mpLineEdit(_lineEdit)
  {

    QString str = "Undef";
    mpLineEdit->setText(str);
    mpLineEdit->setValidator(_dataWriteFormatter->validator());
    mpLineEdit->setEnabled(false);

    mDataReader = _dataReadSource->createReader(_dataReadName,
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
            mpLineEdit->setText(mDataReadFormatter.data()->toString(*_data));
          }
        });

    mDataWriter = _dataWriteSource->createWriter(_dataWriteName);


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
            writeRequest(mpLineEdit->text());
            readRequest();
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
          readRequest();
          return false;
        });
  }

  void  setActive(bool _flag)
  {
    if(mDataReader.isNull()) return;
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

protected:

  void readRequest()
  {
    if(!mDataReader.isNull()) mDataReader->readRequest();
  }

  void writeRequest(const QString _text)
  {
    if(mDataWriter.isNull()) return;
    if(mDataWriteFormatter.isNull()) return;
    mDataWriter->writeRequest(
        mDataWriteFormatter->toBytes(_text));
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
      const QString& _dataReadName,
      QSharedPointer<LIRemoteDataSource> _dataReadSource,
      QSharedPointer<LIDataFormatter> _dataReadFormatter,
      const QString& _dataWriteName,
      QSharedPointer<LIRemoteDataSource> _dataWriteSource,
      QSharedPointer<LIDataFormatter> _dataWriteFormatter,
      QLineEdit* _lineEdit)
  {
    auto ctrl = new CQControlBase( 
        _dataReadName, 
        _dataReadSource, 
        _dataReadFormatter, 
        _dataWriteName, 
        _dataWriteSource, 
        _dataWriteFormatter, 
        _lineEdit);
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
      const QString&                      _dataReadName,
      QSharedPointer<LIRemoteDataSource>  _dataReadSource,
      QSharedPointer<LIDataFormatter>     _dataReadFormatter,
      const QString&                      _dataWriteName,
      QSharedPointer<LIRemoteDataSource>  _dataWriteSource,
      QSharedPointer<LIDataFormatter>     _dataWriteFormatter,
      QLineEdit*                          _lineEdit,
      QSharedPointer<LIKeyboard>          _keyboard) :  
    CQControlBase(
        _dataReadName, 
        _dataReadSource, 
        _dataReadFormatter, 
        _dataWriteName, 
        _dataWriteSource, 
        _dataWriteFormatter, 
        _lineEdit
        )
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
        writeRequest(_str);
      };

    auto action_disconnect =
      [this](const QString& _str)
      {
        Q_UNUSED(_str);
        setActive(true);
      };

    QValidator* validator = nullptr; 
    if(!mDataWriteFormatter.isNull())
    {
      validator = mDataWriteFormatter->validator();
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
      const QString&                      _dataReadName,
      QSharedPointer<LIRemoteDataSource>  _dataReadSource,
      QSharedPointer<LIDataFormatter>     _dataReadFormatter,
      const QString&                      _dataWriteName,
      QSharedPointer<LIRemoteDataSource>  _dataWriteSource,
      QSharedPointer<LIDataFormatter>     _dataWriteFormatter,
      QLineEdit*                          _lineEdit,
      QSharedPointer<LIKeyboard>          _keyboard)
  {
    auto ctrl = new CQControlKeyboard( 
        _dataReadName, 
        _dataReadSource, 
        _dataReadFormatter, 
        _dataWriteName, 
        _dataWriteSource, 
        _dataWriteFormatter, 
        _lineEdit,
        _keyboard);
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
  QString data = "data";
  QString dataRead = "dataRead";
  QString dataWrite = "dataWrite";
  QString keyboard = "keyboard";
} __slAttributes;

//==============================================================================
LCXmlDataLineEditBuilder::LCXmlDataLineEditBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlDataLineEditBuilder::~LCXmlDataLineEditBuilder()
{

}


//------------------------------------------------------------------------------
QWidget* LCXmlDataLineEditBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  QLineEdit* line_edit = new QLineEdit(_element.tagName());

  QString data_read_name;
  QString data_write_name;
  QSharedPointer<LIRemoteDataSource> read_source;
  QSharedPointer<LIRemoteDataSource> write_source;
  QSharedPointer<LIDataFormatter> read_formatter;
  QSharedPointer<LIDataFormatter> write_formatter;

  auto ret_wrong = 
    [line_edit]()
    {
      return line_edit;
    };

  auto ret_ok = 
    [ 
    &_element, 
    &_app, 
    &data_read_name,
    &data_write_name,
    &read_source,
    &write_source,
    &read_formatter,
    &write_formatter,
    line_edit
    ]()
    {
      auto keyboard = _app.getKeyboard(
          _element.attribute(__slAttributes.keyboard));

      if(keyboard.isNull())
      {
        CQControlBase::install(
            data_read_name,
            read_source,
            read_formatter,
            data_write_name,
            write_source,
            write_formatter,
            line_edit);
      }
      else
      {
        CQControlKeyboard::install(
            data_read_name,
            read_source,
            read_formatter,
            data_write_name,
            write_source,
            write_formatter,
            line_edit,
            keyboard);
      }

      setWidgetName(_element, line_edit);
      setWidgetStyle(_element, line_edit);
      setWidgetSize(_element, line_edit);
      setWidgetPosition(_element, line_edit);
      setWidgetFixedSize(_element, line_edit);

      return line_edit;
    };

  bool error_flag = false;

  auto data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data),
      [&error_flag](const QString&)
      {
        error_flag = true;
      });

  if(!error_flag)
  {
    data_read_name = data_spec.dataId;
    read_source = _app.getDataSource(data_spec.sourceId);
    read_formatter = _app.getDataFormatter(data_spec.formatterId);

    data_write_name = data_read_name;
    write_source = read_source;
    write_formatter = read_formatter;
    return ret_ok();
  }

  error_flag = false;
  data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataRead),
      [&error_flag](const QString&)
      {
        error_flag = true;
      });

  if(error_flag) return ret_wrong();

  data_read_name = data_spec.dataId;
  read_source = _app.getDataSource(data_spec.sourceId);
  read_formatter = _app.getDataFormatter(data_spec.formatterId);


  error_flag = false;
  data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.dataWrite),
      [&error_flag](const QString&)
      {
        error_flag = true;
      });

  if(error_flag) return ret_wrong();

  data_write_name = data_spec.dataId;
  write_source = _app.getDataSource(data_spec.sourceId);
  write_formatter = _app.getDataFormatter(data_spec.formatterId);

  return ret_ok();
}



