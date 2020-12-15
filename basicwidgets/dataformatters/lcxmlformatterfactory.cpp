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
#include "lcxmlformatterfactory.h"

#include "lcformatterbitfield.h"
#include "lcformatterbits.h"
#include "lcformatterhex.h"
#include "lcformatterbool.h"
#include "lcformatteru8.h"
#include "lcformatters8.h"
#include "lcformatteru16.h"
#include "lcformatters16.h"
#include "lcformatteru32.h"
#include "lcformatters32.h"
#include "lcformatterf32.h"
#include "lcformatterstring.h"

#include <QSharedPointer>

#include <functional>
#include <QMap>

#include <QDebug>

#define __L_MAX_HEX_SIZE    256
#define __L_MAX_BITS_SIZE   64

static const struct
{
    QString datasize = "dataSize";
    QString separator = "separator";
}__attributes;

//------------------------------------------------------------------------------

static QSharedPointer<LIDataFormatter> __formatterBitfield;
static QSharedPointer<LIDataFormatter> __formatterBits;
static QSharedPointer<LIDataFormatter> __formatterHex;
static QSharedPointer<LIDataFormatter> __formatterBool;
static QSharedPointer<LIDataFormatter> __formatterUint8;
static QSharedPointer<LIDataFormatter> __formatterInt8;
static QSharedPointer<LIDataFormatter> __formatterUint16;
static QSharedPointer<LIDataFormatter> __formatterInt16;
static QSharedPointer<LIDataFormatter> __formatterUint32;
static QSharedPointer<LIDataFormatter> __formatterInt32;
static QSharedPointer<LIDataFormatter> __formatterFloat32;
static QSharedPointer<LIDataFormatter> __formatterString;

//==============================================================================

QMap < QString, 
     std::function<QSharedPointer<LIDataFormatter>(const QDomNamedNodeMap& _attr)>> 
     __formatterCreators;

     struct SFormatterCreator1
{
  SFormatterCreator1(){}
  QSharedPointer<LIDataFormatter> 
    operator()(const QDomNamedNodeMap& _attr)
    {
      QDomNode node;

      bool ok_size = false;
      int size = 0;
      QChar separator;

      //Получение значения размера данных в байтах.
      node = _attr.namedItem(__attributes.datasize);
      if(!node.isNull())
      {
        size = node.toAttr().value().toInt(&ok_size);
      }

      //Получение значения разделителя.
      node = _attr.namedItem(__attributes.separator);
      if(!node.isNull())
      {
        separator = node.toAttr().value()[0];
      }
      //Проверяем наличие дополнительных параметров.
      if( ((!ok_size)||(size <= 0)) && (separator.isNull()))
      {
        return __formatterBitfield;
      }

      LCFormatterBitfield *formatter = 
        new LCFormatterBitfield();

      //Копируем параметры форматтера по умолчанию.
      *formatter = *(static_cast<LCFormatterBitfield*>
          (__formatterBitfield.data()));

      //Установка значения размера данных в байтах.
      if(size > __L_MAX_BITS_SIZE) size = __L_MAX_BITS_SIZE;
      if(size > 0) 
      {
        formatter->setSize(size);
      }

      //Установка сепаратора.
      formatter->setSeparator(separator); 
      return QSharedPointer<LIDataFormatter>(formatter); 
    }
};

