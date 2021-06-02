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
#include "xmlbuilders.h"

#include "LIXmlWidgetBuilder.h"
#include "LIXmlLayoutBuilder.h"
#include "LIXmlRemoteDataSourceBuilder.h"

#include "basicwidgetbuilders.h"
#include "basiclayoutbuilders.h"

#include <QLibrary>
#include <QDebug>
#include <QFile>

//==============================================================================
using TLibAccessFunc = void* (*)();

static const struct
{
  QString widgetBuilders  = "WIDGETBUILDERS";
  QString layoutBuilders  = "LAYOUTBUILDERS";
  QString sourceBuilders  = "SOURCEBUILDERS";
}__slRootTags;

static const struct
{
  QString file    = "file";
  QString lib     = "lib";
  QString handler = "handler";
}__slAttributes;

enum class EAddResult
{
  Added,
  Replaced
};

//==============================================================================
template <typename T> QSharedPointer<T> findBuilder(
    const QString& _id, QMap<QString,QSharedPointer<T>> _map)
{
    auto it = _map.find(_id);
    if(it != _map.end())
    {
        return it.value();
    }
    return QSharedPointer<T>();
}

template <typename T> void Upload(
    QMap<QString, QSharedPointer<T>>& _map,
    const QString& _rootTag,
    const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths)
{
  auto adder = [&_map](const QString& _id, void* _builder)
  {
    EAddResult ret = (_map.contains(_id)) ? (EAddResult::Replaced) : (EAddResult::Added); 
    _map.insert(_id, QSharedPointer<T>(static_cast<T*>(_builder)));
    return ret;
  };
  load( _rootTag, _rootElement, _pathPrj, _libPaths, adder);
}

//==============================================================================

static void load(
    const QString& _rootTag,
    const QDomElement& _rootElement, 
    const QString& _pathPrj, 
    const QStringList& _libPaths,
    std::function<EAddResult(const QString& _name, void* _builder)> _adder);

static void loadBuilders(
    const QString& _rootTag, 
    const QDomElement& _element, 
    const QStringList& _libPaths,
    std::function<EAddResult(const QString& _name, void* _builder)> _adder
    );



//==============================================================================
namespace builders
{


//------------------------------------------------------------------------------
namespace widgets
{

QMap<QString, QSharedPointer<LIXmlWidgetBuilder>> __slBuilders;

void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths)
{
  Upload(__slBuilders, __slRootTags.widgetBuilders, _rootElement, _pathPrj, _libPaths);
}

QSharedPointer<LIXmlWidgetBuilder> getBuilder(const QString _id)
{
  return findBuilder(_id, __slBuilders);
}
} /* namespace widget */ 

//------------------------------------------------------------------------------
namespace layouts
{

QMap<QString, QSharedPointer<LIXmlLayoutBuilder>> __slBuilders;

//------------------------------------------------------------------------------
void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths)
{
  Upload(__slBuilders, __slRootTags.layoutBuilders, _rootElement, _pathPrj, _libPaths);
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlLayoutBuilder> getBuilder(const QString _id)
{
  return findBuilder(_id, __slBuilders);
}
} /* namespace layouts */

//------------------------------------------------------------------------------
namespace sources 
{

QMap<QString, QSharedPointer<LIXmlRemoteDataSourceBuilder>> __slBuilders;

//------------------------------------------------------------------------------
void upload( const QDomElement& _rootElement, 
    const QString& _pathPrj, const QStringList& _libPaths)
{
  Upload(__slBuilders, __slRootTags.sourceBuilders, _rootElement, _pathPrj, _libPaths);
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceBuilder> getBuilder(const QString _id)
{
  return findBuilder(_id, __slBuilders);
}

} /* namespace sources*/

class CInitMaps
{
private:
  CInitMaps()
  {
    qDebug() << "xmlbuilders.cpp: Init maps constructor call";
    widgets::__slBuilders.unite(basicwidgetbuilders::getBuilders());
    layouts::__slBuilders.unite(basiclayoutbuilders::getBuilders());
  }
  CInitMaps(const CInitMaps&) = delete;
  CInitMaps& operator=(const CInitMaps&) = delete;
  static CInitMaps instance;
};
CInitMaps CInitMaps::instance;

} /* namespace uploadbuilders */



