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
#include "xmlwindows.h"
#include "applicationinterface.h"
#include "LIXmlWidgetBuilder.h"
#include "LIKeyboard.h"
#include  "LIJScriptService.h"
#include "xmlcommon.h"
#include "lcwindow.h"
#include <QDomElement>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QDebug>
#include <LIRemoteDataWriter.h>
#include <QEvent>
#include <functional>
#include <qnamespace.h>
#include <QRegularExpression>
#include <QGuiApplication>
#include <QScreen>

//==============================================================================
QMap<QString, QSharedPointer<LIWindow>> __slWindowsMap;
QList<QSharedPointer<LIWindow>> __slNoNameWindows;

QList<std::function<void(void)>> __slShowList;

//==============================================================================
static const struct
{

  QString file = "file";
  QString title = "title";
  QString id = "id";
  QString visible = "visible";

  QString screen   = "screen";
  QString interval = "interval";
  QString scriptId = "scriptId";
  QString modality = "modality";
  QString showMode = "showMode";
  QString flags = "flags";

  struct 
  {
    QString attribute = "event";
    struct 
    {
      QString show = "show";
      QString hide = "hide";
    }vals;
  }event;
  QString size = "size";
  QString position = "position";

}__slAttributes;

static const struct
{
  QString window        = "WINDOW";
  QString actions       = "actions";
  QString mainWidget    = "mainWidget";
  QString scriptExecute = "scriptExecute";
  QString scriptLaunch  = "scriptLaunch";
  QString scriptStop    = "scriptStop";
}__slTags;

static const struct
{
  QString stayOnTop = "stayOnTop";
  QString frameless = "frameless";
}__slWindowFlags;

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
  QString attr_file =  _element.attribute(__slAttributes.file);

  auto set_attr = 
    [](const QDomElement& _elf, QDomElement _els)
    {
      auto attrs = _elf.attributes();
      for(int i = 0; i < attrs.length(); i++)
      {
        auto att = attrs.item(i).toAttr();
        if(att.name() != __slAttributes.file)
        {
          _els.setAttribute(att.name(), att.value());
        }
      }
    };

  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return el;
    if(el.tagName() != __slTags.window) return QDomElement();
    set_attr(_element, el);
    return endElement(el, _app);
  }
  return _element;
}

//==============================================================================uploadWidget
static QWidget* uploadWidget(
    const QDomElement& _element, const LIApplication& _app)
{
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
        if(widget) { break; }
      }
    }
  }

  return widget;
}

