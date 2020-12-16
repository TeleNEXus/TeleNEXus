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
#include "lcformatterbitfield.h"
#include <QDebug>
#include <functional>

//==============================================================================
LCFormatterBitfield::
CValidator::CValidator(int _size, QChar _separator, QObject *_parent) : 
  QValidator(_parent),
  mSize(_size),
  mSeparator(_separator)
{
}

//-----------------------------------------------------------------------------
QValidator::State 
LCFormatterBitfield::
CValidator::validate(QString &_input, int& _pos) const
{
  Q_UNUSED(_pos);
  QString instr = _input;

  instr.remove(QRegExp(QString("[ _%1]").arg(mSeparator)));

  // Определить наличие нечисловых значений после удаления 
  // сепараторов и длины строки при явном указании длины
  // данных в байтах.    
  if( ( instr.contains( QRegExp( QString("[^0-1]{1,}"))) ) || 
      ( ( mSize > 0) && (instr.size() > (8 * mSize) )) )
  {
    return State::Invalid;
  }

  //Проверка на ненулевую строку.
  if(instr.isNull())
  {
    return State::Intermediate;
  }

  return State::Acceptable;
}

//==============================================================================
LCFormatterBitfield::LCFormatterBitfield( 
    int     _size,
    QChar   _separator) :  
  mValidator(_size, _separator)
{
}

//------------------------------------------------------------------------------
LCFormatterBitfield::
LCFormatterBitfield( const LCFormatterBitfield& _formatter)
{
  mValidator.mSize          = _formatter.mValidator.mSize;         
  mValidator.mSeparator     = _formatter.mValidator.mSeparator;    
}
//------------------------------------------------------------------------------
LCFormatterBitfield::~LCFormatterBitfield()
{
}

  LCFormatterBitfield& 
LCFormatterBitfield::operator=(const LCFormatterBitfield& _formatter)
{
  mValidator.mSize          = _formatter.mValidator.mSize;         
  mValidator.mSeparator     = _formatter.mValidator.mSeparator;    
  return *this;
}

//------------------------------------------------------------------------------toString
QString LCFormatterBitfield::toString(const QByteArray& _data)
{
  QString str;
  if(_data.size() < 1)
  {
    str = "Wrong";
    wrongState(str);
    return str;
  }

  for(int i = (_data.size() - 1); i >= 0; i--)
  {
    if(mValidator.mSeparator.isNull())
    {
      str = str + QString("%1").arg(
          ((quint8*)_data.constData())[i], 8, 2, QChar('0'));
    }
    else
    {
      str = str + QString("%1%2").arg(
          ((quint8*)_data.constData())[i], 8, 2, QChar('0')).
        arg(mValidator.mSeparator);
    }
  }

  if(!mValidator.mSeparator.isNull())
  {
    str.resize(str.length() -1);
  }
  return str;
}

//-----------------------------------------------------------------------------
static inline void __l_byte_align(QString& _instr)
{
  int remains = _instr.size() % 8;
  if( remains != 0)
  {
    //Добавляем незначащие нули.
    _instr.insert(0, QString(8-remains, '0'));
  }
}
//------------------------------------------------------------------------------fitting
QString LCFormatterBitfield::fitting(const QString& _instr)
{
  QString out_string = _instr;

  //Удаляем разделительные символы.
  out_string.remove(QRegExp(  QString("[ ]{1,}|[_]{1,}|[%1]{1,}")
        .arg(mValidator.mSeparator) ));

  //Проверяем на строку нулевой длины.
  if( out_string.size() <= 0)
  {
    return QString();
  }

  //Проверяем на наличие нецифровых значений.
  if( out_string.contains(QRegExp("[^0-1]{1,}")))
  {
    //Если присутствуют посторонние символы, 
    //возвращаем пустую строку.
    return QString();
  }

  //Переводим строку в нижний регистр.
  out_string  = out_string.toLower(); 

  if(mValidator.mSize <= 0)
  {
    //Если размер не задан, то производим нормализацию 
    //до количества символов кратного восьми и проверку значений.
    __l_byte_align(out_string);
    return out_string;
  }
  //Если установлен размер данных.
  //Удаляем незначащие нули.
  out_string.remove(QRegExp("^[0]{1,}"));

  //Выравниваем количество бит по байту в 8 бит.
  __l_byte_align(out_string);

  int str_byte_size = out_string.length() / 8;

  if( str_byte_size > mValidator.mSize )
  {
    //Удаляем лишние цифры.
    out_string.remove(0, (str_byte_size - mValidator.mSize) * 8);
  }
  else if( mValidator.mSize > str_byte_size )
  {
    //Добавляем незначащий ноль.
    out_string.insert(0, QString((mValidator.mSize - str_byte_size) * 8, '0'));
  }
  return out_string;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterBitfield::toBytes(const QString& _str)
{
  QByteArray out_array;
  QString instr = _str;

  //Удаление всех незначащих символов.
  instr.remove(   QRegExp(
        QString(
          "[ ]{1,}|[_]{1,}|[%1]{1,}").arg(mValidator.mSeparator) ));

  // Проверка на нулевую строку.
  if(instr.length() == 0) return out_array;

  // Проверка на нечисловые значения.
  if(instr.contains(QRegExp("[^0-1]{1,}")))
  {
    return out_array;
  }

  //Выравниваем количество бит по байту 8 бит.
  __l_byte_align(instr);
  //Форматируем строку усли явно задан размер данных в байтах.
  if(mValidator.mSize > 0)
  {
    int str_byte_size = instr.length() / 8;
    if(str_byte_size > mValidator.mSize )
    {
      instr.remove(0, (str_byte_size - mValidator.mSize) * 8);
    }
    else if(mValidator.mSize > str_byte_size)
    {
      instr.insert(0, QString((mValidator.mSize - str_byte_size) * 8, '0'));
    }
  }
  //Декодируем битовое поле в строке в массив байтов.
  for(int i = (instr.length() - 8); i >= 0; i -= 8 )
  {
    char data = instr.mid(i,8).toShort(nullptr, 2);
    out_array.append(1,data);
  }
  return out_array;
}

//------------------------------------------------------------------------------setSize
void LCFormatterBitfield::setSize(int _size)
{
  mValidator.mSize = _size;
}

//------------------------------------------------------------------------------setSeparator
void LCFormatterBitfield::setSeparator(QChar _separator)
{
  mValidator.mSeparator = _separator;
}


