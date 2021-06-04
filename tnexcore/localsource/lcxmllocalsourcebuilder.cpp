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
#include "lcxmllocalsourcebuilder.h"
#include "lclocalsource.h"
#include "LIApplication.h"

#include <QBitArray>
#include <QDomElement>
#include <QDebug>

static const struct
{
  QString id      = "id";
  QString file    = "file";
  QString mapfile = "mapfile";
  QString source  = "source";
  QString type    = "type";
  QString defval  = "defval";
  QString size    = "size";
}__slAttributes;

static const struct
{
  QString localsourcemap  = "localsourcemap";
  QString bits            = "bits";
  QString bytes           = "bytes";
  QString string          = "string";
  QString stream          = "stream";
}__slTags;

static const struct
{
  QString type_uint8    = "uint8";
  QString type_uint16   = "uint16";
  QString type_uint32   = "uint32";
  QString type_int8     = "int8";
  QString type_int16    = "int16";
  QString type_int32    = "int32";
  QString type_float32  = "float32";
  QString type_string   = "string";
  QString type_string16 = "string16";
  QString type_hex      = "hex";
}__slBytesTypes;

//==============================================================================
LCXmlLocalSourceBuilder::LCXmlLocalSourceBuilder()
{
}

//------------------------------------------------------------------------------
static LQDataSources buildLocal(
    const QDomElement& _element, const LIApplication& _app);
//------------------------------------------------------------------------------
LQDataSources LCXmlLocalSourceBuilder::build(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_element);
  Q_UNUSED(_app);

  QString attr_file = _element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return LQDataSources();
    if(el.tagName() != _element.tagName()) return LQDataSources();
    return build(el, _app);
  }

  return buildLocal( _element, _app);
}

//==============================================================================buildLocal
static void buildSource(
    const QDomElement _element, 
    LQDataSources& _sources, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
static LQDataSources buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  LQDataSources sources;

  for(auto node  = _element.firstChildElement(__slAttributes.source);
      !node.isNull();
      node = node.nextSiblingElement(__slAttributes.source))
  {
    buildSource(node.toElement(), sources, _app);
  }
  return sources;
}

//==============================================================================buildSource
static void loadMap(
    LCLocalDataSource& _source, 
    const QDomElement& _map_element, 
    const LIApplication& _app);

static void buildSource(
    const QDomElement _element, 
    LQDataSources& _sources, 
    const LIApplication& _app)
{
  QString attr_id = _element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;
  QString attr_mapfile = _element.attribute(__slAttributes.mapfile);
  if(attr_mapfile.isNull()) return;
  QDomElement map_element = _app.getDomDocument(attr_mapfile).documentElement();
  if(map_element.isNull()) return;
  if(map_element.tagName() != __slTags.localsourcemap) return;

  auto source = LCLocalDataSource::create();
  loadMap(*source.data(), map_element, _app);

  _sources.insert(attr_id, source);
}

//==============================================================================loadMap
static void addItemBits(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element);

static void addItemBytes(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element);

static void addItemString(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element);

static void addItemStream(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element);

//------------------------------------------------------------------------------
static void loadMap(
    LCLocalDataSource& _source, 
    const QDomElement& _map_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_app);
  for(auto node =_map_element.firstChild();
     !node.isNull();
    node = node.nextSibling())
  {
    if(!node.isElement()) continue;
    QDomElement el = node.toElement();

    if(el.tagName() == __slTags.bits)
    {
      addItemBits(_source, el);
    }
    else if(el.tagName() == __slTags.bytes)
    {
      addItemBytes(_source, el);
    }
    else if(el.tagName() == __slTags.string)
    {
      addItemString(_source, el);
    }
    else if(el.tagName() == __slTags.stream)
    {
      addItemStream(_source, el);
    }
  } 
}


//==============================================================================addItemBits
static void addItemBits(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element)
{

  qDebug() << "Local source builder: add item bits";
  
  QString attr_id= _item_element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;

  QString attr = _item_element.attribute(__slAttributes.size);

  if(!attr.isNull())
  {
    int size = 0;
    size = attr.toInt();
    if(size > 0) _source.addBitItem(attr_id, QBitArray(size, false));
    return;
  }

  attr = _item_element.attribute(__slAttributes.defval);
  if(attr.isNull()) return;

  //Удаление символов форматирования строки.
  attr.remove(QRegExp(  QStringLiteral("[ ]{1,}|[_]{1,}")));
  if(attr.size() == 0) return;
  //Проверка на посторонние символы.
  if( attr.contains( QRegExp( QStringLiteral("[^0-1]{1,}")))) return;

  //Формирование массива.
  QBitArray bits(attr.size(), false);
  for(int i = 1; i <= attr.size(); i++)
  {
    if(attr.at(attr.size() - i) == QStringLiteral("1"))
    {
      bits[i-1] = true;
    }
  } 

  _source.addBitItem(attr_id, bits);
}