//==============================================================================uploadWindow
static LCWindow* uploadWindow(QWidget* _widget, const QDomElement& _element)
{
  LCWindow* window = new LCWindow(_widget);

  //--------------------------------------------------init_widget[]
  auto get_title = [](const QDomElement& _element)
  {
    return _element.attribute(__slAttributes.title);
  };

  auto get_show_mode = [](const QDomElement& _element)
  {
    return 
      LCWindow::stringToShowMode(_element.attribute(__slAttributes.showMode));
  };

  //----------------------------------------------------get_flags[]
  auto get_flags = 
    [](const QDomElement& _element)
    {
      QString attr_modes = _element.attribute(__slAttributes.flags);

      Qt::WindowFlags flags = Qt::WindowType::Window;

      auto flag_attributes = xmlcommon::parseAttributes(attr_modes);

      if(flag_attributes.size() == 0) return flags;

      if(flag_attributes.contains(__slWindowFlags.stayOnTop))
      {
        flags |= Qt::WindowType::WindowStaysOnTopHint;
      }

      if(flag_attributes.contains(__slWindowFlags.frameless))
      {
        flags |= Qt::WindowType::FramelessWindowHint;
      }

      return flags;
    };


  //----------------------------------------------------widget_size[]
  auto get_size = [](const QDomElement& _element, const QSize _oldSize)
  {
    QSize size = _oldSize;
    QString attr_size = _element.attribute(__slAttributes.size);
    if(attr_size.isNull()) return size;
    auto values = xmlcommon::parseValues(attr_size);

    ([&size](const QString& _attr_size)
        {
          auto values = xmlcommon::parseValues(_attr_size);
          if(values.size() == 0) return;
          QString val_str;
          bool flag = false;
          auto it = values.begin();
          val_str = (*it);
          val_str.remove(QRegularExpression(QStringLiteral("[^0-9]")));
          int s = (*it).toInt(&flag);
          if(flag) size.setWidth(s);
          it++;
          if(it == values.end()) return;
          val_str = (*it);
          val_str.remove(QRegularExpression(QStringLiteral("[^0-9]")));
          s = (*it).toInt(&flag);
          if(flag) size.setHeight(s);
        })(attr_size);

    return size;
  };

  //----------------------------------------------------get_screen_origin[]
  auto get_screen_origin = 
    [](const QDomElement& _element)
    {
      QString attr = _element.attribute(__slAttributes.screen);
      if(attr.isNull()) return QPoint(0,0);
      bool flag = false;
      int sn = attr.toUInt(&flag);
      if(!flag) return QPoint(0,0);
      auto screens = QGuiApplication::screens();
      if((sn < 0) || (sn >= screens.size())) return QPoint(0,0);
      return screens[sn]->geometry().topLeft();
    };

  //----------------------------------------------------get_position[]
  auto get_position = [](const QDomElement& _element)
  {

    QPoint pos = QPoint(0, 0);

    QString attr_position = _element.attribute(__slAttributes.position);

    if(attr_position.isNull()) return pos;

    auto values = xmlcommon::parseValues(attr_position);

    ([&pos](const QStringList _values)
        {
          if(_values.size() == 0) return;
          QString val_str;
          bool flag = false;
          auto it = _values.begin();
          val_str = (*it);
          val_str.remove(QRegularExpression(QStringLiteral("[^0-9]")));
          int p = (*it).toInt(&flag);
          if(flag) pos.setX(p);
          it++;
          if(it == _values.end()) return;
          val_str = (*it);
          val_str.remove(QRegularExpression(QStringLiteral("[^0-9]")));
          p = (*it).toInt(&flag);
          if(flag) pos.setY(p);
        })(values);

    return pos;
  };


  //--------------------------------------------------set_modality
  auto get_modality = [](const QDomElement& _element)
  {
    QString attr = _element.attribute(__slAttributes.modality);
    if(attr == QStringLiteral("true"))
    {
      return true;
    }
    return false;
  };

  window->setTitle(get_title(_element));
  window->setShowMode(get_show_mode(_element));
  window->setModality(get_modality(_element));
  window->setFlags(get_flags(_element));
  window->setSize(get_size(_element, window->getSize()));
  window->setPosition(get_position(_element) + get_screen_origin(_element));

  return window;
}

//==============================================================================xmlwindows
namespace xmlwindows
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
    const QDomElement &_rootElement)
{
  const auto upload_local = 
    [](const QDomElement& _element, const LIApplication& _app)
    {
      QDomElement el = endElement(_element, _app);
      QWidget* widget = uploadWidget(el, _app);

      if(widget == nullptr) return;

      LCWindow*  window = uploadWindow(widget, _element);

      CActionLoader::getInstance().load(el, _app, window);

      QString attr_id = _element.attribute(__slAttributes.id);

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
        QString attr = _element.attribute(__slAttributes.visible);
        if(attr == QStringLiteral("true"))
        {
          auto wp_win = sp_win.toWeakRef();
          auto show = [wp_win, widget]() 
          {
            auto sp = wp_win.toStrongRef();
            if(sp.isNull()) return;
            sp->action(QStringLiteral("show"));
            QCoreApplication::sendEvent(widget, new QShowEvent());
          };
          __slShowList << show;
        }
      }
    };

  for(auto node = _rootElement.firstChildElement(__slTags.window); 
      !node.isNull(); 
      node = node.nextSiblingElement(__slTags.window))
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    upload_local(el, CApplicationInterface::getInstance());
  }
}
} //namespace