LCXmlStdDataFormatterFactory::LCXmlStdDataFormatterFactory()
{
  __formatterBitfield.reset(    new LCFormatterBitfield());
  __formatterBits.reset(        new LCFormatterBits());
  __formatterHex.reset(         new LCFormatterHex());
  __formatterBool.reset(        new LCFormatterBool());
  __formatterUint8.reset(       new LCFormatterU8());
  __formatterInt8.reset(        new LCFormatterS8());
  __formatterUint16.reset(      new LCFormatterU16());
  __formatterInt16.reset(       new LCFormatterS16());
  __formatterUint32.reset(      new LCFormatterU32());
  __formatterInt32.reset(       new LCFormatterS32());
  __formatterFloat32.reset(     new LCFormatterF32());
  __formatterString.reset(      new LCFormatterString());

  //TODO: Добавить подключение сепаратора для форматтеров.

  //--------------------------------------------------------------------------bitfield
  __formatterCreators.insert("bitfield",
      [](const QDomNamedNodeMap& _attr)
      {
        QDomNode node;

        bool ok_size = false;
        int size = 0;
        QChar separator;

        //Получение значения размера данных в байтах.
        node = _attr.namedItem(__attributes.datasize);
        if(!node.isNull())
        {
          size = node.toAttr().value().toInt(&ok_size);
        }

        //Получение значения разделителя.
        node = _attr.namedItem(__attributes.separator);
        if(!node.isNull())
        {
          separator = node.toAttr().value()[0];
        }
        //Проверяем наличие дополнительных параметров.
        if( ((!ok_size)||(size <= 0)) && (separator.isNull()))
        {
          return __formatterBitfield;
        }

        LCFormatterBitfield *formatter = 
          new LCFormatterBitfield();

        //Копируем параметры форматтера по умолчанию.
        *formatter = *(static_cast<LCFormatterBitfield*>
            (__formatterBitfield.data()));

        //Установка значения размера данных в байтах.
        if(size > __L_MAX_BITS_SIZE) size = __L_MAX_BITS_SIZE;
        if(size > 0) 
        {
          formatter->setSize(size);
        }

        //Установка сепаратора.
        formatter->setSeparator(separator); 
        return QSharedPointer<LIDataFormatter>(formatter); 
      });

  //--------------------------------------------------------------------------bits
  __formatterCreators.insert("bits",
      [](const QDomNamedNodeMap& _attr)
      {
        QDomNode node;

        bool ok_size = false;
        int size = 0;
        QChar separator;

        //Получение значения размера данных в байтах.
        node = _attr.namedItem(__attributes.datasize);
        if(!node.isNull())
        {
          size = node.toAttr().value().toInt(&ok_size);
        }

        //Получение значения разделителя.
        node = _attr.namedItem(__attributes.separator);
        if(!node.isNull())
        {
          separator = node.toAttr().value()[0];
        }
        //Проверяем наличие дополнительных параметров.
        if( ((!ok_size)||(size <= 0)) && (separator.isNull()))
        {
          return __formatterBits;
        }

        LCFormatterBits *formatter = 
          new LCFormatterBits();

        //Копируем параметры форматтера по умолчанию.
        *formatter = *(static_cast<LCFormatterBits*>
            (__formatterBits.data()));

        //Установка значения размера данных в байтах.
        if(size > __L_MAX_BITS_SIZE) size = __L_MAX_BITS_SIZE;
        if(size > 0) 
        {
          formatter->setSize(size);
        }

        //Установка сепаратора.
        formatter->setSeparator(separator); 
        return QSharedPointer<LIDataFormatter>(formatter); 
      });

  //--------------------------------------------------------------------------hex
  __formatterCreators.insert("hex",
      [](const QDomNamedNodeMap& _attr)
      {
        QDomNode node;
        bool ok_size = false;
        int size = 0;
        QChar separator;

        //Получение значения размера данных в байтах.
        node = _attr.namedItem(__attributes.datasize);
        if(!node.isNull())
        {
          size = node.toAttr().value().toInt(&ok_size);
        }

        //Получение значения разделителя.
        node = _attr.namedItem(__attributes.separator);
        if(!node.isNull())
        {
          separator = node.toAttr().value()[0];
        }
        //Проверяем наличие дополнительных параметров.
        if( ((!ok_size)||(size <= 0)) && (separator.isNull()))
        {
          return __formatterHex;
        }

        LCFormatterHex *formatter = 
          new LCFormatterHex();

        //Копируем параметры форматтера по умолчанию.
        *formatter = *(static_cast<LCFormatterHex*>
            (__formatterHex.data()));

        //Установка значения размера данных в байтах.
        if(size > __L_MAX_BITS_SIZE) size = __L_MAX_BITS_SIZE;
        if(size > 0) 
        {
          formatter->setSize(size);
        }

        //Установка сепаратора.
        formatter->setSeparator(separator); 
        return QSharedPointer<LIDataFormatter>(formatter); 
      });

  //--------------------------------------------------------------------------bool
  __formatterCreators.insert("bool",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterBool;});
  __formatterCreators.insert("uint8",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterUint8;});
  __formatterCreators.insert("int8",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterInt8;});
  __formatterCreators.insert("uint16",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterUint16;});
  __formatterCreators.insert("int16",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterInt16;});
  __formatterCreators.insert("uint32",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterUint32;});
  __formatterCreators.insert("int32",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterInt32;});
  __formatterCreators.insert("float32",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterFloat32;});
  __formatterCreators.insert("string",
      [](const QDomNamedNodeMap& _attr){
        Q_UNUSED(_attr); return __formatterString;});
}

//------------------------------------------------------------------------------
LCXmlStdDataFormatterFactory& LCXmlStdDataFormatterFactory::instance()
{
  static LCXmlStdDataFormatterFactory ins;
  return ins;
}

//------------------------------------------------------------------------------
QSharedPointer<LIDataFormatter> 
LCXmlStdDataFormatterFactory::
createStringFormatter( const QDomNamedNodeMap& _attr)
{
  QDomNode node = _attr.namedItem("format");
  if(node.isNull()) return nullptr;

  QString format = node.toAttr().value();

  auto it = __formatterCreators.find(format);
  if(it == __formatterCreators.end())
  {
    return __formatterHex;
  }
  return it.value()(_attr);
}
