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

#include "lcxmlbuttonbuilder.h"
#include "LIDataFormatter.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "LIRemoteDataWriter.h"
#include "LIJScriptService.h"
#include "widgetbuilderscommon.h"
#include "xmlcommon.h"
#include <QPushButton>
#include <QDomElement>
#include <qicon.h>
#include <QDebug>


//==============================================================================
static const struct
{
  QString press = "press";
  QString release = "release";
  QString writeData = "writeData";
  QString controlWindow = "controlWindow";
  QString controlScript = "controlScript";
}__slTags;

static const struct
{
  QString value = "value";
  QString data = "data";

}__slAttributesWriteData;

static const struct
{
  QString windowId = "windowId";
  QString action = "action";
}__slAttributesControlWindow;

static const struct
{
  QString scriptId = "scriptId";
  QString scriptAction = "scriptAction";
}__slAttributesSctriptControl;

static const struct
{
  QString text="text";
}__slAttributes;

//==============================================================================
using TActions = QList<std::function<void(void)>>;

//==============================================================================
class CActionBuilder
{
private:
  QMap<QString, std::function<void(
      TActions&, 
      const QDomElement&, 
      const LIApplication&)>> mActionBuilders;
  CActionBuilder()
  {
  
    //------------------------------------------------writeData[]
    mActionBuilders.insert(__slTags.writeData,
        [](TActions& _actions,
          const QDomElement& _element,
          const LIApplication& _app)
        {
          QSharedPointer<LIRemoteDataSource> source;
          QSharedPointer<LIDataFormatter> format;

          QString attr_dataspec = _element.attribute(__slAttributesWriteData.data);
          if(attr_dataspec.isNull()) return;
          QString attr_value = _element.attribute(__slAttributesWriteData.value);
          if(attr_value.isNull()) return;


          auto data_spec = xmlcommon::parseDataSpecification(attr_dataspec);

          source = _app.getDataSource(data_spec.sourceId);
          format = _app.getDataFormatter(data_spec.formatterId);

          if(source.isNull()||format.isNull()) return;

          auto writer = source->createWriter(data_spec.dataId);
          if(writer.isNull()) return;

          QByteArray data = format->toBytes(attr_value);

          _actions << [writer, data]()
          {
            writer->writeRequest(data);
          };
        });

    //------------------------------------------------controlWindow[]
    mActionBuilders.insert(__slTags.controlWindow,
        [](TActions& _actions,
          const QDomElement& _element,
          const LIApplication& _app)
        {
          QString attr_window = _element.attribute(__slAttributesControlWindow.windowId);
          QString attr_action = _element.attribute(__slAttributesControlWindow.action);

          if(attr_window.isNull()||
              attr_action.isNull())
            return;

          bool error_flag = false;

          auto action = xmlcommon::parseAction(attr_action, 
              [&error_flag](const QString&)
              {
                error_flag = true;
              });

          if(error_flag) return;

          _actions << [action, attr_window, &_app]()
          {
            auto window = _app.getWindow(attr_window);
            if(window.isNull()) return;
            window->action(action.name, action.parameters);
          };
        });

    //------------------------------------------------controlScript[]
    mActionBuilders.insert(__slTags.controlScript,
        [](TActions& _actions,
          const QDomElement& _element,
          const LIApplication& _app)
        {
          QString attr_script = _element.attribute(__slAttributesSctriptControl.scriptId);
          if(attr_script.isNull()) return;
          QString attr_action = _element.attribute(__slAttributesSctriptControl.scriptAction);
          if(attr_action.isNull()) return;

          _actions << [attr_script, attr_action, &_app]()
          {
            auto script = _app.getScriptService(attr_script);
            if(script.isNull()) return;
            script->action(attr_action);
          };
        });
  }

public:
  static CActionBuilder& instance()
  {
    static CActionBuilder builder;
    return builder;
  }

  void build(
      TActions& _actions, 
      const QDomElement& _element, 
      const LIApplication& _app)
  {
    QString action_tag = _element.tagName();
    auto it = mActionBuilders.find(_element.tagName());
    if(it == mActionBuilders.end()) return;
    it.value()(_actions, _element, _app);
  }
};


//==============================================================================LCXmlButtonBuilder
LCXmlButtonBuilder::LCXmlButtonBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlButtonBuilder::~LCXmlButtonBuilder()
{
}


//------------------------------------------------------------------------------
QWidget* LCXmlButtonBuilder::buildLocal(
const QDomElement& _element, const LIApplication& _app)
{

  QString attr_text = _element.attribute(__slAttributes.text);
  if(attr_text.isNull())
  {
    attr_text = QStringLiteral("Button");
  }

  QPushButton* button = new QPushButton(attr_text);

  TActions actions_pressed;
  TActions actions_released;

  auto parse_actions = [&_element, &_app](
      const QString& _action_tag, TActions& _actions)
  {
    auto element = _element.firstChildElement(_action_tag);
    if(element.isNull()) return;
    for(auto node = element.firstChild(); !node.isNull(); node = node.nextSibling())
    {
      auto el = node.toElement();
      if(el.isNull()) continue;
      CActionBuilder::instance().build(_actions, el, _app);
    }
  };

  parse_actions(__slTags.press, actions_pressed);
  parse_actions(__slTags.release, actions_released);

  if(actions_pressed.size() > 0)
  {
    QObject::connect(button, &QPushButton::pressed,
        [actions_pressed]()
        {
          for(auto it = actions_pressed.begin(); it != actions_pressed.end(); it++)
          {
            (*it)();
          }
        });
  }

  if(actions_released.size() > 0)
  {
    QObject::connect(button, &QPushButton::released,
        [actions_released]()
        {
          for(auto it = actions_released.begin(); it != actions_released.end(); it++)
          {
            (*it)();
          }
        });
  }

  auto pixmap = parsePixmap(_element.attribute(QStringLiteral("icon")), _app);

  if(!pixmap.isNull())
  {
    button->setIconSize(pixmap.size());
    button->setIcon(pixmap);
  }

  setWidgetName(_element, button);
  setWidgetStyle(_element, button);
  setWidgetSize(_element, button);
  setWidgetPosition(_element, button);
  setWidgetFixedSize(_element, button);

  return button;
}


