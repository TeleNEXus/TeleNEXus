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
#include "uploadwindows.h"
#include "lcxmlcommon.h"
#include "LIXmlWidgetBuilder.h"
#include "LIWindow.h"
#include "LIKeyboard.h"
#include "LIApplication.h"
#include  "LIJScriptService.h"
#include "tnexcommon.h"

#include "cwindow.h"

#include <QDomElement>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QDebug>
#include <LIRemoteDataWriter.h>
#include <QEvent>
#include <qnamespace.h>

//==============================================================================
QMap<QString, QSharedPointer<LIWindow>> __slWindowsMap;
QList<std::function<void(void)>> __slShowList;

//==============================================================================
static const struct
{
  QString interval = "interval";
  QString scriptId = "scriptId";
  QString modality = "modality";
  QString state = "state";

  QString modes = "modes";

  struct 
  {
    QString attribute = "event";
    struct 
    {
      QString show = "show";
      QString hide = "hide";
    }vals;
  }event;
}__slAttributes;

static const struct
{
  QString actions       = "actions";
  QString mainWidget    = "mainWidget";
  QString scriptExecute = "scriptExecute";
  QString scriptLaunch  = "scriptLaunch";
  QString scriptStop    = "scriptStop";

}__slTags;

//==============================================================================ActionLoader
class CActionLoader
{
private:
  QMap<QString, 
    std::function<
      void(const QDomElement&, const LIApplication&, LIWindow*)>> mLoaders;
  QMap<QString,
    std::function<void(LIWindow*, LIWindow::TAction)>> mEventAdder;
private:
  CActionLoader(const CActionLoader&) = delete;
  CActionLoader& operator=(const CActionLoader&) = delete;
  CActionLoader()
  {

    mEventAdder.insert(__slAttributes.event.vals.show,
        [](LIWindow* _win, LIWindow::TAction _act)
        {
          _win->addActionShow(_act);
        });

    mEventAdder.insert(__slAttributes.event.vals.hide,
        [](LIWindow* _win, LIWindow::TAction _act)
        {
          _win->addActionHide(_act);
        });

    auto add_script_action = 
      [this](const QDomElement& _el, const LIApplication& _app, LIWindow* _win, 
          std::function<void(QSharedPointer<LIJScriptService>)> _scriptAct)
      {
        QString attr_script_id = _el.attribute(__slAttributes.scriptId);
        QString event = _el.attribute(__slAttributes.event.attribute);
        if(event.isNull()) return;
        auto event_adder = mEventAdder.find(event);
        if(event_adder == mEventAdder.end()) return;
        event_adder.value()(_win,
            [&_app, attr_script_id, _scriptAct]()
            {
              auto script = _app.getScriptService(attr_script_id);
              if(script.isNull()) return;
              _scriptAct(script);
            });
      };

    mLoaders.insert(__slTags.scriptExecute,
        [add_script_action](
          const QDomElement& _el, const LIApplication& _app, LIWindow* _win)
        {
          add_script_action( _el, _app, _win,
              [](QSharedPointer<LIJScriptService> _script)
              {
                _script->execute();
              });
        });


    mLoaders.insert(__slTags.scriptLaunch,
        [add_script_action](
          const QDomElement& _el, const LIApplication& _app, LIWindow* _win)
        {
          QString attr_interval = _el.attribute(__slAttributes.interval);
          if(attr_interval.isNull()) return;
          bool flag = false;
          int interval = attr_interval.toInt(&flag);
          if(!flag) return;
          
          add_script_action( _el, _app, _win,
              [interval](QSharedPointer<LIJScriptService> _script)
              {
                _script->launch(interval);
              });
        });

    mLoaders.insert(__slTags.scriptStop,
        [add_script_action](
          const QDomElement& _el, const LIApplication& _app, LIWindow* _win)
        {
          add_script_action( _el, _app, _win,
              [](QSharedPointer<LIJScriptService> _script)
              {
                _script->stop();
              });
        });
  };

public:

  static CActionLoader& getInstance()
  {
    static CActionLoader instance;
    return instance;
  }

  void load(const QDomElement& _el, const LIApplication& _app, LIWindow* _win)
  {
    for(auto act_el = _el.firstChildElement(__slTags.actions); 
        !act_el.isNull();
        act_el = act_el.nextSiblingElement(__slTags.actions))
    {
      for(auto node = act_el.firstChild(); !node.isNull(); node = node.nextSibling())
      {
        auto el = node.toElement();
        if(el.isNull()) continue;
        auto loader = mLoaders.find(el.tagName());
        if(loader == mLoaders.end()) continue;
        loader.value()(el, _app, _win);
      }
    }

  };
};

//==============================================================================LCXmlWindow




