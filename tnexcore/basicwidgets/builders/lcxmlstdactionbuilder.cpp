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

#include "lcxmlstdactionbuilder.h"
#include "LIRemoteDataSource.h"
#include "LIRemoteDataWriter.h"
#include "LIDataFormatter.h"
#include "LIApplication.h"
#include "LIWindow.h"
#include "LIJScriptService.h"
#include "xmlcommon.h"

#include <QDomElement>
#include <QMap>

//==============================================================================
static const struct
{
  QString writeData     = "writeData";
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

using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================
QMap<QString, std::function<void(
    TActions&, 
    const QDomElement&, 
    const LIApplication&)>> __slActionBuilders;



//==============================================================================
LCXmlStdActionBuilder::LCXmlStdActionBuilder()
{
  //------------------------------------------------writeData[]
  __slActionBuilders.insert(__slTags.writeData,
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
  __slActionBuilders.insert(__slTags.controlWindow,
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
  __slActionBuilders.insert(__slTags.controlScript,
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

//------------------------------------------------------------------------------
LCXmlStdActionBuilder& LCXmlStdActionBuilder::instance()
{
  static LCXmlStdActionBuilder builder;
  return builder;
}

TActions LCXmlStdActionBuilder::build(
    const QDomElement& _element, 
    const LIApplication& _app)
{
  TActions actions;
  if(_element.isNull()) return actions;
  for(auto el = _element.firstChildElement();
      !el.isNull(); el = el.nextSiblingElement())
  {
    QString action_tag = el.tagName();
    auto it = __slActionBuilders.find(el.tagName());
    if(it == __slActionBuilders.end()) continue;
    it.value()(actions, el, _app);
  }
  return actions;
}