//==============================================================================addItemBytes
#include <functional>
#include <limits>
static class CBytesCreator
{

public:
  QMap<
    QString, 
    std::function<
      void(
          LCLocalDataSource&,
          const QString& _dataName,
          const QString& _defVal
          )>> mCreatorsMap;
public:
  CBytesCreator()
  {

    //-----------------------------------------------------uint8
    mCreatorsMap.insert( __slBytesTypes.type_uint8, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            qint16 data = _defVal.toShort(&flag,0);
            if(flag && ((data & 0x00ff) == data))
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(quint8));
              _source.addByteItem(_dataName, byte_array);
            }
          }
          /* else */
          /* { */
          /*   QByteArray byte_array = QByteArray(sizeof(quint8),0); */
          /*   _source.addByteItem(_dataName, byte_array); */
          /* } */
        });

    //-----------------------------------------------------int8
    mCreatorsMap.insert( __slBytesTypes.type_int8, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal) 
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            qint16 data = _defVal.toShort(&flag,0);
            if(flag) {
              if((data <= std::numeric_limits<qint8>::max())&&
                  (data >= std::numeric_limits<qint8>::min()))
              {
                QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(qint8));
                _source.addByteItem(_dataName, byte_array);
              }
            }
          }
          /* else */
          /* { */
          /*   QByteArray byte_array = QByteArray(sizeof(qint8),0); */
          /*   _source.addByteItem(_dataName, byte_array); */
          /* } */
        });

    //-----------------------------------------------------uint16
    mCreatorsMap.insert( __slBytesTypes.type_uint16, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            quint16 data = _defVal.toUShort(&flag,0);
            if(flag)
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(quint16));
              _source.addByteItem(_dataName, byte_array);
            }
          }
          /* else */
          /* { */
          /*   QByteArray byte_array = QByteArray(sizeof(quint16),0); */
          /*   _source.addByteItem(_dataName, byte_array); */
          /* } */
        });

    //-----------------------------------------------------int16
    mCreatorsMap.insert( __slBytesTypes.type_int16, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            qint16 data = _defVal.toShort(&flag,0);
            if(flag)
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(qint16));
              _source.addByteItem(_dataName, byte_array);
            }
          }
          /* else */
          /* { */
          /*   QByteArray byte_array = QByteArray(sizeof(qint16),0); */
          /*   _source.addByteItem(_dataName, byte_array); */
          /* } */
        });

    //-----------------------------------------------------uint32
    mCreatorsMap.insert( __slBytesTypes.type_uint32, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            quint32 data = _defVal.toUInt(&flag,0);
            if(flag)
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(quint32));
              _source.addByteItem(_dataName, byte_array);
            }
          }
        });

    //-----------------------------------------------------int32
    mCreatorsMap.insert( __slBytesTypes.type_int32, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            qint32 data = _defVal.toInt(&flag,0);
            if(flag)
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(qint32));
              _source.addByteItem(_dataName, byte_array);
            }
          }
        });

    //-----------------------------------------------------float32
    mCreatorsMap.insert( __slBytesTypes.type_float32, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            bool flag = false;
            float data = _defVal.toFloat(&flag);
            if(flag)
            {
              QByteArray byte_array = QByteArray(
                  reinterpret_cast<char*>(&data), sizeof(float));
              _source.addByteItem(_dataName, byte_array);
            }
          }
        });

    //-----------------------------------------------------string
    mCreatorsMap.insert( __slBytesTypes.type_string, 
        [](LCLocalDataSource& _source, 
          const QString& _dataName, 
          const QString& _defVal)
        {
          if(!_defVal.isNull())
          {
            if(_defVal.size() == 0) return;
            _source.addByteItem(_dataName, _defVal.toUtf8());
          }
        });

  }
} __slTypedBytesCreator;

//------------------------------------------------------------------------------
static void addItemBytes(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element)
{

  QString attr_id = _item_element.attribute(__slAttributes.id);

  if(attr_id.isNull()) return;

  QString attr = _item_element.attribute(__slAttributes.size);

  if(!attr.isNull())
  {
    int size = 0;
    size = attr.toInt();
    if(size > 0) _source.addByteItem(attr_id, QByteArray(size, 0)); 
    return;
  }

  attr = _item_element.attribute(__slAttributes.type);

  if(attr.isNull()) return;
  auto creator_it = __slTypedBytesCreator.mCreatorsMap.find(attr);
  if(creator_it == __slTypedBytesCreator.mCreatorsMap.end()) return;

  creator_it.value()(
      _source, 
      attr_id, 
      _item_element.attribute(__slAttributes.defval));
}

//------------------------------------------------------------------------------
static void addItemString(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element)
{
  QString attr_id = _item_element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;
  _source.addStringItem(attr_id, _item_element.attribute(__slAttributes.defval));
}

//------------------------------------------------------------------------------
static void addItemStream(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element)
{
  QString attr_id = _item_element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;
  _source.addStreamItem(attr_id);
}


