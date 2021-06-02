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
#include "xmlcommon.h"
#include "LIXmlWidgetBuilder.h"
#include "LIWindow.h"
#include "LIKeyboard.h"
#include "LIApplication.h"
#include  "LIJScriptService.h"
#include "tnexcommon.h"

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
#include <QCoreApplication>

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
    if(el.tagName() != xmlcommon::mBaseTags.window) return QDomElement();
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
  auto get_flags = [](const QDomElement& _element)
  {
    QString attr_modes = _element.attribute(__slAttributes.flags);

    Qt::WindowFlags flags = Qt::WindowType::Window;

    auto flag_attributes = tnexcommon::parseAttributes(attr_modes);

    if(flag_attributes.size() == 0) return flags;

    if(flag_attributes.contains(__slWindowFlags.stayOnTop))
    {
      flags |= Qt::WindowType::WindowStaysOnTopHint;
    }

    if(flag_attributes.contains(__slWindowFlags.frameless))
    {
      flags |= Qt::WindowType::FramelessWindowHint;
    }
    /* } */

    return flags;
  };

    //----------------------------------------------------widget_size[]
  auto get_size = [](const QDomElement& _element, const QSize _oldSize)
  {
    QSize size = _oldSize;
    QString attr_size = _element.attribute(__slAttributes.size);
    if(attr_size.isNull()) return size;
    auto values = tnexcommon::parseValues(attr_size);

    ([&size](const QString& _attr_size)
        {
          auto values = tnexcommon::parseValues(_attr_size);
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

  //----------------------------------------------------set_position[]
  auto get_position = [](const QDomElement& _element)
  {

    QPoint pos = QPoint(0, 0);

    auto ret = [&pos]()
    {
      return pos;
    };

    QString attr_position = _element.attribute(__slAttributes.position);

    if(attr_position.isNull()) return ret();

    auto values = tnexcommon::parseValues(attr_position);

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

    return ret();
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
  window->setPosition(get_position(_element));

  return window;
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
      auto show = [window, widget]() 
      {
        window->action(QStringLiteral("show"));
        QCoreApplication::sendEvent(widget, new QShowEvent());
      };
      __slShowList << show;
    }
  }
}
} //namespace
