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
#include "applicationinterface.h"
#include "xmlwindows.h"
#include "xmlcommon.h"
#include "lcwindow.h"
#include "LIXmlWidgetBuilder.h"

#include <QIcon>
#include <QFileInfo>
#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QEvent>
#include <functional>
#include <qnamespace.h>
#include <QRegularExpression>
#include <QGuiApplication>
#include <QScreen>
#include <QShowEvent>

#define __smApp (CApplicationInterface::getInstance())

#define __smMessageHeader "Windows:"
#define __smMessage(msg) __smApp.message(msg)
#define __smWarning(msg) __smApp.warning(msg)
//==============================================================================
QMap<QString, QSharedPointer<LIWindow>> __slWindowsMap;
QList<QSharedPointer<LIWindow>> __slUnonimousWindows;

QList<std::function<void(void)>> __slShowList;

//==============================================================================
static const struct
{

  QString file = "file";
  QString title = "title";
  QString icon  = "icon";
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
  QString mainWidget    = "mainWidget";
}__slTags;

static const struct
{
  QString stayOnTop = "stayOnTop";
  QString frameless = "frameless";
}__slWindowFlags;

//==============================================================================uploadWidget
static QWidget* uploadWidget(const QDomElement& _element)
{
  QWidget* widget = nullptr;

  auto main_widget_element = _element.firstChildElement(__slTags.mainWidget);
  if(main_widget_element.isNull())
  {
    __smWarning("Can't find main widget element");
    return widget;
  }

  for(auto node = main_widget_element.firstChild(); 
      !node.isNull(); 
      node = node.nextSibling())
  {
    if(node.isElement())
    {
      auto el = node.toElement();
      auto builder = __smApp.getWidgetBuilder(el.tagName());
      if(!builder.isNull())
      {
        widget = builder->build(el, __smApp);
        if(widget) 
        { 
          __smMessage(
              QString("Main widget element '%1' was created")
              .arg(el.tagName()));
          break; 
        }
      }
    }
  }

  if(!widget)
  {
    __smWarning("Can't create main widget element");
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

  auto get_icon = 
    [](const QDomElement& _element)
    {
      QString icon_name = _element.attribute(__slAttributes.icon);
      if(icon_name.isNull()) return QIcon();
      return QIcon(icon_name);
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
  window->setIcon(get_icon(_element));
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

  auto  load_element = [](const QDomElement& _element)
  {
    QString attr_file =  _element.attribute(__slAttributes.file);

    auto add_attrs = 
      [](QDomElement& _element, const QDomNamedNodeMap& _attributes)
      {
        for(int i = 0; i < _attributes.length(); i++)
        {
          auto att = _attributes.item(i).toAttr();
          if(att.name() != __slAttributes.file)
          {
            _element.setAttribute(att.name(), att.value());
          }
        }
      };

    if(!attr_file.isNull())
    {
      if(!QFileInfo::exists(attr_file))
      {
        __smWarning(
            QString("Can't load file '%1'")
            .arg(attr_file));
        return QDomElement();
      }

      QDomElement el = __smApp.getDomDocument(attr_file).documentElement();
      if(el.tagName() != __slTags.window)
      {
        __smWarning(
            QString(
              "Wrong root element tag '%1' in file '%2'")
            .arg(el.tagName())
            .arg(attr_file));
        return QDomElement();
      }
      add_attrs(el, _element.attributes());
      return el;
    }
    return _element;
  };

  const auto upload_local = 
    [&load_element](const QDomElement& _element)
    {
      QDomElement el = load_element(_element);
      if(el.isNull()) return;

      QWidget* widget = uploadWidget(el);

      if(widget == nullptr) return;

      LCWindow*  window = uploadWindow(widget, _element);


      QString attr_id = _element.attribute(__slAttributes.id);

      auto sp_win = QSharedPointer<LIWindow>(window);

      if(!attr_id.isNull()) 
      {
        __slWindowsMap.insert(attr_id, sp_win);
      }
      else
      {
        __slUnonimousWindows << sp_win;
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
            /* QCoreApplication::sendEvent(widget, new QShowEvent()); */
          };
          __slShowList << show;
        }
      }
    };

  __smMessage("\n\tBegining deploy of widows\n");

  for(auto element = _rootElement.firstChildElement(__slTags.window); 
      !element.isNull(); 
      element = element.nextSiblingElement(__slTags.window))
  {
    __smMessage(
        QString("\n\tBegining deploy window described in line %1")
        .arg(element.lineNumber()));
    upload_local(element);
    __smMessage(
        QString("\n\tEnd deploy window described in line %1\n")
        .arg(element.lineNumber()));
  }
  __smMessage("\n\tEnd deploy of widows\n");
}
} //namespace
