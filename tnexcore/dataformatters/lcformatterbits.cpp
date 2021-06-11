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
#include "lcformatterbits.h"
#include <QDebug>
#include <functional>

//==============================================================================
LCFormatterBits::
    CValidator::CValidator(int _size, QChar _separator, QObject *_parent) : 
    QValidator(_parent),
    mSize(_size),
    mSeparator(_separator)
{
}

//-----------------------------------------------------------------------------
QValidator::State 
LCFormatterBits::
CValidator::validate(QString &_input, int& _pos) const
{
    Q_UNUSED(_pos);
    QString instr = _input;
    instr.remove(QRegExp(QString("[ _%1]").arg(mSeparator)));
    if(instr.isNull())
    {
        return State::Intermediate;
    }
    //Определить наличие нечисловых значений после удаления 
    //сепараторов.
    if( instr.contains( QRegExp( QString("[^0-1]{1,}"))) )
    {
        return State::Invalid;
    }
    //Проверка длины строки при явном указании длины
    //данных в байтах.
    if( ( mSize > 0) && (instr.size() > mSize ))
    {
        return State::Invalid;
    }

    return State::Acceptable;
}

//==============================================================================
LCFormatterBits::LCFormatterBits( 
        int     _size,
        QChar   _separator) :  
    mValidator(_size, _separator)
{
}

//------------------------------------------------------------------------------
LCFormatterBits::
LCFormatterBits( const LCFormatterBits& _formatter)
{
    mValidator.mSize      = _formatter.mValidator.mSize;         
    mValidator.mSeparator = _formatter.mValidator.mSeparator;    
}
//------------------------------------------------------------------------------
LCFormatterBits::~LCFormatterBits()
{
}

LCFormatterBits& 
LCFormatterBits::operator=(const LCFormatterBits& _formatter)
{
    mValidator.mSize          = _formatter.mValidator.mSize;         
    mValidator.mSeparator     = _formatter.mValidator.mSeparator;    
    return *this;
}
//------------------------------------------------------------------------------toString
QString LCFormatterBits::toString(const QByteArray& _data)
{
    QString out_str;

    if(_data.size() < 1)
    {
      out_str = "Wrong";
      wrongState(out_str);
      return out_str;
    }

    for(int i = (_data.size() - 1); i >= 0; i--)
    {
        if((quint8)_data.constData()[i] != 0)
        {
            out_str += '1';
        }
        else
        {
            out_str += '0';
        }

        if(!mValidator.mSeparator.isNull())
        {
            if( (i - 1) >= 0) out_str += ' ';
        }
    }
    return out_str;
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterBits::toBytes(const QString& _str)
{
    QByteArray arr;
    QString instr = _str;

    instr.remove(QRegExp(QString("[ _%1]{1,}").arg(mValidator.mSeparator)));

    for(int i = (instr.length() - 1); i >= 0; i -= 1)
    {
        if(instr[i] == '0')
        {
            arr.append(1, 0);
        }
        else
        {
            arr.append(1, 1);
        }
    }
    return arr;
}

//------------------------------------------------------------------------------setSize
void LCFormatterBits::setSize(int _size)
{
    mValidator.mSize = _size;
}

//------------------------------------------------------------------------------setSeparator
void LCFormatterBits::setSeparator(QChar _separator)
{
    mValidator.mSeparator = _separator;
}
