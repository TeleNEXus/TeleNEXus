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

#include "uploadkeyboards.h"
#include "lcxmlcommon.h"
#include "LIApplication.h" 
#include "LIKeyboard.h"
#include "LIXmlWidgetBuilder.h"
#include "lcformattertextstream.h"
#include "LIRemoteDataWriter.h"
#include "lcformattertextstream.h"

#include <QKeyEvent>
#include <QApplication>
#include <QMap>
#include <QDomElement>
#include <QWidget>
#include <QDebug>
#include <qnamespace.h>

static const struct
{
  QString streamSource = "streamSource"; 
  QString streamName = "streamName";
}__slAttribubes;


class LCKeyboard : public LIKeyboard
{
private:

  QList<QWidget*> mWidgets;
  QSharedPointer<LIRemoteDataReader> mspStreamReader;
  QSharedPointer<LIRemoteDataWriter> mspDataWriter;

public:
  QWidget* mpKeyboardWidget;
public:
  LCKeyboard() = delete;

  LCKeyboard(QWidget* _widget, 
      QSharedPointer<LIRemoteDataSource> _streamSource, 
      const QString& _streamName,
      QSharedPointer<LIRemoteDataSource> _dataSource, 
      const QString& _dataName
      ) : 
    mpKeyboardWidget(_widget)
  {
    mspStreamReader = _streamSource->createReader(_streamName,
        [this](QSharedPointer<QByteArray> _data, LERemoteDataStatus _status)
        {
          QKeyEvent ke(QEvent::Type::KeyPress, Qt::KeyboardModifier::NoModifier);
          for(auto it = mWidgets.begin(); it != mWidgets.end(); it++)
          {
            QApplication::sendEvent
            /* (*it)->emi */
          }
        });

    mspDataWriter = _dataSource->createWriter(_dataName,
        [](LERemoteDataStatus _status)
        {
          Q_UNUSED(_status);
        });
  }

  ~LCKeyboard()
  {
    mpKeyboardWidget->deleteLater();
  }

  virtual void connectWidget(QWidget* _widget) override
  {
    mWidgets << _widget;
  }

  virtual void disconnectWidget(QWidget* _widget) override
  {
    mWidgets.removeAll(_widget);
  }

  virtual void setData(const QString& _data) override
  {
    mspDataWriter->writeRequest(_data.toUtf8());
  }

  virtual void show() override
  {
    mpKeyboardWidget->show();
  } 

  virtual void hide() override
  {
    mpKeyboardWidget->hide();
  }
};

QMap<QString, QSharedPointer<LCKeyboard>> __slKeyboards;

//==============================================================================
static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app)
{
  for(auto node = _element.firstChild(); 
      !node.isNull(); 
      node = node.nextSibling())
  {
    if(node.isElement())
    {
      auto el = node.toElement();
      auto builder = _app.getWidgetBuilder(el.tagName());
      if(!builder.isNull())
      {
        auto widget = builder->build(el, _app);
        if(widget) return widget;
      }
    }
  }
  return nullptr;
}

//==============================================================================
static void widgetAttr(QWidget* _widget, const QDomElement& _element)
{
  //Получение атрибутов.
  QString attr_width = _element.attribute(
      LCXmlCommon::mCommonAttributes.widht);

  QString attr_height = _element.attribute(
      LCXmlCommon::mCommonAttributes.height);

  //Переопределение размеров окна.
  QSize s = _widget->geometry().size();

  if(!attr_width.isNull())
  {
    bool flag = false;
    int width = attr_width.toInt(&flag);

    if(flag) 
    {
      s.setWidth(width);
    }
  }

  if(!attr_height.isNull())
  {
    bool flag = false;
    int height = attr_height.toInt(&flag);
    if(flag) 
    {
      s.setHeight(height);
    }
  }

  bool flag_posx = false;
  bool flag_posy = false;
  int posx;
  int posy;

  QString attr = _element.attribute(
      LCXmlCommon::mCommonAttributes.posx);
  posx = attr.toInt(&flag_posx);

  attr = _element.attribute(
      LCXmlCommon::mCommonAttributes.posy);
  posy = attr.toInt(&flag_posy);

  if(flag_posx && flag_posy)
  {
    _widget->move(posx, posy);
  }
  _widget->resize(s);
}

//==============================================================================
static QSharedPointer<LCKeyboard> uploadLocal(
    const QDomElement& _element, 
    const LIApplication& _app)
{
  QString attr_file =  _element.attribute(
      LCXmlCommon::mCommonAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(!el.isNull())
    {
      return uploadLocal(el, _app);
    }
    return nullptr;
  }

  QString attr_stream_source = _element.attribute(__slAttribubes.streamSource);
  if(attr_stream_source.isNull()) return nullptr;
  auto stream_source = _app.getDataSource(attr_stream_source);
  if(stream_source.isNull()) return nullptr;

  QString attr_stream_name = _element.attribute(__slAttribubes.streamName);
  if(attr_stream_source.isNull()) return nullptr;


  QWidget* widget = buildWidget(_element, _app);

  if(widget == nullptr) 
  {
    return nullptr;
  }

  widgetAttr(widget, _element);

  return QSharedPointer<LCKeyboard>(
      new LCKeyboard(widget, stream_source, attr_stream_name));
}


//==============================================================================
namespace uploadkeyboards 
{

void upload( const QDomElement &_element, const LIApplication& _app)
{
  QString attr_id = _element.attribute(
      LCXmlCommon::mCommonAttributes.id);

  if(attr_id.isNull()) return;

  auto keyboard = uploadLocal(_element, _app);

  if(keyboard.isNull()) return;

  QString attr_title = _element.attribute(
      LCXmlCommon::mCommonAttributes.title);

  if(!attr_title.isNull())
  {
    keyboard->mpWidget->setWindowTitle(attr_title);
  }
  else
  {
    keyboard->mpWidget->setWindowTitle(attr_id);
  }

  __slKeyboards.insert(attr_id, keyboard);
  keyboard->mpWidget->show();

}

QSharedPointer<LIKeyboard> getKeyboard(const QString& _keyboardId)
{
  auto it = __slKeyboards.find(_keyboardId);
  if(it == __slKeyboards.end()) return QSharedPointer<LIKeyboard>();
  return it.value();
}

}