//==============================================================================
static void load(
    const QString& _rootTag,
    const QDomElement& _rootElement, 
    const QString& _pathPrj, 
    const QStringList& _libPaths,
    std::function<EAddResult(const QString& _name, void* _builder)> _adder)
{

  auto element = _rootElement.firstChildElement(_rootTag);
  if(element.isNull()) return;

  QString fileName = element.attribute(__slAttributes.file);

  if(fileName.isNull())
  {
    loadBuilders(_rootTag, element, _libPaths, _adder);
    return;
  }

  fileName = _pathPrj + "/" + fileName;

  QFile file(fileName);
  QDomDocument domDoc;
  QString errorStr;
  int errorLine;
  int errorColumn;

  if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
  {
    qDebug() << 
      QString(
          "ERROR[builder loader]: tag=%1 load parse file %2 error at line:%3 column:%4 msg: %5")
      .arg(_rootTag).arg(fileName).arg(errorLine).arg(errorColumn).arg(errorStr);
    return;
  }

  qDebug() << 
    QString("MESSAGE[builder loader]: tag=%1 parce file %2")
    .arg(_rootTag).arg(fileName);

  QDomElement rootElement = domDoc.documentElement();

  if(rootElement.tagName() != _rootTag)
  {
    qDebug() << 
      QString("ERROR[builder loader %1 ]:file %2 wrong root element tag name")
      .arg(_rootTag).arg(fileName);
    return;
  }
  loadBuilders(_rootTag, rootElement, _libPaths, _adder);
}

//==============================================================================
static void loadBuilders(
    const QString& _rootTag, 
    const QDomElement& _element, 
    const QStringList& _libPaths,
    std::function<EAddResult(const QString& _name, void* _builder)> _adder)
{

  QDomNode node = _element.firstChild();

  while(!node.isNull())
  {
    if(!node.isElement())
    {
      node = node.nextSibling();
      continue;
    }

    QDomElement el = node.toElement();

    QString libhandler = el.attribute(__slAttributes.handler);
    QString libfilename  = el.attribute(__slAttributes.lib);

    if(libhandler.isNull() || libfilename.isNull())
    {
      node = node.nextSibling();
      continue;
    }


    QLibrary lib;

    //Поиск библиотеки по списку путей.
    for(int i = 0; i < _libPaths.size(); i++)
    {

      qDebug() << QString("lib file: %1/%2").arg(_libPaths[i]).arg(libfilename);

      lib.setFileName(_libPaths[i] + "/" + libfilename);

      if(lib.isLoaded())
      {

        qDebug() << QString("MESSAGE[builder loader]: tag=%1 library %2 is already loaded")
          .arg(_rootTag).arg(lib.fileName());
        break;
      }

      if(lib.load())
      {
        qDebug() << QString("MESSAGE[builder loader]: tag=%1 library %2 is loaded")
          .arg(_rootTag).arg(lib.fileName());
        break;
      }
    }

    if(!lib.isLoaded())
    {
      qDebug() << 
        QString("WARNING[builder loader]: tag=%1 can't load library %2 to load source builder %3")
        .arg(_rootTag).arg(lib.fileName()).arg(el.tagName());
      node = node.nextSibling();
      continue;
    }

    //Загрузка построителей объектов.
    TLibAccessFunc func = (TLibAccessFunc)lib.resolve(libhandler.toStdString().c_str());

    if(!func)
    {
      qDebug() << 
        QString("WARNING[builder loader]: "
            "tag=%1 can't resolve access func %2 in library %3 "
            "to load remote source builder %4")
        .arg(_rootTag).arg(libhandler).arg(lib.fileName()).arg(el.tagName());

      node = node.nextSibling();
      continue;
    }

    switch(_adder(el.tagName(), func()))
    {
    case EAddResult::Added:
      qDebug() << QString("MESSAGE[builder loader]: tag=%1 builder %2 "
        "with access func %3 was added").arg(_rootTag).arg(el.tagName()).arg(libhandler);
      break;

    case EAddResult::Replaced:
      qDebug() << QString("WARNING[builder loader]: tag=%1 builder %2 "
        "with access func %3 was replaced")
        .arg(_rootTag).arg(el.tagName()).arg(libhandler);
      break;
    }
    node = node.nextSibling();
  }
}
