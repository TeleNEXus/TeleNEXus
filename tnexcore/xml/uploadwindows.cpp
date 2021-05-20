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
QList<QSharedPointer<LIWindow>> __slNoNameWindows;

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

static const struct
{
  QString valueOn = "on";
  QString valueOff = "valueOff";
  QString stayOnTop = "stayOnTop";
  QString frameless = "frameless";
}__slModes;

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

//==============================================================================endElement
static QDomElement endElement(
    const QDomElement& _element,
    const LIApplication& _app)
{
  QString attr_file =  _element.attribute(
      LCXmlCommon::mCommonAttributes.file);

  auto set_attr = 
    [](const QDomElement& _elf, QDomElement _els)
    {
      auto attrs = _elf.attributes();
      for(int i = 0; i < attrs.length(); i++)
      {
        auto att = attrs.item(i).toAttr();
        if(att.name() != LCXmlCommon::mCommonAttributes.file)
        {
          _els.setAttribute(att.name(), att.value());
        }
      }
    };

  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return el;
    if(el.tagName() != LCXmlCommon::mBaseTags.window) return QDomElement();
    set_attr(_element, el);
    return endElement(el, _app);
  }
  return _element;
}

//==============================================================================buildWidget
static QWidget* buildWidget(const QDomElement& _element, 
    const LIApplication& _app)
{

  //--------------------------------------------------init_widget[]
  auto init_widget = [&_element](QWidget* _widget)
  {

    QString attr = _element.attribute(LCXmlCommon::mCommonAttributes.title);
    if(!attr.isNull()) { _widget->setWindowTitle(attr); }

    attr = _element.attribute(__slAttributes.modality);
    if(attr == QStringLiteral("yes"))
    {
      _widget->setWindowModality(Qt::WindowModality::ApplicationModal);
    }


    //----------------------------------------------------set_modes[]
    auto set_modes = [&_element, _widget]()
    {
      QString attr_modes = _element.attribute(__slAttributes.modes);

      auto get_mode_setters = [_widget]() mutable
      {
        QMap<QString, std::function<void(const QString&)>> setters_map;

        setters_map.insert(__slModes.stayOnTop,
            [_widget](const QString& _val)
            {
              if(_val == __slModes.valueOn)
              {
                _widget->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);
              }
              else if(_val == __slModes.valueOff)
              {
                _widget->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, false);
              }
            });

        setters_map.insert(__slModes.frameless,
            [_widget](const QString& _val)
            {
              if(_val == __slModes.valueOn)
              {
                _widget->setWindowFlag(Qt::WindowType::FramelessWindowHint, true);
              }
              else if(_val == __slModes.valueOff)
              {
                _widget->setWindowFlag(Qt::WindowType::FramelessWindowHint, false);
              }
            });
        return setters_map;
      };

      if(!attr_modes.isNull())
      {
        tnexcommon::setMultipleAttributes(get_mode_setters(), attr_modes);
      }
    };


    //----------------------------------------------------widget_size[]
    auto widget_size = [&_element, _widget]()
    {
      QSize size = _widget->geometry().size();

      QString attr_width = _element.attribute(
          LCXmlCommon::mCommonAttributes.widht);

      QString attr_height = _element.attribute(
          LCXmlCommon::mCommonAttributes.height);

      if(!attr_width.isNull())
      {
        bool flag = false;
        int width = attr_width.toInt(&flag);

        if(flag) 
        {
          size.setWidth(width);
        }
      }

      if(!attr_height.isNull())
      {
        bool flag = false;
        int height = attr_height.toInt(&flag);
        if(flag) 
        {
          size.setHeight(height);
        }
      }
      return size;
    };

    //----------------------------------------------------set_position[]
    auto set_position = [&_element, _widget]()
    {
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
    };

    set_modes();
    set_position();
    _widget->resize(widget_size());
  };
  //--------------------------------------------------------

  QWidget* widget = nullptr;

  auto main_widget_element = _element.firstChildElement(__slTags.mainWidget);

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
        widget = builder->build(el, _app);
        if(widget) 
        {
          init_widget(widget);
          break;
        }
      }
    }
  }

  return widget;
}

//==============================================================================uploadwindow
namespace uploadwindows
{

//------------------------------------------------------------------------------
QSharedPointer<LIWindow> getWindow(const QString& _windowId)
{
  auto it = __slWindowsMap.find(_windowId); 
  if(it == __slWindowsMap.end()) 
  {
    return nullptr;
  }
  return it.value();
}

//------------------------------------------------------------------------------
void show()
{
  for(auto it = __slShowList.begin(); it != __slShowList.end();
      it++)
  {
    (*it)();
  }
}

//------------------------------------------------------------------------------
void upload(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  QDomElement el = endElement(_element, _app);
  QWidget* widget = buildWidget(el, _app);
  if(widget == nullptr) return;

  LCXmlWindow*  window = new LCXmlWindow(widget);

  CActionLoader::getInstance().load(el, _app, window);

  QString attr_id = _element.attribute(
      LCXmlCommon::mCommonAttributes.id);

  auto sp_win = QSharedPointer<LIWindow>(window);
  if(!attr_id.isNull()) 
  {
    __slWindowsMap.insert(attr_id, sp_win);
  }
  else
  {
    __slNoNameWindows << sp_win; 
  }


  //Show controll
  {
    QString attr_show = _element.attribute(
        LCXmlCommon::mCommonAttributes.show.tag);
    auto show = [attr_show, widget] () mutable
    {
      attr_show = attr_show.remove(" ");
      attr_show = attr_show.toLower();

      if(attr_show == QStringLiteral("normal"))
        widget->show();
      else if(attr_show == QStringLiteral("fullscreen"))
      {
        widget->show();
        widget->setWindowState(Qt::WindowState::WindowFullScreen);
      }
    };
    __slShowList << show;
  }

}
} //namespace
