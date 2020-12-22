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
#include <QDomElement>
#include <QDebug>

#define __L_MAX_HEX_SIZE    256
#define __L_MAX_BITS_SIZE   64

static const struct
{
  QString format = "format";
  QString datasize  = "dataSize";
  QString separator = "separator";
}__slAttributes;

static const struct
{
  QString format_bitfield   = "bitfield";
  QString format_bits       = "bits";
  QString format_hex        = "hex";
  QString format_bool       = "bool";
  QString format_uint8      = "uint8";
  QString format_int8       = "int8";
  QString format_uint16     = "uint16";
  QString format_int16      = "int16";
  QString format_uint32     = "uint32";
  QString format_int32      = "int32";
  QString format_float32    = "float32";
  QString format_string     = "string";
}__slStdFormatterNames;

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
     std::function<QSharedPointer<LIDataFormatter>(const QDomElement&)>> 
     __formatterCreators;

QMap <QString, QSharedPointer<LIDataFormatter>> __slStdFormattersMap;

static class CXmlStdDataFormatterFactory final
{

public:
  CXmlStdDataFormatterFactory()
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

    __slStdFormattersMap.insert(__slStdFormatterNames.format_bitfield, __formatterBitfield);
    __slStdFormattersMap.insert(__slStdFormatterNames.format_bits    , __formatterBits    );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_hex     , __formatterHex     );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_bool    , __formatterBool    );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_uint8   , __formatterUint8   );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_int8    , __formatterInt8    );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_uint16  , __formatterUint16  );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_int16   , __formatterInt16   );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_uint32  , __formatterUint32  );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_int32   , __formatterInt32   );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_float32 , __formatterFloat32 );
    __slStdFormattersMap.insert(__slStdFormatterNames.format_string  , __formatterString  );

    //TODO: Добавить подключение сепаратора для форматтеров.

    //--------------------------------------------------------------------------bitfield
    __formatterCreators.insert(__slStdFormatterNames.format_bitfield,
        [](const QDomElement& _element)
        {
          bool ok_size = false;
          int size = 0;
          QChar separator;

          //Получение значения размера данных в байтах.
          QString attr = _element.attribute(__slAttributes.datasize);
          if(!attr.isNull())
          {
            size = attr.toInt(&ok_size);
          }

          //Получение значения разделителя.
          attr = _element.attribute(__slAttributes.separator);
          if(!attr.isNull())
          {
            separator = attr[0];
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
    __formatterCreators.insert(__slStdFormatterNames.format_bits,
        [](const QDomElement& _element)
        {

          bool ok_size = false;
          int size = 0;
          QChar separator;

          //Получение значения размера данных в байтах.
          QString attr = _element.attribute(__slAttributes.datasize);
          if(!attr.isNull())
          {
            size = attr.toInt(&ok_size);
          }

          //Получение значения разделителя.
          attr = _element.attribute(__slAttributes.separator);
          if(!attr.isNull())
          {
            separator = attr[0];
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
    __formatterCreators.insert(__slStdFormatterNames.format_hex,
        [](const QDomElement& _element)
        {
          bool ok_size = false;
          int size = 0;
          QChar separator;

          //Получение значения размера данных в байтах.
          QString attr = _element.attribute(__slAttributes.datasize);
          if(!attr.isNull())
          {
            size = attr.toInt(&ok_size);
          }

          //Получение значения разделителя.
          attr = _element.attribute(__slAttributes.separator);
          if(!attr.isNull())
          {
            separator = attr[0];
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

    //--------------------------------------------------------------------------
    __formatterCreators.insert( __slStdFormatterNames.format_bool, 
        [](const QDomElement& _element) { 
          Q_UNUSED(_element); return __formatterBool;});

    __formatterCreators.insert( __slStdFormatterNames.format_uint8, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterUint8;});

    __formatterCreators.insert( __slStdFormatterNames.format_int8, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterInt8;});

    __formatterCreators.insert( __slStdFormatterNames.format_uint16, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterUint16;});

    __formatterCreators.insert( __slStdFormatterNames.format_int16, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterInt16;});

    __formatterCreators.insert( __slStdFormatterNames.format_uint32, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterUint32;});

    __formatterCreators.insert( __slStdFormatterNames.format_int32, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterInt32;});

    __formatterCreators.insert( __slStdFormatterNames.format_float32, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterFloat32;});

    __formatterCreators.insert( __slStdFormatterNames.format_string, 
        [](const QDomElement& _element){ 
          Q_UNUSED(_element); return __formatterString;});
  }


  //------------------------------------------------------------------------------
  QSharedPointer<LIDataFormatter> createFormatter(const QDomElement& _element)
  {
    auto it = __formatterCreators.find(
        _element.attribute(__slAttributes.format));
    if(it == __formatterCreators.end()) {return nullptr;}
    return it.value()(_element);
  }

  //------------------------------------------------------------------------------
  QSharedPointer<LIDataFormatter> getFormatter( const QString& _name)
  {
    auto it = __slStdFormattersMap.find(_name);
    if(it == __slStdFormattersMap.end()) return nullptr;
    return it.value();
  }

}__slFormattersFactory;

//==============================================================================stddataformatterfactory
namespace stddataformatterfactory
{
QSharedPointer<LIDataFormatter> createFormatter(const QDomElement& _element)
{
  return __slFormattersFactory.createFormatter(_element);
}

QSharedPointer<LIDataFormatter> getFormatter(
    const QString& _name)
{
  return __slFormattersFactory.getFormatter(_name);
}

}
