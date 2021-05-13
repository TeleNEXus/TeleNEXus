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
#include "lcqremlineedit.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include "LIKeyboard.h"
#include "LIKeyboardListener.h"
#include <QDomElement>
#include <QDebug>

//==============================================================================
class CQEventFilter : public QObject
{
private:
  QSharedPointer<LIKeyboardListener> mListener;
  CQEventFilter() : QObject(nullptr) {}

public:
  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {
    if(_event->type() == QEvent::Type::MouseButtonPress)
    {
      auto line_edit = dynamic_cast<QLineEdit*>(_obj);
      if(line_edit == nullptr) return false;
      mListener->connect(line_edit->text());
      qDebug() << "Remote Line Edit mouse button press";
    }
    return false;
  }

  static void install(
      QSharedPointer<LIKeyboardListener> _listener, QLineEdit* _lineEdit)
  {
    qDebug() << "+++++++++++++++++++RemLineEdit install event filter";
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
  /* QString format  = "format"; */
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
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QString data;
  QString attr = element.attribute(__slAttributes.source);
  QSharedPointer<LIRemoteDataSource> source;
  QSharedPointer<LIDataFormatter> format;

  auto ret_widget = 
    [_buildData](QLineEdit* _le = nullptr)
    {
      qDebug() << "+++++++++++++++++++ Build RemLineEdit 0";
      if(_le == nullptr) _le = new QLineEdit(_buildData->element.tagName());
      qDebug() << "+++++++++++++++++++ Build RemLineEdit 1";
      QString style = LCBuildersCommon::getBaseStyleSheet(_buildData->element, _buildData->application);
      _le->setStyleSheet(style);
      LCBuildersCommon::initPosition(_buildData->element, *_le);

      //-----------------------------keyboard
      QString keyboard_id = _buildData->element.attribute(__slAttributes.keyboard);
      if(keyboard_id.isNull()) return _le;
      qDebug() << "+++++++++++++++++++ Build RemLineEdit 2 keyboard id = "<< keyboard_id;
      auto keyboard = _buildData->application.getKeyboard(keyboard_id);
      if(keyboard.isNull()) return _le;

      qDebug() << "+++++++++++++++++++ Build RemLineEdit 3";


      /* QObject::connect(_le, QLineEdit:: */





      auto action_change =
        [_le](const QString& _str)
        {
        };

      auto action_enter =
        [_le](const QString& _str)
        {
          _le->setText(_str);
        };

      auto action_disconnect =
        [](const QString& _str)
        {
        };

      auto listener = keyboard->createListener(action_change, action_enter, action_disconnect);
      CQEventFilter::install(listener, _le);
      return _le;
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

  return ret_widget(new LCQRemLineEdit(data, data, source, format));
}