//==============================================================================LCXmlWindows
LCXmlWindows::LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows::~LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows& LCXmlWindows::instance()
{
  static LCXmlWindows instance;
  return instance;
}

//------------------------------------------------------------------------------
QSharedPointer<LIWindow> LCXmlWindows::getWindow(const QString& _windowId)
{
  auto it = __slWindowsMap.find(_windowId); 
  if(it == __slWindowsMap.end()) 
  {
    return nullptr;
  }
  return it.value();
}

//------------------------------------------------------------------------------
void LCXmlWindows::show()
{
  for(auto it = __slShowList.begin(); it != __slShowList.end();
      it++)
  {
    (*it)();
  }
}

//------------------------------------------------------------------------------

static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app);

static LCXmlWindow* buildLocal(const QDomElement& _element,
    const LIApplication& _app);

static void initWindow(LCXmlWindow* _win, const QDomElement& _element);

//------------------------------------------------------------------------------
void LCXmlWindows::buildWindow(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  auto window = buildLocal(_element, _app); 
  if(window == nullptr) return;
  initWindow(window, _element);
}

//------------------------------------------------------------------------------
static void initWindow(LCXmlWindow* _win, const QDomElement& _element)
{
  QString attr_id = _element.attribute(
      LCXmlCommon::mCommonAttributes.id);

  QString attr_title = _element.attribute(
      LCXmlCommon::mCommonAttributes.title);

  QString attr_show = _element.attribute(
      LCXmlCommon::mCommonAttributes.show.tag);
  QString attr_state = _element.attribute(__slAttributes.state);

  QString attr_modality = _element.attribute(__slAttributes.modality);

  if(!attr_id.isNull()) 
  {
    __slWindowsMap.insert(attr_id, 
        QSharedPointer<LIWindow>(_win));
    if(attr_title.isNull()) attr_title = attr_id;
  }

  if(!attr_title.isNull())
  {
    _win->mpWidget->setWindowTitle(attr_title);
  }

  if(attr_modality == QStringLiteral("yes"))
  {
    qDebug() << "initWindow set modality";
    _win->mpWidget->setWindowModality(Qt::WindowModality::ApplicationModal);
  }


  QString attr_modes = _element.attribute(__slAttributes.modes);


  auto get_mode_setters = [_win]() mutable
  {
    QMap<QString, std::function<void(const QString&)>> setters_map;

    setters_map.insert(QStringLiteral("stayOnTop"),
        [_win](const QString& _val)
        {
          if(_val == QStringLiteral("on"))
          {
            _win->mpWidget->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);
          }
          else if(_val == QStringLiteral("off"))
          {
            _win->mpWidget->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, false);
          }
        });

    setters_map.insert(QStringLiteral("frameless"),
        [_win](const QString& _val)
        {
          if(_val == QStringLiteral("on"))
          {
            _win->mpWidget->setWindowFlag(Qt::WindowType::FramelessWindowHint, true);
          }
          else if(_val == QStringLiteral("off"))
          {
            _win->mpWidget->setWindowFlag(Qt::WindowType::FramelessWindowHint, false);
          }
        });
    return setters_map;
  };

  if(!attr_modes.isNull())
  {
    tnexcommon::setMultipleAttributes(get_mode_setters(), attr_modes);
  }


  auto show = [attr_show, _win] () mutable
  {
    attr_show = attr_show.remove(" ");
    attr_show = attr_show.toLower();

    if(attr_show == QStringLiteral("normal"))
      _win->mpWidget->show();
    else if(attr_show == QStringLiteral("fullscreen"))
    {
      qDebug() << "initWindow set full screen state";
      _win->mpWidget->show();
      _win->mpWidget->setWindowState(Qt::WindowState::WindowFullScreen);
    }
  };

  LCXmlWindow::smShowList << show;
}

//------------------------------------------------------------------------------
static void widgetAttr(QWidget* _widget, const QDomElement& _element);

static LCXmlWindow* buildLocal(const QDomElement& _element,
    const LIApplication& _app)
{
  QString attr_file =  _element.attribute(
      LCXmlCommon::mCommonAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return nullptr;
    if(el.tagName() != LCXmlCommon::mBaseTags.window) return nullptr;
    return buildLocal(el, _app);
  }


  QWidget* widget = buildWidget(_element, _app);
  if(widget == nullptr) 
  {
    return nullptr;
  }

  LCXmlWindow*  window = new LCXmlWindow(widget);

  widgetAttr(widget, _element);

  CActionLoader::getInstance().load(_element, _app, window);

  return window;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app)
{
  auto main_widget_element = _element.firstChildElement(__slTags.mainWidget);
  if(main_widget_element.isNull()) return nullptr;

  for(auto node = main_widget_element.firstChild(); 
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
